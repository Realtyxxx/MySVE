#include <stdint.h>
#include <string.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include "float.h"
#ifdef __ARM_FEATURE_SVE
#include <arm_sve.h>
#endif
#include "align_malloc.hpp"
#include "soft_max.hpp"

#define CHECK(condition) _CHECK(condition, #condition)
#define _CHECK(con, msg)              \
  if (!(con)) {                       \
    printf("check %s failed\n", msg); \
    exit(1);                          \
  }

class deleter {
 public:
  void operator()(float *ptr) { delete[] ptr; }
};
class Timer {
  using Clock = std::chrono::high_resolution_clock;

 public:
  inline void tic() { start_ = Clock::now(); }

  inline void toc() { end_ = Clock::now(); }

  inline double Elapsed() {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_);
    return duration.count();
  }

 private:
  Clock::time_point start_, end_;
};

void randomInit(float *data, int size) {
  long i;
  for (i = 0; i < size; ++i) {
    data[i] = ((float)rand() / (float)RAND_MAX);
  }
}

void ref_softmax_f32(float *input, float *output, int workNum, int workSize, float beta) {
  float *input_data  = (float *)input;
  float *output_data = (float *)output;

  int64_t outer_size = workNum;
  int     cnt        = workSize;

  for (int i = 0; i < outer_size; i++) {
    float acc_exp = 0.0f;
    float max     = -FLT_MAX;
    // Find max element value which we'll use to ensure numerical stability
    // taking advantage of the following equality:
    // exp(x[i])/sum(exp(x[i])) == exp(x[i]+C)/sum(exp(x[i]+C))
    for (int j = 0; j < cnt; j++) {
      max = fmax(max, *(input_data + j));
    }

    // compute sum
    for (int j = 0; j < cnt; j++) {
      acc_exp += exp(beta * (*(input_data + j) - max));
    }

    // compute final result
    for (int j = 0; j < cnt; j++) {
      *(output_data + j) = exp(beta * (*(input_data + j) - max)) / acc_exp;
    }
    input_data += cnt;
    output_data += cnt;
  }
}

bool check_result(const float *a, const float *b, int size) {
  int   error   = 0;
  float err_sum = 0.f;
  float eps     = 1e-5;
  for (int i = 0; i < size; ++i) {
    if (fabs(a[i] - b[i]) > eps) {
      error++;
    }
    err_sum += fabs(a[i] - b[i]);
  }
  if (error != 0) {
    std::cout << "error count : " << error << "error sum : " << err_sum << std::endl;
    return false;
  } else {
    std::cout << "passed CPU SVE test" << std::endl;
    return true;
  }
}

int main(int argc, char **argv) {
#ifdef __ARM_FEATURE_SVE
  printf("sve vector has %d bits length\n", svcntb() * 8);
#endif
  srand(time(NULL));

  int workNum  = 3;
  int workSize = 1000;
  if (argc > 1) {
    CHECK(argc == 3);
    workNum  = atoi(argv[1]);
    workSize = atoi(argv[2]);
  }
  float *src       = malloc_aligned(workSize, workNum, sizeof(float));
  float *dst_sve   = malloc_aligned(workSize, workNum, sizeof(float));
  float *dst_naive = malloc_aligned(workSize, workNum, sizeof(float));

  randomInit(src, workSize * workNum);

  float beta = 1.f;
  // init args
  Timer t1, t2;

  t1.tic();
  softmax<false>(src, dst_sve, workNum, workSize, beta);
  t1.toc();

  printf("sve ends\n");
  t2.tic();
  ref_softmax_f32(src, dst_naive, workNum, workSize, beta);
  t2.toc();

  std::cout << "my    : " << t1.Elapsed() << std::endl;
  std::cout << "csinn : " << t2.Elapsed() << std::endl;

  check_result(dst_sve, dst_naive, workNum * workSize);
  for (int i = 0; i < 15; ++i) {
      std::cout << dst_sve[i] << " " << dst_naive[i] << "\n";
    }
  }
