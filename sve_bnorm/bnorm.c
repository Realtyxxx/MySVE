#include <math.h>
#if defined(__ARM_FEATURE_SVE)
#include <arm_sve.h>
#endif

void shl_ref_batch_normalization_f32(float *input, float *mean, float *variance, float *beta, float *output,
                                     float epsilon, float *gamma, int dims_count,
                                     struct csinn_bn_params *params int *dim) {
  float *input_data  = input;
  float *mean_data   = mean;
  float *var_data    = variance;
  float *beta_data   = beta;
  float *output_data = output;
  int    batches     = 1;

  /* compute the outer size */
  for (int i = 0; i < dims_count - 1; i++) { batches *= dim[i]; }

  int batch_offset = dim[dims_count - 1];

  for (int b = 0; b < batches; ++b) {
    for (int c = 0; c < dim[dims_count - 1]; ++c) {
      float intput_val = input_data[b * batch_offset + c];
      float mean_val   = mean_data[c];
      float var_val    = var_data[c];
      float beta_val   = beta_data[c];
      float result     = 1 / sqrt(var_val + epsilon);
      result *= (intput_val - mean_val);
      if (gamma != NULL) {
        float *gamma_data = gamma;
        result *= gamma_data[c];
      }
      result += beta_val;
      output_data[b * batch_offset + c] = result;
    }
  }
}

void batchnorm_cpu(int ndim, int *dims, float *src, float *mean, float *variance, float *scale, float *shift,
                   float *dst, float *eps) {
  // nchw 布局
  // int pre_last_dim = 1;  // nch
  // for (int i = 0; i < ndim - 1; i++) {
  //   pre_last_dim *= dims[i];
  // }
  int pre_last_dim = 1;
  for (int i = 0; i < ndim - 1; i++) { pre_last_dim *= dims[i]; }
  // get_dim_size(dims, 0, ndim - 2);
  const int w = dims[ndim - 1];  // w

  // 拉成 [nch, w]
  for (int i = 0; i < pre_last_dim; i++) {
    int start_w = i * w;
    int end_w   = start_w + w;  // 这一行的w

    float mean_temp = 0.0;
    float var_temp  = 0.0;

    // 求均值 方差
    for (int j = start_w; j < end_w; j++) {
      mean_temp += src[j];
      var_temp += (src[j] * src[j]);  // 溢出问题?
    }

    mean_temp = mean_temp / w;
    var_temp  = var_temp / w - mean_temp * mean_temp;
    // 保存
    mean[i]     = mean_temp;
    variance[i] = var_temp;

    // 归一化
    var_temp = sqrtf(var_temp + eps[0]);
    for (int j = start_w; j < end_w; j++) {
      dst[j] = (src[j] - mean_temp) / var_temp;
      if (scale != NULL) {
        dst[j] *= scale[j - start_w];
      }
      if (shift != NULL) {
        dst[j] += shift[j - start_w];
      }
    }
  }
}

#if defined(__ARM_FEATURE_SVE)
void batchnorm_sve_nhwc(int ndim, int *dims, float *src, float *dst, float *mean, float *variance, float *scale,
                        float *shift, float *eps, float *gamma, float *beta) {
  const float *input_mean  = mean;
  const float *input_var   = var;
  const float *input_gamma = gamma;
  const float *input_beta  = beta;

  // Global vec
  const svfloat32_t epsilon_vec = svdup_n_f32(epsilon);
  const svfloat32_t const_1     = svdup_n_f32(1.f);
  const svfloat32_t const_0     = svdup_n_f32(0.f);

  int valid_cells = 1;
  for (int i = 0; i < ndim - 1; ++i) { valid_cells *= dims[i]; }
  const int c_start = 0;
  const int c_end   = dims[ndim - 1];
  const int offset  = dims[ndim - 1];

  for (int i = 0; i < valid_cells; ++i) {
    const float *input_ptr  = src + i * offset;
    const float *output_ptr = dst + i * offset;
    int          x          = c_start;
    svbool_t     pg         = svwhilelt_b32(x, c_end);
    do {
      // Basic vecs
      const svfloat32_t mean_vec  = svld1_f32(pg, input_mean + x);
      const svfloat32_t var_vec   = svld1_f32(pg, input_mean + x);
      const svfloat32_t gamma_vec = (input_gamma != NULL) ? svld1_f32(pg, input_mean + x) : const_1;
      const svfloat32_t beta_vec  = (input_beta != NULL) ? svld1_f32(pg, input_beta + x) : const_0;

      // Calculate denominator
      const svfloat32_t tmp         = svadd_f32_z(pg, var_vec, epsilon_vec);
      // svfloat32_t       denominator = svrsqrte_f32(tmp);
      // denominator = svmul_f32_z(pg, svrsqrts_f32(svmul_f32_z(pg, tmp, denominator), denominator), denominator);
      // denominator = svmul_f32_z(pg, svrsqrts_f32(svmul_f32_z(pg, tmp, denominator), denominator), denominator);
#if 1
    svfloat32_t sqrt_vec  = svsqrt_f32_z(pg, tmp);
    svfloat32_t denominator = svdiv_f32_z(pg, const_1, sqrt_vec);
#else
    svfloat32_t denominator = svrsqrte_f32(tmp);
    denominator = svmul_f32_z(pg, svrsqrts_f32(svmul_f32_z(pg, tmp, denominator), denominator), denominator);
    denominator = svmul_f32_z(pg, svrsqrts_f32(svmul_f32_z(pg, tmp, denominator), denominator), denominator);
#endif



      // Calculate x bar
      const svfloat32_t numerator = svsub_f32_z(pg, svld1_f32(pg, input_ptr + x), mean_vec);
      const svfloat32_t x_bar     = svmul_f32_z(pg, numerator, denominator);
      svfloat32_t       res       = svmla_f32_z(pg, beta_vec, x_bar, gamma_vec);
      svst1_f32(pg, output_ptr + x, res);

      x += svcntw();
      pg = svwhilelt_b32(x, c_end);
    } while (svptest_any(svptrue_b32(), pg));
  }
}
#endif
