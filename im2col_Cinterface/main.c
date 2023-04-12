
#include <omp.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#ifdef __ARM_FEATURE_SVE
#include <arm_sve.h>
#endif
// bool is_a_ge_zero_and_a_lt_b(int a, int b) {
//   if ((0 <= a) && (a < b)) {
//     return true;
//   } else
//     return false;
// }

// void im2col_cpu_threads(float *data_im, int channels, int height, int width, int kernel_h, int kernel_w, int pad_h,
//                         int pad_w, int stride_h, int stride_w, int dilation_h, int dilation_w, float *data_col) {
//   const int output_h     = (height + 2 * pad_h - (dilation_h * (kernel_h - 1) + 1)) / stride_h + 1;
//   const int output_w     = (width + 2 * pad_w - (dilation_w * (kernel_w - 1) + 1)) / stride_w + 1;
//   const int channel_size = height * width;
//   // std::cout << "channels" << channels << "  ih" << height << "  iw" << width << "  kh" << kernel_h << "  kw" <<
//   // kernel_w
//   //           << "  ph" << pad_h << "  pw" << pad_w << "  sh" << stride_h << "  sw" << stride_w << "  dh" << dilation_h
//   //           << "  dw" << dilation_w << '\n';
//   int max_threads = omp_get_max_threads();
// #pragma omp parallel for num_threads(max_threads)
//   for (int channel = 0; channel < channels; ++channel) {
//     int datacol_off = channel * kernel_h * kernel_w * output_h * output_w;
//     for (int kernel_row = 0; kernel_row < kernel_h; kernel_row++) {
//       for (int kernel_col = 0; kernel_col < kernel_w; kernel_col++) {
//         int input_row = -pad_h + kernel_row * dilation_h;
//         for (int output_rows = output_h; output_rows; output_rows--) {
//           if (!is_a_ge_zero_and_a_lt_b(input_row, height)) {
//             for (int output_cols = output_w; output_cols; output_cols--) {
//               *(data_col + datacol_off++) = 0;
//             }
//           } else {
//             int input_col = -pad_w + kernel_col * dilation_w;
//             for (int output_col = output_w; output_col; output_col--) {
//               if (is_a_ge_zero_and_a_lt_b(input_col, width)) {
//                 *(data_col + datacol_off++) = *(data_im + channel_size * channel + input_row * width + input_col);
//               } else {
//                 *(data_col + datacol_off++) = 0;
//               }
//               input_col += stride_w;
//             }
//           }
//           input_row += stride_h;
//         }
//       }
//     }
//   }
// }

// void im2col_cpu(float *data_im, int channels, int height, int width, int kernel_h, int kernel_w, int pad_h, int pad_w,
//                 int stride_h, int stride_w, int dilation_h, int dilation_w, float *data_col) {
//   const int output_h     = (height + 2 * pad_h - (dilation_h * (kernel_h - 1) + 1)) / stride_h + 1;
//   const int output_w     = (width + 2 * pad_w - (dilation_w * (kernel_w - 1) + 1)) / stride_w + 1;
//   const int channel_size = height * width;
//   for (int channel = channels; channel--; data_im += channel_size) {
//     for (int kernel_row = 0; kernel_row < kernel_h; kernel_row++) {
//       for (int kernel_col = 0; kernel_col < kernel_w; kernel_col++) {
//         int input_row = -pad_h + kernel_row * dilation_h;
//         for (int output_rows = output_h; output_rows; output_rows--) {
//           if (!is_a_ge_zero_and_a_lt_b(input_row, height)) {
//             for (int output_cols = output_w; output_cols; output_cols--) {
//               *(data_col++) = 0;
//             }
//           } else {
//             int input_col = -pad_w + kernel_col * dilation_w;
//             for (int output_col = output_w; output_col; output_col--) {
//               if (is_a_ge_zero_and_a_lt_b(input_col, width)) {
//                 *(data_col++) = data_im[input_row * width + input_col];
//               } else {
//                 *(data_col++) = 0;
//               }
//               input_col += stride_w;
//             }
//           }
//           input_row += stride_h;
//         }
//       }
//     }
//   }
// }

int main(int argc, char **argv) {
#ifdef __ARM_FEATURE_SVE
  printf("sve vector has %d bits length\n", svcntb() * 8);
#endif
  srand(time(NULL));
  printf("omp has max_threads as %d\n", omp_get_max_threads());

  int image_c  = 128;
  int image_h  = 224;
  int image_w  = 224;
  int kernel_h = 3;
  int kernel_w = 3;
  if (argc > 1) {
    image_c  = atoi(argv[1]);
    image_h  = atoi(argv[2]);
    image_w  = atoi(argv[3]);
    kernel_h = atoi(argv[4]);
    kernel_w = atoi(argv[5]);
  }
}
