#include "batch_norm_sve_hpp"
// ? nhwc
// ? mean [c]
// ? var [c]
// ? beta [c]
// ? gamma [c]
// ? input [nhw c]
// ? output [nhw c]


void batchnorm_sve_nhwc(float *input, float *mean, float *variance, float *beta,
                        float *output, float epsilon, float *gamma, int ndim,
                        int *dims) {
  const float *input_mean  = mean;
  const float *input_var   = variance;
  const float *input_gamma = gamma;
  const float *input_beta  = beta;

  // Global vec
  const svfloat32_t epsilon_vec = svdup_n_f32(epsilon);
  const svfloat32_t const_1     = svdup_n_f32(1.f);
  const svfloat32_t const_0     = svdup_n_f32(0.f);

  int valid_cells = 1;
  for (int i = 0; i < ndim - 1; ++i) {
    valid_cells *= dims[i];
  }
  const int c_start = 0;
  const int c_end   = dims[ndim - 1];
  const int offset  = dims[ndim - 1];

  for (int i = 0; i < valid_cells; ++i) {
    const float *input_ptr  = input + i * offset;
    float *      output_ptr = output + i * offset;
    int          x          = c_start;
    svbool_t     pg         = svwhilelt_b32(x, c_end);
    do {
      // Basic vecs
      const svfloat32_t mean_vec = svld1_f32(pg, input_mean + x);
      const svfloat32_t var_vec  = svld1_f32(pg, input_var + x);
      const svfloat32_t gamma_vec =
          (input_gamma != NULL) ? svld1_f32(pg, input_mean + x) : const_1;
      const svfloat32_t beta_vec =
          (input_beta != NULL) ? svld1_f32(pg, input_beta + x) : const_0;

      // Calculate denominator
      const svfloat32_t tmp = svadd_f32_z(pg, var_vec, epsilon_vec);
      // svfloat32_t       denominator = svrsqrte_f32(tmp);
      // denominator = svmul_f32_z(pg, svrsqrts_f32(svmul_f32_z(pg, tmp,
      // denominator), denominator), denominator); denominator = svmul_f32_z(pg,
      // svrsqrts_f32(svmul_f32_z(pg, tmp, denominator), denominator),
      // denominator);
#if 1
      svfloat32_t sqrt_vec    = svsqrt_f32_z(pg, tmp);
      svfloat32_t denominator = svdiv_f32_z(pg, const_1, sqrt_vec);
#else
      svfloat32_t denominator = svrsqrte_f32(tmp);
      denominator             = svmul_f32_z(
          pg, svrsqrts_f32(svmul_f32_z(pg, tmp, denominator), denominator),
          denominator);
      denominator = svmul_f32_z(
          pg, svrsqrts_f32(svmul_f32_z(pg, tmp, denominator), denominator),
          denominator);
#endif

      // Calculate x bar
      const svfloat32_t numerator =
          svsub_f32_z(pg, svld1_f32(pg, input_ptr + x), mean_vec);
      const svfloat32_t x_bar = svmul_f32_z(pg, numerator, denominator);
      svfloat32_t       res   = svmla_f32_z(pg, beta_vec, x_bar, gamma_vec);
      svst1_f32(pg, output_ptr + x, res);

      x += svcntw();
      pg = svwhilelt_b32(x, c_end);
    } while (svptest_any(svptrue_b32(), pg));
  }
}

#include "sve_avg.hpp"
void get_mean_and_variance_nhwc_sve(float *src, float *mean, float *variance,
                                    int ndim, int *dims) {
  int valid_cells = 1, C = dims[ndim - 1];
  for (int i = 0; i < ndim - 1; ++i) {
    valid_cells *= dims[i];
  }
  float **inptrs = (float **)alloca(valid_cells * sizeof(float *));
  for (int i = 0; i < valid_cells; ++i) {
    inptrs[i] = src + i * C;
  }
  sve_fp32_nhwc_avg_generic_depthfirst_impl(valid_cells, valid_cells, C, inptrs,
                                            mean);

  const auto all_true_pg = svptrue_b32();
  int        c           = 0;
  svbool_t   pg          = svwhilelt_b32(c, C);
  do {
    auto       vec_sum  = svdup_n_f32(0.f);
    const auto vec_mean = svld1(pg, mean + c);
    for (int i = 0; i < valid_cells; ++i) {  // loop by cells
      auto vec = svld1(pg, (src + i * C + c));
      vec      = svsub_z(pg, vec, vec_mean);
      vec      = svmul_z(pg, vec, vec);
      vec_sum  = svadd_z(pg, vec_sum, vec);
    }
    svst1(pg, variance + c, vec_sum);
    c += svcntw();
    pg = svwhilelt_b32(c, C);
  } while (svptest_any(all_true_pg, pg));  // loop by channel
}