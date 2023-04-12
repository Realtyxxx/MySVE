
#include <arm_sve.h>
#include <omp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define TIME_START(num)                      \
  struct timeval tv_start##num, tv_end##num; \
  gettimeofday(&tv_start##num, NULL);

#define TIME_END(num, tag)                                \
  gettimeofday(&tv_end##num, NULL);                       \
  printf(                                                 \
      "[%s-%d]:%s Cost=%fms\n", __func__, __LINE__, #tag, \
      ((tv_end##num.tv_sec - tv_start##num.tv_sec) * 1e6 + (tv_end##num.tv_usec - tv_start##num.tv_usec)) / 1000.0f);

void conv_im2col_gemm_f32(float *src, float *weights, float *dst, int batches, int channel, int height, int width,
                          int out_channel, int kernel_height, int kernel_width, int stride, int pad);

void randomInit(float *data, int size) {
  long i;
  for (i = 0; i < size; ++i) {
    data[i] = ((float)rand() / (float)RAND_MAX);
  }
}

int main(int argc, char **argv) {
#ifdef __ARM_FEATURE_SVE
  printf("sve vector has %d bits length\n", svcntb() * 8);
#endif
  srand(time(NULL));
  printf("omp has max_threads as %d\n", omp_get_max_threads());

  int c      = 128;
  int h      = 224;
  int w      = 224;
  int kh     = 3;
  int kw     = 3;
  int oc     = 2;
  int pad    = 1;
  int stride = 1;
  if (argc > 1) {
    c      = atoi(argv[1]);
    h      = atoi(argv[2]);
    w      = atoi(argv[3]);
    oc     = atoi(argv[4]);
    kh     = atoi(argv[5]);
    kw     = atoi(argv[6]);
    pad    = atoi(argv[7]);
    stride = atoi(argv[8]);
  }

  int    input_size   = c * h * w;
  int    weights_size = oc * kw * kh;
  float *input        = (float *)malloc(sizeof(float) * input_size);
  float *weights      = (float *)malloc(sizeof(float) * weights_size);

  randomInit(input, input_size);
  randomInit(weights, weights_size);

  int out_height  = (h - kh + pad * 2) / stride + 1;
  int out_width   = (w - kw + pad * 2) / stride + 1;
  int output_size = out_height * out_width;

  float *output = (float *)malloc(sizeof(float) * output_size);
  TIME_START(conv);
  conv_im2col_gemm_f32(input, weights, output, 1, c, h, w, oc, kh, kw, stride, pad);
  TIME_END(conv, conv);
  return 0;
}
