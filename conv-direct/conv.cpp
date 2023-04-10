#ifdef __ARM_FEATRUE_SVE
#include <arm_sve.h>
#endif

void direct_conv_nchw_f32(float* src, float* dst, float* weight, int batches, int channels, int height, int width,
                          int out_channels, int kernel_h, int kernel_w, int padding, int stride) {
  int output_height = (height + 2 * padding - kernel_h) / stride + 1;
  int output_width  = (width + 2 * padding - kernel_w) / stride + 1;
  for (int oc = 0; oc < out_channels; ++oc) {
    for (int oh = 0; oh < output_height; ++oh) {
      for (int ow = 0; ow < output_width; ++ow) {
      }
    }
  }
}

void im2col_conv_sve()
