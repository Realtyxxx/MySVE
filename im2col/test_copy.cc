#include <arm_sve.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include <random>

using namespace std;

inline void copy(int valid_len, float *out_ptr, float *in_ptr) {
  for (size_t i = 0; i < valid_len; i++) {
    out_ptr[i] = in_ptr[i];
  }
}

inline void sve_copy(int valid_len, float *out_ptr, float *in_ptr) {
  const auto all_true_pg = svptrue_b32();

  int              x       = 0;
  static const int vec_len = svcntw();
  // static const int vec_len2 = 2 * vec_len;
  // static const int vec_len3 = 3 * vec_len;
  // static const int vec_len4 = 4 * vec_len;

  // printf("DEBUG: kernel_col : %d , valid_len : %d , vec_len : %d\n",
  // kernel_col, valid_len, vec_len);

  svbool_t pg0 = svwhilelt_b32(x, valid_len);
  // svbool_t pg1 = svwhilelt_b32(x + vec_len, valid_len);
  // svbool_t pg2 = svwhilelt_b32(x + vec_len2, valid_len);
  // svbool_t pg3 = svwhilelt_b32(x + vec_len3, valid_len);

  do {
    svst1_f32(pg0, out_ptr, svld1(pg0, in_ptr));
    // svst1_f32(pg1, out_ptr + vec_len, svld1(pg1, in_ptr + vec_len));
    // svst1_f32(pg2, out_ptr + vec_len2, svld1(pg2, in_ptr + vec_len2));
    // svst1_f32(pg3, out_ptr + vec_len3, svld1(pg3, in_ptr + vec_len3));
    // x += vec_len4;
    x += vec_len;
    pg0 = svwhilelt_b32(x, valid_len);
    // pg1 = svwhilelt_b32(x + vec_len, valid_len);
    // pg2 = svwhilelt_b32(x + vec_len * 2, valid_len);
    // pg3 = svwhilelt_b32(x + vec_len * 3, valid_len);

    // out_ptr += vec_len4;
    // in_ptr += vec_len4;
    out_ptr += vec_len;
    in_ptr += vec_len;
  } while (svptest_any(pg0, all_true_pg));
}

// inline void neon_copy(unsigned int valid_len, float *out_ptr, float *in_ptr)
// {
//
//   int iter16    = valid_len / 16;
//   int iter4     = (valid_len - iter16 * 16) / 4;
//   int iter_left = (valid_len - iter16 * 16) % 4;
//
//   static const int vec_len  = 4;
//   static const int vec_len2 = 8;
//   static const int vec_len3 = 12;
//   static const int vec_len4 = 16;
//   // printf("valid_len : %d, iter16 : %d, iter4 : %d, iter_left : %d\n",
//   valid_len, iter16, iter4, iter_left);
//
//   int i;
//   for (i = 0; i < iter16; ++i) {
//     vst2_f32(out_ptr, vld2_f32(in_ptr));
//     vst2_f32(out_ptr + vec_len, vld2_f32(in_ptr + vec_len));
//     vst2_f32(out_ptr + vec_len2, vld2_f32(in_ptr + vec_len2));
//     vst2_f32(out_ptr + vec_len3, vld2_f32(in_ptr + vec_len3));
//     out_ptr += vec_len4;
//     in_ptr += vec_len4;
//   }
//   for (i = 0; i < iter4; ++i) {
//     vst2_f32(out_ptr, vld2_f32(in_ptr));
//     out_ptr += vec_len;
//     in_ptr += vec_len;
//   }
//   for (int i = 0; i < iter_left; ++i) {
//     *out_ptr++ = *in_ptr++;
//   }
// }

class Timer {
  using Clock = std::chrono::high_resolution_clock;

 public:
  inline void tic() { start_ = Clock::now(); }

  inline void toc() { end_ = Clock::now(); }

  inline long long Elapsed() {
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_);
    return duration.count();
  }

 private:
  Clock::time_point start_, end_;
};

void init(float *ptr, int size, int flag, float val) {
  switch (flag) {
    case 0:
      for (int i = 0; i < size; ++i) {
        ptr[i] = val;
      }
      break;
    case 1:
      for (int i = 0; i < size; ++i) {
        ptr[i] = val++;
      }
      break;
    default: {
      static auto getVal = []() { return rand() / 1000.f; };
      for (int i = 0; i < size; ++i) {
        ptr[i] = getVal();
      }
    }
  }
}

void check(float *x, float *y, int size) {
  float error_sum   = 0;
  int   error_count = 0;
  for (int i = 0; i < size; ++i) {
    if (fabs(x[i] - y[i]) > 1e-5) {
      error_count++;
    }
    error_sum += fabs(x[i] - y[i]);
  }
  cout << error_count << " : " << error_sum << endl;
}

int main(int argc, char *argv[]) {

  int    size  = 8 * 13489012;
  float *test  = new float[size];
  float *test1 = new float[size];
  float *test2 = new float[size];
  float *test3 = new float[size];
  memset(test1, 0, size * sizeof(float));
  memset(test2, 0, size * sizeof(float));
  memset(test3, 0, size * sizeof(float));
  init(test, size, 1, 0);

  Timer     t1, t2, t3;
  long long e1 = 0, e2 = 0, e3 = 0;
  for (int i = 0; i < 1e3; ++i) {

    t1.tic();
    memcpy(test1, test, size * sizeof(float));
    t1.toc();
    e1 += t1.Elapsed();

    t2.tic();
    copy(size, test2, test);
    t2.toc();
    e2 += t2.Elapsed();

    t3.tic();
    sve_copy(size, test3, test);
    t3.toc();
    e3 += t3.Elapsed();
  }

  cout << e1 << " | " << e2 << " | " << e3 << endl;

  check(test, test1, size);
  check(test, test2, size);
  check(test, test3, size);
  delete[] test;
  delete[] test1;
  delete[] test2;
  delete[] test3;
  return 0;
}
