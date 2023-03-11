/**
 * @Author       : Realtyxxx
 * @version      : 1.0
 * @Date         : 2022-12-05 16:35:34
 * @LastEditors  : Realtyxxx
 * @LastEditTime : 2022-12-06 16:24:53
 * @FilePath     : /sve_pooling/test_pooling_generic/naive_nhwc_depthfirst_generic_max_pooling.cc
 * @ToDo         :
 */

#include "alloca.h"
#include "cstdint"
template <class data_type>
void naive_nhwc_depthfirst_generic_max_pooling(const void* const input,
                                               void* const       output,
                                               unsigned int      n_batches,
                                               unsigned int      n_channels,
                                               unsigned int      input_rows,
                                               unsigned int      input_cols,
                                               unsigned int      stride_rows,
                                               unsigned int      stride_cols,
                                               unsigned int      window_rows,
                                               unsigned int      window_cols,
                                               unsigned int      pad_top,
                                               unsigned int      pad_bottom,
                                               unsigned int      pad_left,
                                               unsigned int      pad_right) {

  const uint64_t output_h = (input_rows - window_rows + pad_top + pad_bottom) / stride_rows + 1;
  const uint64_t output_w = (input_cols - window_cols + pad_left + pad_right) / stride_cols + 1;

  const uint64_t ld_output_col   = n_channels;
  const uint64_t ld_output_row   = output_w * n_channels;
  const uint64_t ld_output_batch = output_w * output_h * n_channels;

  const uint64_t ld_input_col   = n_channels;
  const uint64_t ld_input_row   = input_cols * n_channels;
  const uint64_t ld_input_batch = input_rows * input_cols * n_channels;
  const uint64_t output_length  = output_h * output_w;
  const uint64_t length         = input_rows * input_cols;

  const int H_kernel     = input_rows - window_rows;
  const int W_kernel     = input_cols - window_cols;
  const int window_range = window_rows * window_cols;

  int* offset = (int*)alloca(window_range * sizeof(int));
  {
    int pos = 0;
    for (int i = 0; i < window_rows; ++i)
      for (int j = 0; j < window_cols; ++j)
        offset[pos++] = i * ld_input_row + j * ld_input_col;
  }
  for (int b = 0; b < n_batches; ++b) {     // batch 的每一张图像对应的特征图分开池化
    for (int i = 0; i < n_channels; ++i) {  // 每个通道
      data_type*       input_b            = (data_type*)input + b * ld_input_batch;
      data_type* const cur_image_features = input_b + i;
      data_type*       output_b           = (data_type*)output + b * ld_output_batch;
      data_type* const output_ptr         = output_b + i;
      int              cnt                = 0;                             // 当前池化输出的位置
      for (int x = 0; x <= H_kernel; x += stride_rows) {                   // ? 有没有 =
        data_type* const row_ptr = cur_image_features + x * ld_input_row;  // 获取这个通道图像的第 x 行指针
        for (int y = 0; y <= W_kernel; y += stride_cols) {                 // ? 有没有 =
          // 找到局部的 kernel_size X kernel_size 的区域, 找最大值
          data_type* ker       = row_ptr + y * ld_input_col;
          data_type  max_value = *ker;
          for (int k = 1; k < window_range; ++k) {  // 从 1 开始因为 0 已经比过了, max_value = row_ptr[y]
            data_type comp = ker[offset[k]];
            if (max_value < comp) {
              max_value = comp;
            }
          }
          // 局部最大值填到输出的对应位置上
          output_ptr[cnt] = max_value;
          cnt += n_channels;
        }
      }
    }
  }
}

template void naive_nhwc_depthfirst_generic_max_pooling<float>(const void* const input,
                                                               void* const       output,
                                                               unsigned int      n_batches,
                                                               unsigned int      n_channels,
                                                               unsigned int      input_rows,
                                                               unsigned int      input_cols,
                                                               unsigned int      stride_rows,
                                                               unsigned int      stride_cols,
                                                               unsigned int      window_rows,
                                                               unsigned int      window_cols,
                                                               unsigned int      pad_top,
                                                               unsigned int      pad_bottom,
                                                               unsigned int      pad_left,
                                                               unsigned int      pad_right);

// 9 3 6   0 3 6
// 10 3 7  0 3 6
//
//
//
