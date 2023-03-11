
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
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_);
    return duration.count();
  }

 private:
  Clock::time_point start_, end_;
};

bool is_a_ge_zero_and_a_lt_b(int a, int b) {
  if ((0 <= a) && (a < b)) {
    return true;
  } else
    return false;
}
void im2col_cpu(float *data_im, int channels, int height, int width, int kernel_h, int kernel_w, int pad_h, int pad_w,
                int stride_h, int stride_w, int dilation_h, int dilation_w, float *data_col) {
  const int output_h     = (height + 2 * pad_h - (dilation_h * (kernel_h - 1) + 1)) / stride_h + 1;
  const int output_w     = (width + 2 * pad_w - (dilation_w * (kernel_w - 1) + 1)) / stride_w + 1;
  const int channel_size = height * width;
  for (int channel = channels; channel--; data_im += channel_size) {
    for (int kernel_row = 0; kernel_row < kernel_h; kernel_row++) {
      for (int kernel_col = 0; kernel_col < kernel_w; kernel_col++) {
        int input_row = -pad_h + kernel_row * dilation_h;
        for (int output_rows = output_h; output_rows; output_rows--) {
          if (!is_a_ge_zero_and_a_lt_b(input_row, height)) {
            for (int output_cols = output_w; output_cols; output_cols--) { *(data_col++) = 0; }
          } else {
            int input_col = -pad_w + kernel_col * dilation_w;
            for (int output_col = output_w; output_col; output_col--) {
              if (is_a_ge_zero_and_a_lt_b(input_col, width)) {
                *(data_col++) = data_im[input_row * width + input_col];
              } else {
                *(data_col++) = 0;
              }
              input_col += stride_w;
            }
          }
          input_row += stride_h;
        }
      }
    }
  }
}

void im2col_f32(void *const src, void *const dst, const stride_args &stride, const padding_args &pad,
                const image_args &image_hw, const kernel_args &kernel_hw, const image_args &o_image_hw);

template <typename dtype>
bool check_result(const Matrix<dtype> &A, const Matrix<dtype> &B) {
  CHECK(A.size() == B.size());
  CHECK(A.H() == B.H());
  CHECK(A.W() == B.W());
  int                error = 0;
  float              eps   = 1e-5;
  const dtype *const a     = A.get();
  const dtype *const b     = B.get();
#pragma omp parallel for
  for (int i = 0; i < A.size(); ++i) {
    if (abs(a[i] - b[i]) > eps) {
      error++;
    }
  }
  if (error != 0) {
    std::cout << "error count : " << error << std::endl;
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
  im2col_f32(image.get(), out_image.get(), stride, pad, i_arg, k_arg, o_arg);
  t1.toc();

  t2.tic();
  im2col_cpu(image.get(), i_arg.ic, i_arg.h, i_arg.w, k_arg.h, k_arg.w, pad.up, pad.left, stride.h, stride.w, 1, 1,
             ref_image.get());
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
