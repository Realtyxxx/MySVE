#if defined(__ARM_FEATURE_SVE)
#include <arm_sve.h>
#pragma message("using sve")
#else
#warning "not using sve"
#endif

#include <iostream>
#include "args.h"

namespace {
bool is_a_ge_zero_and_a_lt_b(int a, int b) {
  if ((0 <= a) && (a < b)) {
    return true;
  } else
    return false;
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

  dtype *src_ptr = (dtype *)src;
  dtype *dst_ptr = (dtype *)dst;
  for (int ic = image_hw.ic; ic--; src_ptr += channel_size, dst_ptr += o_channel_size) {  // big image loop

    // iteration by kernel pos
    for (int kernel_row = 0; kernel_row < kernel_hw.w; ++kernel_row) {
      for (int kernel_col = 0; kernel_col < kernel_hw.h; ++kernel_col) {

        int input_row = -pad.up + kernel_row;
        for (int out_row = 0; out_row < o_image_hw.h; ++out_row, input_row += stride.h) {
          if (input_row < 0 || input_row >= image_hw.h) {
            continue;
          }
          float *dst_offset = dst_ptr + (kernel_row * kernel_hw.w + kernel_col) * (o_image_hw.w * o_image_hw.h);
          // * here could be optimized
          // #if 1
#ifndef __ARM_FEATURE_SVE
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
          const auto all_true_pg = svptrue_b32();
          int        i_offset    = (kernel_col > pad.left) ? kernel_col - pad.left : 0;
          int        o_offset    = (pad.left > kernel_col) ? pad.left - kernel_col : 0;
          int        left_offset = (kernel_col - pad.left >= 0 ? kernel_col - pad.left : 0);
          int        right_offset =
              (kernel_hw.w - (kernel_col + 1) > pad.right ? kernel_hw.w - (kernel_col + 1) - pad.right : 0);

          int valid_len = image_hw.w - left_offset - right_offset;

          // printf("DEBUG: kernel_col : %d , offsets  :  %d , %d\n", kernel_col, left_offset, right_offset);

          int              x        = 0;
          static const int vec_len  = svcntw();
          static const int vec_len2 = 2 * vec_len;
          static const int vec_len3 = 3 * vec_len;
          static const int vec_len4 = 4 * vec_len;

          // printf("DEBUG: kernel_col : %d , valid_len : %d , vec_len : %d\n", kernel_col, valid_len, vec_len);

          svbool_t         pg0      = svwhilelt_b32(x, valid_len);
          svbool_t         pg1      = svwhilelt_b32(x + vec_len, valid_len);
          svbool_t         pg2      = svwhilelt_b32(x + vec_len * 2, valid_len);
          svbool_t         pg3      = svwhilelt_b32(x + vec_len * 3, valid_len);

          float *out_ptr = dst_offset + out_row * o_image_hw.w + o_offset;
          float *in_ptr  = src_ptr + input_row * image_hw.w + i_offset;
          do {
            svst1_f32(pg0, out_ptr, svld1(pg0, in_ptr));
            svst1_f32(pg1, out_ptr + vec_len, svld1(pg1, in_ptr + vec_len));
            svst1_f32(pg2, out_ptr + vec_len2, svld1(pg2, in_ptr + vec_len2));
            svst1_f32(pg3, out_ptr + vec_len3, svld1(pg3, in_ptr + vec_len3));
            x += vec_len4;
            pg0 = svwhilelt_b32(x, valid_len);
            pg1 = svwhilelt_b32(x + vec_len, valid_len);
            pg2 = svwhilelt_b32(x + vec_len * 2, valid_len);
            pg3 = svwhilelt_b32(x + vec_len * 3, valid_len);
            out_ptr += vec_len4;
            in_ptr += vec_len4;
          } while (svptest_any(pg0, all_true_pg));
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
