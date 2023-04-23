#include <arm_sve.h>

// for m < mr || n < nr case
// #define print(str) printf(str)
#define print(str)

// vl x k x 1
#include "vl_1.h"

// vl x k x 2
#include "vl_2.h"

// vl x k x 4
#include "vl_4.h"

// vl x k x 8
#include "vl_8.h"

void vl_n(int m, int n, int k, float* alpha, float* a, float* b, float* beta,
          float* c, int rs_c, int cs_c, void* a_next, void* b_next) {
  switch (n) {
    case 7: {
      print("case : 7\n");
      vl_4(m, n, k, alpha, a, b, beta, c, rs_c, cs_c, a_next, b_next);
      vl_2(m, n, k, alpha, a, b + 4, beta, c + 4 * cs_c, rs_c, cs_c, a_next, b_next);
      vl_1(m, n, k, alpha, a, b + 6, beta, c + 6 * cs_c, rs_c, cs_c, a_next, b_next);
      break;
    }
    case 6: {
      print("case : 6\n");
      vl_4(m, n, k, alpha, a, b, beta, c, rs_c, cs_c, a_next, b_next);
      vl_2(m, n, k, alpha, a, b + 4, beta, c + 4 * cs_c, rs_c, cs_c, a_next, b_next);
      break;
    }
    case 5: {
      print("case : 5\n");
      vl_4(m, n, k, alpha, a, b, beta, c, rs_c, cs_c, a_next, b_next);
      vl_1(m, n, k, alpha, a, b + 4, beta, c + 4 * cs_c, rs_c, cs_c, a_next, b_next);
      break;
    }
    case 4: {
      print("case : 4\n");
      vl_4(m, n, k, alpha, a, b, beta, c, rs_c, cs_c, a_next, b_next);
      break;
    }
    case 3: {
      print("case : 3\n");
      vl_2(m, n, k, alpha, a, b, beta, c, rs_c, cs_c, a_next, b_next);
      vl_1(m, n, k, alpha, a, b + 2, beta, c + 2 * cs_c, rs_c, cs_c, a_next, b_next);
      break;
    }
    case 2: {
      print("case : 2\n");
      vl_2(m, n, k, alpha, a, b, beta, c, rs_c, cs_c, a_next, b_next);
      break;
    }
    case 1: {
      print("case : 1\n");
      vl_1(m, n, k, alpha, a, b, beta, c, rs_c, cs_c, a_next, b_next);
      break;
    }
  }
}


// void mini_impl(int k, float* alpha, float* a, float* b, float* beta, float*
// c,
//                int rs_c, int sc_c, void* a_next, void* b_next) {}
