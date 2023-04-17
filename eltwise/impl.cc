#include <arm_sve.h>
#include <limits.h>
#include <omp.h>

enum ElementWiseUnary { exp, neg, abs };

enum ElementWiseBinary { max, min, add, sub, mul };

inline svfloat32_t svtaylor_poly_f32_z(svbool_t pg, svfloat32_t x, svfloat32_t coeff_1, svfloat32_t coeff_2,
                                       svfloat32_t coeff_3, svfloat32_t coeff_4, svfloat32_t coeff_5,
                                       svfloat32_t coeff_6, svfloat32_t coeff_7, svfloat32_t coeff_8) {
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
  const auto CONST_LN2     = svdup_n_f32(0.6931471805f);  // ln(2)
  const auto CONST_INV_LN2 = svdup_n_f32(1.4426950408f);  // 1/ln(2)

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
  auto poly = svtaylor_poly_f32_z(pg, val, exp_tab_1, exp_tab_2, exp_tab_3, exp_tab_4, exp_tab_5, exp_tab_6, exp_tab_7,
                                  exp_tab_8);

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

inline void elementwise_unary_impl(svfloat32_t src, svbool_t pg, svfloat32_t dst, ElementWiseUnary op) {
  switch (op) {
    case ElementWiseUnary::exp:
      dst = svexp_f32_z(pg, src);
      break;
    case ElementWiseUnary::neg:
      dst = svneg_z(pg, src);
      break;
    case ElementWiseUnary::abs:
      dst = svabs_z(pg, src);
      break;
    default:
      printf("unsupported");
      break;
  }
}

inline void elementwise_binary_impl(svfloat32_t src0, svfloat32_t src1, svbool_t pg, svfloat32_t dst,
                                    ElementWiseBinary op) {
  switch (op) {
    case ElementWiseUnary::max:
      dst = svmax_z(pg, src0, src1);
      break;
    case ElementWiseUnary::min:
      dst = svmin_z(pg, src0, src1);
      break;
    case ElementWiseUnary::mul:
      dst = svmul_z(pg, src0, src1);
      break;
    case ElementWiseUnary::add:
      dst = svadd_z(pg, src0, src1);
      break;
    case ElementWiseUnary::sub:
      dst = svsub_z(pg, src0, src1);
      break;
    default:
      printf("unsupported");
      break;
  }
}

void elementwise_unary(float* src, int len, ElementWiseuary op) {
  int total_threads = omp_get_max_threads();
  int sve_len       = svcntw();
}

void elementwise_binary(float* src0, float* src1, int len, ElementWiseBinary op) {
  int total_threads = omp_get_max_threads();
  int sve_len       = svcntw();
}
