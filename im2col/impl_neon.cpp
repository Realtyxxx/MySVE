#include <arm_neon.h>

#include <string.h>
#include <cstdint>
#include <iostream>
#include "args.h"
#include "omp.h"

namespace {
bool is_a_ge_zero_and_a_lt_b(int a, int b) {
  if ((0 <= a) && (a < b)) {
    return true;
  } else
    return false;
}
inline void copy(unsigned int valid_len, float *out_ptr, float *in_ptr) {

  int iter16    = valid_len / 16;
  int iter4     = (valid_len - iter16 * 16) / 4;
  int iter_left = (valid_len - iter16 * 16) % 4;

  static const int vec_len  = 4;
  static const int vec_len2 = 8;
  static const int vec_len3 = 12;
  static const int vec_len4 = 16;

  int i;
  for (i = 0; i < iter16; ++i) {
    vst2_f32(out_ptr, vld2_f32(in_ptr));
    vst2_f32(out_ptr + vec_len, vld2_f32(in_ptr + vec_len));
    vst2_f32(out_ptr + vec_len2, vld2_f32(in_ptr + vec_len2));
    vst2_f32(out_ptr + vec_len3, vld2_f32(in_ptr + vec_len3));
    out_ptr += vec_len4;
    in_ptr += vec_len4;
  }
  for (i = 0; i < iter4; ++i) {
    vst2_f32(out_ptr, vld2_f32(in_ptr));
    out_ptr += vec_len;
    in_ptr += vec_len;
  }
  for (int i = 0; i < iter_left; ++i) {
    *out_ptr++ = *in_ptr++;
  }
}

}  // namespace

/*
 * ! this is for nchw transition
 * ! and also is for stride 1
 * ! set the dst place to be 0 before
 */
template <typename dtype>
/**
 * @brief
 *
 * @param src
 * @param dst
 * @param stride
 * @param pad
 * @param image_hw
 * @param kernel_hw
 * @param o_image_hw 卷积结果尺寸
 */
void im2col(void *const src, void *const dst, const stride_args &stride, const padding_args &pad,
            const image_args &image_hw, const kernel_args &kernel_hw, const image_args &o_image_hw) {
  // stride = 1 then it could vectorize
  int channel_size   = image_hw.w * image_hw.h;
  int o_channel_size = o_image_hw.w * o_image_hw.h * kernel_hw.h * kernel_hw.w;
  memset(dst, 0, o_image_hw.h * o_image_hw.w * sizeof(float));

  dtype *src_ptr = (dtype *)src;
  dtype *dst_ptr = (dtype *)dst;
  for (int ic = image_hw.ic; ic--; src_ptr += channel_size, dst_ptr += o_channel_size) {  // big image loop

    int kernel_total;
    int kernel_row, kernel_col;
    // iteration by kernel pos
    int max_threads = omp_get_max_threads();
#pragma omp parallel for num_threads(max_threads) private(kernel_row, kernel_col)
    for (kernel_total = 0; kernel_total < kernel_hw.w * kernel_hw.h; ++kernel_total) {
      kernel_row = kernel_total / kernel_hw.w;
      kernel_col = kernel_total % kernel_hw.w;
      // for (kernel_row = 0; kernel_row < kernel_hw.w; ++kernel_row) {
      //   for (kernel_col = 0; kernel_col < kernel_hw.h; ++kernel_col)
      {

        int input_row = -pad.up + kernel_row;
        for (int out_row = 0; out_row < o_image_hw.h; ++out_row, input_row += stride.h) {
          if (input_row < 0 || input_row >= image_hw.h) {
            continue;
          }
          float *dst_offset = dst_ptr + (kernel_row * kernel_hw.w + kernel_col) * (o_image_hw.w * o_image_hw.h);
          // * here could be optimized
          // #if 1
#if 0 
          int input_col = -pad.left + kernel_col;
          for (int out_col = 0; out_col < o_image_hw.w; ++out_col, input_col += stride.w) {
            if (input_col < 0 || input_col >= image_hw.w) {
              continue;
            }
            dst_offset[out_row * o_image_hw.w + out_col] = src_ptr[input_row * image_hw.w + input_col];
          }
#else
          CHECK(stride.w == 1);
          // vectorize at the input col iteration

          int i_offset    = (kernel_col > pad.left) ? kernel_col - pad.left : 0;
          int o_offset    = (pad.left > kernel_col) ? pad.left - kernel_col : 0;
          int left_offset = (kernel_col - pad.left >= 0 ? kernel_col - pad.left : 0);
          int right_offset =
              (kernel_hw.w - (kernel_col + 1) > pad.right ? kernel_hw.w - (kernel_col + 1) - pad.right : 0);

          int valid_len = image_hw.w - left_offset - right_offset;

          // printf("DEBUG:valid_len : %d , kernel_col : %d , offsets  :  %d , %d\n", valid_len, kernel_col,
          // left_offset, right_offset);

          float *out_ptr = dst_offset + out_row * o_image_hw.w + o_offset;
          float *in_ptr  = src_ptr + input_row * image_hw.w + i_offset;
          copy(valid_len, out_ptr, in_ptr);
#endif
        }
      }
    }
  }
}

template void im2col<float>(void *const src, void *const dst, const stride_args &stride, const padding_args &pad,
                            const image_args &image_hw, const kernel_args &kernel_hw, const image_args &o_image_hw);

void im2col_f32(void *const src, void *const dst, const stride_args &stride, const padding_args &pad,
                const image_args &image_hw, const kernel_args &kernel_hw, const image_args &o_image_hw) {
  im2col<float>(src, dst, stride, pad, image_hw, kernel_hw, o_image_hw);
}

/*
 g++ impl.cpp -c -o impl.o -march=armv8.2-a+sve
 */
