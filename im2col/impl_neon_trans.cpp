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
  // printf("valid_len : %d, iter16 : %d, iter4 : %d, iter_left : %d\n", valid_len, iter16, iter4, iter_left);

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

void im2col(float *data_im, int channels, int height, int width, int kernel_h, int kernel_w, int pad_h, int pad_w,
            int stride_h, int stride_w, int dilation_h, int dilation_w, float *data_col) {
  const int output_h     = (height + 2 * pad_h - (dilation_h * (kernel_h - 1) + 1)) / stride_h + 1;
  const int output_w     = (width + 2 * pad_w - (dilation_w * (kernel_w - 1) + 1)) / stride_w + 1;
  const int channel_size = height * width;
  // std::cout << "channels" << channels << "  ih" << height << "  iw" << width << "  kh" << kernel_h << "  kw" <<
  // kernel_w
  //           << "  ph" << pad_h << "  pw" << pad_w << "  sh" << stride_h << "  sw" << stride_w << "  dh" << dilation_h
  //           << "  dw" << dilation_w << '\n';
#pragma omp parallel for num_threads(16)
  for (int channel = 0; channel < channels; ++channel) {
    int datacol_off = channel * kernel_h * kernel_w * output_h * output_w;
    for (int kernel_row = 0; kernel_row < kernel_h; kernel_row++) {
      for (int kernel_col = 0; kernel_col < kernel_w; kernel_col++) {
        int input_row = -pad_h + kernel_row * dilation_h;
        for (int output_rows = output_h; output_rows; output_rows--) {
          if (!is_a_ge_zero_and_a_lt_b(input_row, height)) {
            for (int output_cols = output_w; output_cols; output_cols--) {
              *(data_col + datacol_off++) = 0;
            }
          } else {
            int input_col = -pad_w + kernel_col * dilation_w;
            for (int output_col = output_w; output_col; output_col--) {
              if (is_a_ge_zero_and_a_lt_b(input_col, width)) {
                *(data_col + datacol_off++) = *(data_im + channel_size * channel + input_row * width + input_col);
              } else {
                *(data_col + datacol_off++) = 0;
              }
              input_col += stride_w;
            }
          }
          input_row += stride_h;
        }
      }
    }
  }
}

/* @brief
 *
 * @param src
 * @param dst
 * @param stride
 * @param pad
 * @param image_hw
 * @param kernel_hw
 * @param o_image_hw 卷积结果尺寸
 */
void im2col_neon(float *src, int channels, int height, int width, int kernel_h, int kernel_w, int pad_h, int pad_w,
                 int stride_h, int stride_w, int dilation_h, int dilation_w, float *dst) {

  // void im2col(void *const src, void *const dst, const stride_args &stride, const padding_args &pad,
  //             const image_args &image_hw, const kernel_args &kernel_hw, const image_args &o_image_hw) {
  // stride = 1 then it could vectorize

  const int output_h = (height + 2 * pad_h - (dilation_h * (kernel_h - 1) + 1)) / stride_h + 1;
  const int output_w = (width + 2 * pad_w - (dilation_w * (kernel_w - 1) + 1)) / stride_w + 1;

  int channel_size   = width * height;
  int o_channel_size = output_h * output_w * kernel_h * kernel_w;
  // memset(dst, 0, output_h * output_w * sizeof(float));

  // float *src_ptr = (float *)src;
  // float *dst_ptr = (float *)dst;
#pragma omp parallel for num_threads(2)
  for (int ic = 0; ic < channels; ++ic) {  // big image loop
    // for (int ic = channels; ic--; src_ptr += channel_size, dst_ptr += o_channel_size) {  // big image loop
    float *src_ptr = ic * channel_size + src;
    float *dst_ptr = ic * o_channel_size + dst;
    int    kernel_total;
    int    kernel_row, kernel_col;

    int max_threads = 8;
// printf("omp provide %d threads\n", max_threads);
#pragma omp parallel for num_threads(max_threads) private(kernel_row, kernel_col)
    for (kernel_total = 0; kernel_total < kernel_w * kernel_h; ++kernel_total) {
      kernel_row = kernel_total / kernel_w;
      kernel_col = kernel_total % kernel_w;
      {

        int input_row = -pad_h + kernel_row;
        for (int out_row = 0; out_row < output_h; ++out_row, input_row += stride_h) {
          if (input_row < 0 || input_row >= height) {
            continue;
          }
          float *dst_offset = dst_ptr + (kernel_row * kernel_w + kernel_col) * (output_w * output_h);
          // * here could be optimized
          // #if 1
#if 0 
          int input_col = -pad_w + kernel_col;
          for (int out_col = 0; out_col < output_w; ++out_col, input_col += stride_w) {
            if (input_col < 0 || input_col >= width) {
              continue;
            }
            dst_offset[out_row * output_w + out_col] = src_ptr[input_row * width + input_col];
          }
#else
          // CHECK(stride_w == 1);
          // vectorize at the input col iteration

          int i_offset     = (kernel_col > pad_w) ? kernel_col - pad_w : 0;
          int o_offset     = (pad_w > kernel_col) ? pad_w - kernel_col : 0;
          int left_offset  = (kernel_col - pad_w >= 0 ? kernel_col - pad_w : 0);
          int right_offset = (kernel_w - (kernel_col + 1) > pad_w ? kernel_w - (kernel_col + 1) - pad_w : 0);

          int valid_len = width - left_offset - right_offset;

          // printf("DEBUG:valid_len : %d , kernel_col : %d , offsets  :  %d , %d\n", valid_len, kernel_col,
          // left_offset, right_offset);

          float *out_ptr = dst_offset + out_row * output_w + o_offset;
          float *in_ptr  = src_ptr + input_row * width + i_offset;
          copy(valid_len, out_ptr, in_ptr);
#endif
        }
      }
    }
  }
}

// template void im2col<float>(void *const src, void *const dst, const stride_args &stride, const padding_args &pad,
// const image_args &image_hw, const kernel_args &kernel_hw, const image_args &o_image_hw);

// void im2col_f32(void *const src, void *const dst, const stride_args &stride, const padding_args &pad,
//                 const image_args &image_hw, const kernel_args &kernel_hw, const image_args &o_image_hw) {
//   im2col_neon(src, image_hw.ic, image_hw.h, image_hw.w, kernel_hw.h, kernel_hw.w, pad.up, pad.left, stride.h,
//   stride.w,
//               0, 0, dst);
// }
// namespace template<typenamedtype>void
// im2col(float*src,intchannels,intheight,intwidth,intkernel_h,intkernel_w,intpad_h,intpad_w,intstride_h,intstride_w,intdilation_h,intdilation_w,float*dst)
// im2col(float*src,intchannels,intheight,intwidth,intkernel_h,intkernel_w,intpad_h,intpad_w,intstride_h,intstride_w,intdilation_h,intdilation_w,float*dst)

/*
 g++ impl.cpp -c -o impl.o -march=armv8.2-a+sve
 */
