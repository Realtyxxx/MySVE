#include <arm_sve.h>
#include <float.h>
#include <cmath>
#include <iostream>
#include <random>

#include <chrono>

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

void naive(float* src, float* dst, int length) {
  for (size_t i = 0; i < length; i++) {
    dst[i] = 1 / sqrt(src[i]);
  }
}

void sve(float* src, float* dst, int length) {
  int               x       = 0;
  svbool_t          pg      = svwhilelt_b32(x, length);
  const svfloat32_t const_1 = svdup_n_f32(1.f);
  do {
    // Calculate denominator
    float* input_ptr  = src + x;
    float* output_ptr = dst + x;

    const svfloat32_t tmp = svld1_f32(pg, input_ptr);
#if 1
    svfloat32_t sqrt_vec    = svsqrt_f32_z(pg, tmp);
    svfloat32_t denominator = svdiv_f32_z(pg, const_1, sqrt_vec);
#else
    svfloat32_t denominator = svrsqrte_f32(tmp);
    denominator             = svmul_f32_z(
        pg, svrsqrts_f32(svmul_f32_z(pg, tmp, denominator), denominator),
        denominator);
    denominator = svmul_f32_z(
        pg, svrsqrts_f32(svmul_f32_z(pg, tmp, denominator), denominator),
        denominator);
#endif

    svst1_f32(pg, output_ptr, denominator);
    x += svcntw();
    pg = svwhilelt_b32(x, length);
  } while (svptest_any(svptrue_b32(), pg));
}

int main() {
  Timer naive_timer, sve_timer;
  srand(1);
  constexpr int sve_len     = 16 * 320;
  const float   float_const = 15;
  float         vec[sve_len] __attribute__((aligned(32)));
  for (size_t i = 0; i < sve_len; i++) {
    vec[i] = 1.f / float(rand() % 100) + float_const;
  }
  float vec_ans_naive[sve_len];
  float vec_ans_sve[sve_len];
  naive_timer.tic();
  for (int i = 0; i < 10000; ++i) naive(vec, vec_ans_naive, sve_len);
  naive_timer.toc();
  sve_timer.tic();
  for (int i = 0; i < 10000; ++i) sve(vec, vec_ans_naive, sve_len);
  sve_timer.toc();

  int err = 0;
  for (size_t i = 0; i < sve_len; i++) {
    if (fabs(vec_ans_naive[i] - vec_ans_sve[i]) > 1e5) {
      err++;
      std::cout << vec[i] << " : " << vec_ans_naive[i] << " : "
                << vec_ans_sve[i] << "\n";
    }
  }
  std::cout << err << std::endl;
  std::cout << "NAIVE : " << naive_timer.Elapsed()
            << "\n  SVE : " << sve_timer.Elapsed() << std::endl;
}
