
#include <stdint.h>
#include <string.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include "args.h"
#include "matrix.h"
#ifdef __ARM_FEATURE_SVE
#include <arm_sve.h>
#endif

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
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_);
    return duration.count();
  }

 private:
  Clock::time_point start_, end_;
};

void im2col(float *data_im, int channels, int height, int width, int kernel_h,
            int kernel_w, int pad_h, int pad_w, int stride_h, int stride_w,
            int dilation_h, int dilation_w, float *data_col);

void im2col_neon(float *src, int channels, int height, int width, int kernel_h,
                 int kernel_w, int pad_h, int pad_w, int stride_h, int stride_w,
                 int dilation_h, int dilation_w, float *dst);

template <typename dtype>
bool check_result(const Matrix<dtype> &A, const Matrix<dtype> &B) {
  CHECK(A.size() == B.size());
  CHECK(A.H() == B.H());
  CHECK(A.W() == B.W());
  int                error     = 0;
  float              eps       = 1e-5;
  const dtype *const a         = A.get();
  const dtype *const b         = B.get();
  double             error_sum = 0.f;
#pragma omp parallel for
  for (int i = 0; i < A.size(); ++i) {
    if (fabs(a[i] - b[i]) > eps) {
      error++;
    }
    error_sum += fabs(a[i] - b[i]);
  }
  if (error != 0) {
    std::cout << "error count : " << error << std::endl;
    std::cout << "error sum : " << error_sum << std::endl;
    return false;
  } else {
#if defined(__ARM_FEATURE_SVE)
    std::cout << "passed CPU SVE test" << std::endl;
#else
    std::cout << "passed CPU NAIVE test" << std::endl;
#endif
    return true;
  }
}

int main(int argc, char **argv) {
#ifdef __ARM_FEATURE_SVE
  printf("sve vector has %d bits length\n", svcntb() * 8);
#endif
  srand(time(NULL));

  int image_c  = 128;
  int image_h  = 224;
  int image_w  = 224;
  int kernel_h = 3;
  int kernel_w = 3;
  if (argc > 1) {
    CHECK(argc == 6);
    image_c  = atoi(argv[1]);
    image_h  = atoi(argv[2]);
    image_w  = atoi(argv[3]);
    kernel_h = atoi(argv[4]);
    kernel_w = atoi(argv[5]);
  }

  // init args
  stride_args  stride{1, 1};
  padding_args pad{1, 1, 1, 1};
  // padding_args pad{0, 0, 0, 0};
  kernel_args k_arg{1, 1, kernel_h, kernel_w};
  image_args  i_arg{1, image_c, image_h, image_w};
  int         out_h = (i_arg.h - k_arg.h + pad.up + pad.down) / stride.h + 1;
  int         out_w = (i_arg.w - k_arg.w + pad.left + pad.right) / stride.w + 1;
  image_args  o_arg{1, 1, out_h, out_w};

  Matrix<float> image(i_arg.ic, i_arg.h, i_arg.w);

  Matrix<float> ref_image(i_arg.ic, k_arg.h * k_arg.w, o_arg.h * o_arg.w);
  Matrix<float> out_image(i_arg.ic, k_arg.h * k_arg.w, o_arg.h * o_arg.w);

  // kernel.init(0, 1);
  // float *src = new float[i_arg.size()];
  image.init(1, 1);
  ref_image.init(0, 0);
  out_image.init(0, 0);

  // image.print("input");
  // ref_image.print("ref");
  // out_image.print("my");

  Timer t1, t2;

  t1.tic();
  im2col_neon(image.get(), i_arg.ic, i_arg.h, i_arg.w, k_arg.h, k_arg.w, pad.up,
              pad.left, stride.h, stride.w, 1, 1, out_image.get());
  t1.toc();

  t2.tic();
  im2col(image.get(), i_arg.ic, i_arg.h, i_arg.w, k_arg.h, k_arg.w, pad.up,
         pad.left, stride.h, stride.w, 1, 1, ref_image.get());
  t2.toc();

  std::cout << "my    : " << t1.Elapsed() << std::endl;
  std::cout << "caffe : " << t2.Elapsed() << std::endl;

  if (!check_result<float>(ref_image, out_image)) {
    out_image.print("my", o_arg.h);
    ref_image.print("ref", o_arg.h);
  }
  // out_image.print("my");
  // ref_image.print("ref");
}
