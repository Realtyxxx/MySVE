#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>

/* version 1 */
void naive_fp32_nhwc_max_generic_depthfirst_impl(  // TYX_NOTES: pooling op2kerneli generic depthfirst impl max
    const uint64_t,
    const uint64_t            n_valid_cores,  //
    uint64_t                  n_channels,     //* channel num
    const float *const *const inptrs,         //? judge inptrs is nhw*,  *inptrs is nhwc*, so inptrs + 1 means next nhw*
    float *                   outptr,
    void *) {

  for (int j = 0; j < n_channels; ++j) {
    float tmp = std::numeric_limits<float>::min();
    for (int i = 0; i < n_valid_cores; ++i) {
      tmp = std::max<float>(tmp, inptrs[i][j]);
    }
    outptr[j] = tmp;
  }
}

/* version 2 */
void naive_fp32_nhwc_max_generic_depthfirst_impl1(  // TYX_NOTES: pooling op2kerneli generic depthfirst impl max
    const uint64_t,
    const uint64_t            n_valid_cores,  //
    uint64_t                  n_channels,     //* channel num
    const float *const *const inptrs,         //? judge inptrs is nhw*,  *inptrs is nhwc*, so inptrs + 1 means next nhw*
    float *                   outptr,
    void *) {

  const int sve_len = 2048 / (sizeof(float) * 8);  // 64
  // const int sve_len  = 512 / 32;
  const int sve_len4 = 4 * sve_len;

  int n_iter = n_valid_cores >> 2;  // 四个四个像素点的求
  // int n_left       = n_valid_cores & 0x3; // 末尾处理

  int c_iter_begin = 0;
  int c_iter_end   = std::min<float>(n_channels, c_iter_begin + sve_len4);
  while (c_iter_begin < n_channels) {
    for (int i = c_iter_begin; i < c_iter_end; ++i) {
      float tmp = std::numeric_limits<float>::min();
      for (int j = 0; j < n_iter; ++j) { /* 4 次循环展开在 n_valid_cores */
        tmp = std::max<float>({inptrs[4 * j][i],
                               inptrs[4 * j + 1][i],
                               inptrs[4 * j + 2][i],
                               inptrs[4 * j + 3][i], tmp});
      }
      for (int j = n_iter << 2; j < n_valid_cores; ++j) {
        tmp = std::max<float>(tmp, inptrs[j][i]);
      }
      outptr[i] = tmp;
    }
    // if (c_iter_end == n_channels) break;
    c_iter_begin = std::min<float>(n_channels, c_iter_begin + sve_len4);
    c_iter_end   = std::min<float>(n_channels, c_iter_end + sve_len4);  // 模拟向量处理，每次4倍sve长度处理
  }
}
