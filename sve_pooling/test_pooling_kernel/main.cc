/**
 * @Author       : Realtyxxx
 * @version      : 1.0
 * @Date         : 2022-11-28 21:01:28
 * @LastEditors  : Realtyxxx
 * @LastEditTime : 2022-12-15 01:15:21
 * @FilePath     : /sve_pooling/test_pooling_kernel/main.cc
 * @ToDo         : 
 */

#include <assert.h>
#include <memory.h>
#include <sys/time.h>
#include <time.h>
#include <iostream>
#include <numeric>
#include <random>

// #define HWC

#if defined(HWC)
#define C 128
#define H 224
#define W 224
#endif

#define h 3
#define w 3

static struct timeval start;
static struct timeval end;

int tic(void) { return gettimeofday(&start, NULL); }

double toc(void) {
  gettimeofday(&end, NULL);
  return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

void sve_fp32_nhwc_max_generic_depthfirst_impl(
    const uint64_t,
    const uint64_t            n_valid_cells,
    uint64_t                  n_channels,
    const float *const *const inptrs,
    float *                   outptr,
    void *);

void naive_fp32_nhwc_max_generic_depthfirst_impl(
    const uint64_t,
    const uint64_t            n_valid_cells,
    uint64_t                  n_channels,
    const float *const *const inptrs,
    float *                   outptr,
    void *);

void naive_fp32_nhwc_max_generic_depthfirst_impl1(
    const uint64_t,
    const uint64_t            n_valid_cells,
    uint64_t                  n_channels,
    const float *const *const inptrs,
    float *                   outptr,
    void *);

void random_initialize(float *const in, int size) {
  std::default_random_engine      e(1997);
  std::normal_distribution<float> engine(0, 20);
  for (int i = 0; i < size; ++i) in[i] = engine(e);
}

void copy_initialize(float *const out, float *const in, int size) {
  for (int i = 0; i < size; ++i) out[i] = in[i];
}

void print(float *in, int size, int assign = 0) {
  for (int i = 0; i < size; ++i) {
    if (assign != 0 && i % assign == 0) std::cout << std::endl;
    std::cout << in[i] << " ";
  }
  std::cout << std::endl;
}

int main(const int argc, char **argv) {
#if defined(HWC)
  const uint64_t input_size    = H * W * C;
  const uint64_t n_valid_cells = H * W;
  const uint64_t n_channels    = C;
  const uint64_t output_size   = C;
#else
  if (argc != 4) {
    printf("error!\nusage : ./test C H W\n");
    return -1;
  }
  const int      C             = atoi(argv[1]);
  const int      H             = atoi(argv[2]);
  const int      W             = atoi(argv[3]);
  const uint64_t input_size    = H * W * C;
  const uint64_t n_valid_cells = H * W;
  const uint64_t n_channels    = C;
  const uint64_t output_size   = C;
#endif

  /* initialize */
  float *const raw_input  = new float[input_size];
  float *const raw_input1 = new float[input_size];
  float *const raw_input2 = new float[input_size];

  random_initialize(raw_input, input_size);
  copy_initialize((float *)raw_input1, (float *)raw_input, input_size);
  copy_initialize((float *)raw_input2, (float *)raw_input, input_size);

  float **input  = new float *[n_valid_cells];
  float **input1 = new float *[n_valid_cells];
  float **input2 = new float *[n_valid_cells];
  float * input_ptr;

  input_ptr = raw_input;
  for (int i = 0; i < n_valid_cells; ++i, input_ptr += n_channels) {
    input[i] = input_ptr;
  }
  input_ptr = raw_input1;
  for (int i = 0; i < n_valid_cells; ++i, input_ptr += n_channels) {
    input1[i] = input_ptr;
  }

  input_ptr = raw_input2;
  for (int i = 0; i < n_valid_cells; ++i, input_ptr += n_channels) {
    input2[i] = input_ptr;
  }

  float *output  = new float[output_size];
  float *output1 = new float[output_size];
  float *output2 = new float[output_size];

  memset(output, 0, sizeof(float) * output_size);
  memset(output1, 0, sizeof(float) * output_size);
  memset(output2, 0, sizeof(float) * output_size);

  double t1 = 0., t2 = 0., t3 = 0.;
  tic();
  naive_fp32_nhwc_max_generic_depthfirst_impl(0, n_valid_cells, n_channels, (float **)input, output, nullptr);
  t1 = toc();

  tic();
  naive_fp32_nhwc_max_generic_depthfirst_impl1(0, n_valid_cells, n_channels, (float **)input1, output1, nullptr);
  t2 = toc();

  tic();
  sve_fp32_nhwc_max_generic_depthfirst_impl(0, n_valid_cells, n_channels, (float **)input2, output2, nullptr);
  t3 = toc();

  std::cout << "C       : " << t1 << std::endl
            << "C-OPT   : " << t2 << std::endl
            << "SVE ASM : " << t3 << std::endl;

  uint64_t err_cnt1 = 0, err_cnt2 = 0, err_cnt3 = 0;
  for (int i = 0; i < output_size; ++i) {
    if (output[i] != output1[i]) ++err_cnt1;
    if (output[i] != output2[i]) ++err_cnt2;
    if (output1[i] != output2[i]) ++err_cnt3;
  }

  std::cout << "ERROR(C     | C OPT)  : " << err_cnt1 << std::endl;
  std::cout << "ERROR(C     | SVE)    : " << err_cnt2 << std::endl;
  std::cout << "ERROR(C OPT | SVE)    : " << err_cnt3 << std::endl;

  if (err_cnt1 || err_cnt2 || err_cnt3) {
    // 输出第一个channel 各个求解结果
    float tmp = std::numeric_limits<float>::min();
    for (int i = 0; i < n_valid_cells; ++i) {
      std::cout << raw_input[i * n_channels] << " ";
      tmp = std::max<float>(tmp, raw_input[i * n_channels]);
    }
    std::cout << std::endl
              << tmp << std::endl;
    std::cout << output[0] << std::endl;
    std::cout << output1[0] << std::endl;
    std::cout << output2[0] << std::endl;
  }

  delete[] output;
  delete[] output1;
  delete[] output2;
  delete[] raw_input;
  delete[] raw_input1;
  delete[] raw_input2;
  delete[] input;
  delete[] input1;
  delete[] input2;
}
