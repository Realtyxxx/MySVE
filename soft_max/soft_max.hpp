#include <arm_sve.h>
#include <float.h>
#include <cmath>
#include <iostream>
#include "align_malloc.hpp"
#include "limits"

inline svfloat32_t svtaylor_poly_f32_z(svbool_t pg, svfloat32_t x, svfloat32_t coeff_1, svfloat32_t coeff_2, svfloat32_t coeff_3, svfloat32_t coeff_4, svfloat32_t coeff_5, svfloat32_t coeff_6,
                                       svfloat32_t coeff_7, svfloat32_t coeff_8) {
  const auto A   = svmla_f32_z(pg, coeff_1, coeff_5, x);
  const auto B   = svmla_f32_z(pg, coeff_3, coeff_7, x);
  const auto C   = svmla_f32_z(pg, coeff_2, coeff_6, x);
  const auto D   = svmla_f32_z(pg, coeff_4, coeff_8, x);
  const auto x2  = svmul_f32_z(pg, x, x);
  const auto x4  = svmul_f32_z(pg, x2, x2);
  const auto res = svmla_f32_z(pg, svmla_f32_z(pg, A, B, x2), svmla_f32_z(pg, C, D, x2), x4);
  return res;
}

inline svfloat32_t svexp_f32_z(svbool_t pg, svfloat32_t x) {
  const auto CONST_LN2          = svdup_n_f32(0.6931471805f);  // ln(2)
  const auto CONST_INV_LN2      = svdup_n_f32(1.4426950408f);  // 1/ln(2)
  const auto CONST_INF          = svdup_n_f32(std::numeric_limits<float>::infinity());
  const auto CONST_MAX_INPUT    = svdup_n_f32(88.7f);
  const auto CONST_0            = svdup_n_f32(0.f);
  const auto CONST_NEGATIVE_126 = svdup_n_s32(-126);

  /** Exponent polynomial coefficients */
  const svfloat32_t exp_tab_1 = svdup_n_f32(1.f);
  const svfloat32_t exp_tab_2 = svdup_n_f32(0.0416598916054f);
  const svfloat32_t exp_tab_3 = svdup_n_f32(0.500000596046f);
  const svfloat32_t exp_tab_4 = svdup_n_f32(0.0014122662833f);
  const svfloat32_t exp_tab_5 = svdup_n_f32(1.00000011921f);
  const svfloat32_t exp_tab_6 = svdup_n_f32(0.00833693705499f);
  const svfloat32_t exp_tab_7 = svdup_n_f32(0.166665703058f);
  const svfloat32_t exp_tab_8 = svdup_n_f32(0.000195780929062f);

  // Perform range reduction [-log(2),log(2)]
  auto m   = svcvt_s32_f32_z(pg, svmul_f32_z(pg, x, CONST_INV_LN2));
  auto val = svmls_f32_z(pg, x, svcvt_f32_s32_z(pg, m), CONST_LN2);

  // Polynomial Approximation
  auto poly = svtaylor_poly_f32_z(pg, val, exp_tab_1, exp_tab_2, exp_tab_3, exp_tab_4, exp_tab_5, exp_tab_6, exp_tab_7, exp_tab_8);

  // Reconstruct
  poly = svreinterpret_f32_s32(svqadd_s32(svreinterpret_s32_f32(poly), svlsl_n_s32_z(pg, m, 23)));

  // Handle underflow
  svbool_t ltpg = svcmplt_s32(pg, m, CONST_NEGATIVE_126);
  poly          = svsel_f32(ltpg, CONST_0, poly);

  // Handle overflow
  svbool_t gtpg = svcmpgt_f32(pg, x, CONST_MAX_INPUT);
  poly          = svsel_f32(gtpg, CONST_INF, poly);

  return poly;
}

void logits_1d_max(const float* in, float* out, int workSize) {
  auto       vec_max     = svdup_n_f32(-FLT_MAX);
  int        x           = 0;
  auto       pg          = svwhilelt_b32(x, workSize);
  const auto all_true_pg = svptrue_b32();
  do {
    // compute
    auto current_value = svld1(pg, in + x);
    vec_max            = svmax_m(pg, vec_max, current_value);

    // iterate
    x += svcntw();
    pg = svwhilelt_b32(x, workSize);
  } while (svptest_any(all_true_pg, pg));
  auto maxVal = svmaxv(all_true_pg, vec_max);
  *out        = maxVal;
}

// * max 为 workNum大小的向量
// * exp_tmp 为workSize大小的向量，可以重复使用
// * in， out 为 workNum * workSize大小
template <bool is_log>
void logits_1d_soft_max(float* in, float* exp_tmp, float* max, float* out, const float beta, int workSize, int workNum) {
  const auto all_true_pg = svptrue_b32();
  for (int i = 0; i < workNum; ++i) {
    float* in_ptr  = in + i * workSize;
    float* out_ptr = out + i * workSize;
    float* max_ptr = max + i;

    float sum = 0;

    {
      //* compute exponentials and sum
      //* get max value
      const auto max_val  = *max_ptr;
      const auto vec_max  = svdup_n_f32(max_val);
      const auto vec_beta = svdup_n_f32(beta);

      //* init sum to zero
      auto     vec_sum = svdup_n_f32(0.f);
      int      x       = 0;
      svbool_t pg      = svwhilelt_b32(x, workSize);
      do {
        auto vec_elements = svld1(pg, in_ptr + x);
        vec_elements      = svmul_z(pg, svsub_z(pg, vec_elements, vec_max), vec_beta);
        if (!is_log) {
          vec_elements = svexp_f32_z(pg, vec_elements);
          vec_sum      = svadd_m(pg, vec_sum, vec_elements);
        }
        svst1(pg, exp_tmp + x, vec_elements);
        if (is_log) {
          vec_sum = svadd_m(pg, vec_sum, svexp_f32_z(pg, vec_elements));
        }
        x += svcntw();
        pg = svwhilelt_b32(x, workSize);
      } while (svptest_any(all_true_pg, pg));

      // Reduce sum
      sum = svaddv(all_true_pg, vec_sum);
      if (is_log) {
        sum = (float)(log(sum));
      } else {
        sum = 1.f / sum;
      }
    }
    //* Normalize exponentials
    {
      int      x       = 0;
      svbool_t pg      = svwhilelt_b32(x, workSize);
      auto     vec_sum = svdup_n_f32(sum);
      do {
        auto vec_in           = svld1(pg, exp_tmp + x);
        auto normalized_value = svdup_n_f32(0.f);
        if (is_log) {
          normalized_value = svsub_z(pg, vec_in, vec_sum);
        } else {
          normalized_value = svmul_z(pg, vec_in, vec_sum);
        }
        svst1(pg, out_ptr + x, normalized_value);
        x += svcntw();
        pg = svwhilelt_b32(x, workSize);
      } while (svptest_any(all_true_pg, pg));
    }
  }
}

template <bool is_log>
void softmax(float* in, float* out, int workNum, int workSize, float beta) {
  float* max_tmp = malloc_aligned(workNum, 1, sizeof(float));
  float* exp_tmp = malloc_aligned(1, workSize, sizeof(float));
  for (int i = 0; i < workNum; ++i) {
    logits_1d_max(in + workSize * i, max_tmp + i, workSize);
  }
  logits_1d_soft_max<is_log>(in, exp_tmp, max_tmp, out, beta, workSize, workNum);
}
