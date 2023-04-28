#include <arm_sve.h>
#include <limits.h>
#include <omp.h>
#include <stdio.h>

enum Activation {
  abs,
  linear,
  logistic,
  relu,
  bounded_relu,
  lu_bounded_relu,
  leaky_relu,
  soft_relu,
  elu,
  sqrt,
  tanh,
  square,
  sigmoid
};

inline svfloat32_t svtanh_f32_z(svbool_t pg, svfloat32_t val)
{
    const svfloat32_t CONST_1        = svdup_n_f32(1.f);
    const svfloat32_t CONST_2        = svdup_n_f32(2.f);
    const svfloat32_t CONST_MIN_TANH = svdup_n_f32(-10.f);
    const svfloat32_t CONST_MAX_TANH = svdup_n_f32(10.f);

    svfloat32_t x     = svmin_f32_z(pg, svmax_f32_z(pg, val, CONST_MIN_TANH), CONST_MAX_TANH);
    svfloat32_t exp2x = svexp_f32_z(pg, svmul_f32_z(pg, CONST_2, x));
    svfloat32_t num   = svsub_f32_z(pg, exp2x, CONST_1);
    svfloat32_t den   = svadd_f32_z(pg, exp2x, CONST_1);
    svfloat32_t tanh  = svdiv_f32_z(pg, num, den);
    return tanh;
}

void leaky_relu_kernel(svfloat32_t src, svbool_t pg, svfloat32_t dst, float alpha,
                float beta) {}

void relu_kernel(svfloat32_t src, svbool_t pg, svfloat32_t dst, float alpha,
          float beta) {}

void tanh_kernel(svfloat32_t src, svbool_t pg, svfloat32_t dst, float alpha,
          float beta) {}

void sigmoid_kernel(svfloat32_t src, svbool_t pg, svfloat32_t dst, float alpha,
             float beta) {}

inline void elementwise_unary_impl(svfloat32_t src, svbool_t pg,
                                   svfloat32_t dst, Activation op) {

  void (*func)(svfloat32_t src, svbool_t pg, svfloat32_t dst, float alpha,
               float beta) = NULL;
  switch (op) {
    case sigmoid:
      func = sigmoid_kernel;
      break;
    case relu:
      func = relu_kernel;
      break;
    case leaky_relu:
      func = leaky_relu_kernel;
      break;
    case tanh:
      func = tanh_kernel;
      break;
    default:
      printf("unsupported");
      break;
  }
}

int main() {}
