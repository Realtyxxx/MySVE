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
