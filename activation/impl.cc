#include <arm_sve.h>
#include <limits.h>
#include <omp.h>

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
  square
};

void leaky_relu(svfloat32_t src, svbool_t pg, svfloat32_t dst, float alpha,
                float beta) {}

void relu(svfloat32_t src, svbool_t pg, svfloat32_t dst, float alpha,
          float beta) {}

void tanh(svfloat32_t src, svbool_t pg, svfloat32_t dst, float alpha,
          float beta) {}

void sigmoid(svfloat32_t src, svbool_t pg, svfloat32_t dst, float alpha,
             float beta) {}

inline void elementwise_unary_impl(svfloat32_t src, svbool_t pg,
                                   svfloat32_t dst, ElementWiseUnary op) {

  void(*func(svfloat32_t src, svbool_t pg, svfloat32_t dst, float alpha,
             float beta));
  switch ((enum Activation) op) {
    case Activation::sigmoid:
      func = sigmoid;
      break;
    case Activation::relu:
      func = relu;
      break;
    case Activation::leaky_relu:
      dst = svneg_z(pg, src);
      break;
    case Activation::tanh:
      dst = svabs_z(pg, src);
      break;
    default:
      printf("unsupported");
      break;
  }
}

int main(){}
