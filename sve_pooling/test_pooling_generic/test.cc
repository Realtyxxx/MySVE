/**
 * @Author       : Realtyxxx
 * @version      : 1.0
 * @Date         : 2022-12-06 00:45:56
 * @LastEditors  : Realtyxxx
 * @LastEditTime : 2022-12-06 00:45:57
 * @FilePath     : /sve_pooling/test_pooling_generic/test.cc
 * @ToDo         : 
 */
/**
 * @Author       : Realtyxxx
 * @version      : 1.0
 * @Date         : 2022-12-05 13:37:23
 * @LastEditors  : Realtyxxx
 * @LastEditTime : 2022-12-06 00:42:14
 * @FilePath     : /sve_pooling/test_pooling_generic/main.cc
 * @ToDo         : 
 */

#include <assert.h>
#include <memory.h>
#include <sys/time.h>
#include <time.h>
// #include <chrono>
#include <iostream>
#include <random>
#include "naive_nhwc_depthfirst_generic_max_pooling.h"
#include "pooling_generic.h"

#define NHWC

#if defined(NHWC)
#define N 1
#define H 33
#define W 33
#define C 32
#endif

#define h 33
#define w 33
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

#if defined(NHWC)
  assert(H < UINT64_MAX / W);
  assert(H * W < UINT64_MAX / C);
  assert(H * W * C < UINT64_MAX / N);
  const uint64_t     n_batches  = N;
  const uint64_t     input_size = H * W * C;
  const unsigned int n_channels = C;
#else
  if (argc < 5) {
    printf("error!\nusage : ./test N H W C\n");
    return -1;
  }
  const int N = atoi(argv[1]);
  const int H = atoi(argv[2]);
  const int W = atoi(argv[3]);
  const int C = atoi(argv[4]);

  const uint64_t     n_batches  = N;
  const uint64_t     input_size = H * W * C;
  const unsigned int n_channels = C;
#endif

  /* initialize */
  // float *const raw_input  = new float[n_batches * input_size];
  // float *const raw_input_ = new float[n_batches * input_size];

  /* create Pooling execute object */
  unsigned int           input_rows = H;
  unsigned int           input_cols = W;
  pooling::PoolingType   type       = PoolingType::MAX;
  pooling::PoolingWindow window;
  window.rows = h;
  window.cols = w;
  pooling::PoolingStride stride;
  stride.rows = stride_row;
  stride.cols = stride_col;
  pooling::PaddingValues pad;
  pad.bottom = pad_bottom;
  pad.left   = pad_left;
  pad.right  = pad_right;
  pad.left   = pad_left;

  const uint64_t output_rows = (input_rows - window.rows + pad_up + pad_bottom) / stride_row + 1;
  const uint64_t output_cols = (input_cols - window.cols + pad_left + pad_right) / stride_col + 1;

  assert(output_rows < UINT64_MAX / output_cols);
  const uint64_t output_size = output_rows * output_cols;

  // float *const raw_input  = (float *)alloca(sizeof(float) * (n_batches + 1) * input_size);
  // float *const raw_input_ = (float *)alloca(sizeof(float) * (n_batches + 1) * input_size);
  // float *      output     = (float *)alloca(sizeof(float) * n_batches * output_size);
  // float *      output_    = (float *)alloca(sizeof(float) * n_batches * output_size);

  float *const raw_input  = (float *)malloc(sizeof(float) * (n_batches + 1) * input_size);
  float *const raw_input_ = (float *)malloc(sizeof(float) * (n_batches + 1) * input_size);
  float *      output     = (float *)malloc(sizeof(float) * n_batches * output_size);
  float *      output_    = (float *)malloc(sizeof(float) * n_batches * output_size);

  random_initialize(raw_input, n_batches * input_size);
  copy_initialize((float *)raw_input_, (float *)raw_input, n_batches * input_size);
  memset(output, 0, n_batches * output_size);
  memset(output_, 0, n_batches * output_size);

  PoolingArgs                 args(type, window, stride, false, n_batches, input_rows, input_cols, n_channels, output_rows, output_cols, pad, nullptr);
  PoolingCommon<float, float> pooling(args);

  double t1 = 0.,
         t2 = 0.;
  tic();
  naive_nhwc_depthfirst_generic_max_pooling<float>(raw_input_, output_, n_batches, n_channels, input_rows, input_cols, stride.rows, stride.cols, window.rows, window.cols, pad.top, pad.bottom, pad.left, pad.right);
  t1 = toc();

  std::cout << "C       : " << t1 << std::endl;

  tic();
  // pooling.execute(raw_input, output, nullptr, 0, 1);  //only one thread
  t2 = toc();

  std::cout << "SVE ASM : " << t2 << std::endl;

  uint64_t err_cnt = 0;
  for (int i = 0; i < n_batches * output_size; ++i) {
    if (output[i] != output_[i]) {
      ++err_cnt;
    }
  }

  std::cout << "ERROR : " << err_cnt << std::endl;

  free(raw_input);
  free(raw_input);
  free(output);
  free(output_);
  // delete[] raw_input;
  // delete[] raw_input_;
  // delete[] output;
  // delete[] output_;
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
