#include <arm_sve.h>
#include <float.h>
#include <chrono>
#include <cmath>
#include <iostream>
#include <random>
#include "batch_norm_sve.hpp"

class Timer {
  using Clock = std::chrono::high_resolution_clock;

 public:
  inline void tic() { start_ = Clock::now(); }

  inline void toc() { end_ = Clock::now(); }

  inline double Elapsed() {
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_);
    return duration.count();
  }

 private:
  Clock::time_point start_, end_;
};

float* malloc_aligned(int m, int n, int size) {
  float* ptr;
  int    err;

  err = posix_memalign((void**)&ptr, (size_t)32, size * m * n);

  if (err) {
    printf("bl_malloc_aligned(): posix_memalign() failures");
    exit(1);
  }

  return ptr;
}

void initRandom(float* src, int num) {
  for (int i = 0; i < num; ++i) {
    src[i] = rand() * 1.f / static_cast<float>(RAND_MAX);
  }
}

void initConstant(float* src, int num, float val) {
  for (int i = 0; i < num; ++i) {
    src[i] = val;
  }
}

int main(int argc, char** argv) {
  Timer naive_timer, sve_timer;
  srand(time(NULL));
  if (argc != 3) {
    printf("./func cells, C\n");
    return 0;
  }
  int         cells       = atoi(argv[1]);
  int         C           = atoi(argv[2]);
  int         len         = cells * C;
  const float float_const = 15;
  float       vec[len] __attribute__((aligned(32)));
  int         ndim[2] = {cells, C};

  float naive_mean[C];
  float naive_var[C];
  float naive_out[len];

  float sve_mean[C];
  float sve_var[C];
  float sve_out[len];

  float beta[C];

  initConstant(beta, C, 1.f);
  // initConstant(beta, C, 0.f);
  // initConstant(beta, C, 0.f);

  naive_timer.tic();
  for (int i = 0; i < 1000; ++i) {
    get_mean_and_variance_naive(vec, naive_mean, naive_var, 2, ndim);
    shl_ref_batch_normalization_f32(vec, naive_mean, naive_var, beta, naive_out,
                                    10.f, NULL, 2, ndim);
  }
  naive_timer.toc();
  sve_timer.tic();
  for (int i = 0; i < 1000; ++i) {
    get_mean_and_variance_sve(vec, sve_mean, sve_var, 2, ndim);
    batchnorm_sve_nhwc(vec, naive_mean, naive_var, beta, sve_out, 10.f, NULL, 2,
                       ndim);
  }
  sve_timer.toc();

  int err = 0;
  // std::cout << "mean : " << std::endl;
  // for (size_t i = 0; i < C; i++) {
  //   if (fabs(naive_mean[i] - sve_mean[i]) > 1e-5) {
  //     err++;
  //     if (err < 5) std::cout << i << " : " << naive_mean[i] << " , " <<
  //     sve_mean[i] << "\n";
  //   }
  // }
  // std::cout << err << std::endl;
  // std::cout << "var : " << std::endl;
  // err = 0;
  // for (size_t i = 0; i < C; i++) {
  //   if (fabs(naive_var[i] - sve_var[i]) > 1e-5) {
  //     err++;
  //     if (err < 5) std::cout << i << " : " << naive_var[i] << " , " <<
  //     sve_var[i] << "\n";
  //   }
  // }
  // std::cout << err << std::endl;
  // std::cout << "bnorm : " << std::endl;
  err = 0;
  for (size_t i = 0; i < len; i++) {
    if (fabs(naive_out[i] - sve_out[i]) > 1e-5) {
      err++;
      if (err < 5)
        std::cout << i << " : " << naive_out[i] << " , " << sve_out[i] << "\n";
    }
  }
  std::cout << err << std::endl;
  std::cout << "NAIVE : " << naive_timer.Elapsed()
            << "\n  SVE : " << sve_timer.Elapsed() << std::endl;
}
