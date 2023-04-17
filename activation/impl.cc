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

inline void elementwise_unary_impl(svfloat32_t src, svbool_t pg, svfloat32_t dst, ElementWiseUnary op) {
  switch (op) {
    case ElementWiseUnary::exp:
      dst = svexp_f32_z(pg, src);
      break;
    case ElementWiseUnary::neg:
      dst = svneg_z(pg, src);
      break;
    case ElementWiseUnary::abs:
      dst = svabs_z(pg, src);
      break;
    default:
      printf("unsupported");
      break;
  }
}
