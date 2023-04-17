/**
 * @Author       : Realtyxxx
 * @version      : 1.0
 * @Date         : 2022-12-05 13:37:23
 * @LastEditors  : Realtyxxx
 * @LastEditTime : 2022-12-06 17:58:26
 * @FilePath     : /sve_pooling/test_pooling_generic/main.cc
 * @ToDo         :
 */

#include <omp.h>

#include <assert.h>
#include <memory.h>
#include <sys/time.h>
#include <time.h>
// #include <chrono>
#include <iostream>
#include <random>
#include "naive_nhwc_depthfirst_generic_max_pooling.h"

#include "pooling_generic.h"

int N = 1;
int H = 224;
int W = 224;
int C = 128;
int h = 3;
int w = 3;

#define stride_row 2
#define stride_col 2

#define pad_left 0
#define pad_right 0
#define pad_bottom 0
#define pad_up 0

static struct timeval start;
static struct timeval end;

/* helper function */
int    tic(void);
double toc(void);
void   random_initialize(float *const in, int size);
void   copy_initialize(float *const out, float *const in, int size);
void   print(float *in, int size, int assign = 0);

/* main func */
int main(const int argc, char **argv) {
  if (argc >= 7) {
    N = atoi(argv[1]);
    H = atoi(argv[2]);
    W = atoi(argv[3]);
    C = atoi(argv[4]);
    h = atoi(argv[5]);
    w = atoi(argv[6]);
  }
  const uint64_t     n_batches  = N;
  const uint64_t     input_size = H * W * C;
  const unsigned int n_channels = C;

  unsigned int input_rows = H;
  unsigned int input_cols = W;

  /* create Pooling execute object */
  pooling::PoolingType   type = PoolingType::MAX;
  pooling::PoolingWindow window;
  window.rows = h;
  window.cols = w;
  pooling::PoolingStride stride;
  stride.rows = stride_row;
  stride.cols = stride_col;
  pooling::PaddingValues pad;
  pad.bottom = pad_bottom;
  pad.top    = pad_up;
  pad.right  = pad_right;
  pad.left   = pad_left;

  const uint64_t output1rows = (input_rows - h + pad_up + pad_bottom) / stride_row + 1;
  const uint64_t output1cols = (input_cols - w + pad_left + pad_right) / stride_col + 1;

  assert(output1rows < UINT64_MAX / output1cols);
  const uint64_t output1size = output1rows * output1cols * n_channels;


  float *const raw_input  = (float *)malloc(sizeof(float) * (n_batches)*input_size);
  float *const raw_input1 = (float *)malloc(sizeof(float) * (n_batches)*input_size);
  float *const raw_input2 = (float *)malloc(sizeof(float) * (n_batches)*input_size);
  float *      output     = (float *)malloc(sizeof(float) * n_batches * output1size);
  float *      output1    = (float *)malloc(sizeof(float) * n_batches * output1size);
  float *      output2    = (float *)malloc(sizeof(float) * n_batches * output1size);

  random_initialize(raw_input, n_batches * input_size);
  copy_initialize((float *)raw_input1, (float *)raw_input, n_batches * input_size);
  copy_initialize((float *)raw_input2, (float *)raw_input, n_batches * input_size);
  memset(output, 0, n_batches * output1size);
  memset(output1, 0, n_batches * output1size);
  memset(output2, 0, n_batches * output1size);

  PoolingArgs                 args(type, window, stride, false, n_batches, input_rows, input_cols, n_channels, output1rows, output1cols, pad, nullptr);
  PoolingCommon<float, float> pooling(args);

  double t1 = 0.,
         t2 = 0.,
         t3 = 0.;

  int max_threads = omp_get_max_threads(), id;
  printf("max threads : %d\n", max_threads);

  tic();
#pragma omp parallel num_threads(max_threads) private(id)
  {
    id = omp_get_thread_num();
    pooling.execute(raw_input, output, nullptr, id, max_threads);  // only one thread
  }
  t1 = toc();

  tic();
  {
    pooling.execute(raw_input1, output1, nullptr, 0, 1);  // only one thread
  }
  t2 = toc();

  tic();
  naive_nhwc_depthfirst_generic_max_pooling<float>(raw_input2, output2, n_batches, n_channels, input_rows, input_cols, stride_row, stride_col, h, w);
  t3 = toc();

  std::cout << "SVE ASM MULTI THREADS: " << t1 * 1000.f << "ms"<< std::endl;
  std::cout << "SVE ASM              : " << t2 * 1000.f<< "ms"<< std::endl;
  std::cout << "C                    : " << t3 * 1000.f<< "ms"<< std::endl;

  uint64_t err_cnt1 = 0, err_cnt2 = 0;
  for (int i = 0; i < n_batches * output1size; ++i) {
    if (output[i] != output2[i]) {
      ++err_cnt1;
    }
    if (output1[i] != output2[i]) {
      ++err_cnt2;
    }
  }
  std::cout << "ERROR : " << err_cnt1 << std::endl;
  std::cout << "ERROR : " << err_cnt2 << std::endl;

  // print(output, output1size, output1cols);
  // print(output1, output1size, output1cols);

  free(raw_input);
  free(raw_input1);
  free(output);
  free(output1);
}

int tic(void) { return gettimeofday(&start, NULL); }

double toc(void) {
  gettimeofday(&end, NULL);
  return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

void random_initialize(float *const in, int size) {
  std::default_random_engine      e(1997);
  std::normal_distribution<float> engine(0, 20);
  for (int i = 0; i < size; ++i) in[i] = engine(e);
  // for (int i = 0; i < size; ++i) in[i] = i % 12;
}

void copy_initialize(float *const out, float *const in, int size) {
  for (int i = 0; i < size; ++i) out[i] = in[i];
}

void print(float *in, int size, int assign) {
  for (int i = 0; i < size; ++i) {
    if (assign != 0 && i % assign == 0) std::cout << std::endl;
    std::cout << in[i] << " ";
  }
  std::cout << std::endl;
}
