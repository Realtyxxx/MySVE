#include <omp.h>
#if defined(__ARM_FEATURE_SVE)
#include <arm_sve.h>
#pragma message("using sve")
#else
#warning "not using sve"
#endif

#include <stdio.h>
#include <string.h>
#include "gemm.h"

// inline void copy(int valid_len, float *out_ptr, float *in_ptr) {
// const svbool_t all_true_pg = svptrue_b32();

// int       x        = 0;
// const int vec_len  = svcntw();
// const int vec_len2 = 2 * vec_len;
// const int vec_len3 = 3 * vec_len;
// const int vec_len4 = 4 * vec_len;
// svbool_t  pg0      = svwhilelt_b32(x, valid_len);
// svbool_t  pg1      = svwhilelt_b32(x + vec_len, valid_len);
// svbool_t  pg2      = svwhilelt_b32(x + vec_len * 2, valid_len);
// svbool_t  pg3      = svwhilelt_b32(x + vec_len * 3, valid_len);

// do {
//   svst1_f32(pg0, out_ptr, svld1(pg0, in_ptr));
//   svst1_f32(pg1, out_ptr + vec_len, svld1(pg1, in_ptr + vec_len));
//   svst1_f32(pg2, out_ptr + vec_len2, svld1(pg2, in_ptr + vec_len2));
//   svst1_f32(pg3, out_ptr + vec_len3, svld1(pg3, in_ptr + vec_len3));
//   x += vec_len4;
//   pg0 = svwhilelt_b32(x, valid_len);
//   pg1 = svwhilelt_b32(x + vec_len, valid_len);
//   pg2 = svwhilelt_b32(x + vec_len * 2, valid_len);
//   pg3 = svwhilelt_b32(x + vec_len * 3, valid_len);

//   out_ptr += vec_len4;
//   in_ptr += vec_len4;
// } while (svptest_any(pg0, all_true_pg));
// }

void im2col(float *src, float *dst, const int stride, const int pad, const int channel, const int height, const int width, const int kernel_height, const int kernel_width, const int out_height,
            const int out_width) {
  // stride = 1 then it could vectorize
  int channel_size   = width * height;
  int o_channel_size = out_width * out_height * kernel_height * kernel_width;
  memset(dst, 0, out_height * out_width * sizeof(float));

  float *src_ptr = (float *)src;
  float *dst_ptr = (float *)dst;
  for (int ic = channel; ic--; src_ptr += channel_size, dst_ptr += o_channel_size) {  // big image loop

    int kernel_total;
    int kernel_row, kernel_col;
    // iteration by kernel pos
    int max_threads = omp_get_max_threads();
#pragma omp parallel for num_threads(max_threads) private(kernel_row, kernel_col)
    for (kernel_total = 0; kernel_total < kernel_width * kernel_height; ++kernel_total) {
      kernel_row = kernel_total / kernel_width;
      kernel_col = kernel_total % kernel_width;
      {

        int input_row = -pad + kernel_row;
        for (int out_row = 0; out_row < out_height; ++out_row, input_row += stride) {
          if (input_row < 0 || input_row >= height) {
            continue;
          }
          float *dst_offset = dst_ptr + (kernel_row * kernel_width + kernel_col) * (out_width * out_height);
          // * here could be optimized
#ifndef __ARM_FEATURE_SVE
          int input_col = -pad + kernel_col;
          for (int out_col = 0; out_col < out_width; ++out_col, input_col += stride) {
            if (input_col < 0 || input_col >= width) {
              continue;
            }
            dst_offset[out_row * out_width + out_col] = src_ptr[input_row * width + input_col];
          }
#else
          // vectorize at the input col iteration

          int i_offset = (kernel_col > pad) ? kernel_col - pad : 0;
          int o_offset = (pad > kernel_col) ? pad - kernel_col : 0;

          int left_offset  = (kernel_col > pad) ? kernel_col - pad : 0;
          int right_offset = (kernel_width - (kernel_col + 1) > pad ? kernel_width - (kernel_col + 1) - pad : 0);

          int    valid_len = width - left_offset - right_offset;
          float *out_ptr   = dst_offset + out_row * out_width + o_offset;
          float *in_ptr    = src_ptr + input_row * width + i_offset;

          // printf("DEBUG: kernel_col : %d , offsets  :  %d , %d\n", kernel_col, left_offset, right_offset);
          // copy(valid_len, out_ptr, in_ptr);
          const svbool_t all_true_pg = svptrue_b32();

          int       x        = 0;
          const int vec_len  = svcntw();
          const int vec_len2 = 2 * vec_len;
          const int vec_len3 = 3 * vec_len;
          const int vec_len4 = 4 * vec_len;
          svbool_t  pg0      = svwhilelt_b32(x, valid_len);
          svbool_t  pg1      = svwhilelt_b32(x + vec_len, valid_len);
          svbool_t  pg2      = svwhilelt_b32(x + vec_len * 2, valid_len);
          svbool_t  pg3      = svwhilelt_b32(x + vec_len * 3, valid_len);

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

void conv_im2col_gemm_f32(float *src, float *weights, float *dst, int batches, int channel, int height, int width, int out_channel, int kernel_height, int kernel_width, int stride, int pad) {
  int    out_height  = (height - kernel_height + pad * 2) / stride + 1;
  int    out_width   = (width - kernel_width + pad * 2) / stride + 1;
  float *trans_image = (float *)malloc(out_height * out_width * channel * kernel_height * kernel_width * sizeof(float));
  im2col(src, trans_image, stride, pad, channel, height, width, kernel_height, kernel_width, out_height, out_width);
  int M = out_height * out_width;
  int N = out_channel;
  int K = channel * kernel_height * kernel_width;
  printf("M = %d, N = %d, K = %d\n", M, N, K);
  sgemm(M, N, K, 1, trans_image, M, weights, K, 0, dst, M);
  free(trans_image);
}
