#include <arm_sve.h>
#include <math.h>

void batchnorm_sve_nhwc(float *input, float *mean, float *variance, float *beta,
                        float *output, float epsilon, float *gamma, int ndim,
                        int *dims);

void get_mean_and_variance_nhwc_sve(float *src, float *mean, float *variance,
                                    int ndim, int *dims);

#ifdef ENABLE_SVE
// ? nchw
// ? mean [c]
// ? var [c]
// ? beta [c]
// ? gamma [c]
// ? input [n c hw]
// ? output [n c hw]

void batchnorm_nchw_sve(float *input, float *mean, float *variance, float *beta,
                        float *output, float epsilon, float *gamma, int ndim,
                        int *dims) {}

void get_mean_and_variance_nchw_sve(float *src, float *mean, float *variance,
                                    int ndim, int *dims) {}
#endif

void get_mean_and_variance_naive(float *src, float *mean, float *variance,
                                 int ndim, int *dims);

void shl_ref_batch_normalization_f32(float *input, float *mean, float *variance,
                                     float *beta, float *output, float epsilon,
                                     float *gamma, int ndim, int *dims);