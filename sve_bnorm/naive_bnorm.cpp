#include <math.h>
void get_mean_and_variance_naive(float *src, float *mean, float *variance,
                                 int ndim, int *dims) {
  int valid_cells = 1, C = dims[ndim - 1];
  for (int i = 0; i < ndim - 1; ++i) {
    valid_cells *= dims[i];
  }
  for (int i = 0; i < C; ++i) {
    mean[i]     = 0.f;
    variance[i] = 0.f;
  }
  for (int i = 0; i < valid_cells; ++i) {
    for (int c = 0; c < C; ++c) {
      mean[c] += src[i * C + c];
    }
  }

  for (int c = 0; c < C; ++c) {
    mean[c] *= float(1.f / valid_cells);
  }

  for (int i = 0; i < valid_cells; ++i) {
    for (int c = 0; c < C; ++c) {
      variance[c] += pow((src[i * C + c] - mean[c]), 2);
    }
  }
}

void shl_ref_batch_normalization_f32(float *input, float *mean, float *variance,
                                     float *beta, float *output, float epsilon,
                                     float *gamma, int ndim, int *dims) {
  float *input_data  = input;
  float *mean_data   = mean;
  float *var_data    = variance;
  float *beta_data   = beta;
  float *output_data = output;
  int    batches     = 1;

  /* compute the outer size */
  for (int i = 0; i < ndim - 1; i++) {
    batches *= dims[i];
  }

  int batch_offset = dims[ndim - 1];

  for (int b = 0; b < batches; ++b) {
    for (int c = 0; c < dims[ndim - 1]; ++c) {
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