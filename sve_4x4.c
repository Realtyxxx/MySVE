/*
 * @Author       : Realtyxxx
 * @Descripttion : SVE_GEMM
 * @version      : 1.0
 * @Date         : 2022-01-12 16:49:19
 * @LastEditors  : Realtyxxx
 * @LastEditTime : 2022-01-15 03:49:01
 * @FilePath     : /sve/sve_gemm/sve_4x4.c
 * @ToDo         :
 */
#include "gemm.h"

// @msg: in register computation C (mr(4) * nr(4)) + A (mr * kc) * B (kc  * nr) * alpha + beta * C; * @param {int} k
void add_dot_4x4_sve(int k, double *restrict a, int *restrict lda, double *b, int *restrict ldb, int *restrict alpha,
                     double *restrict c, int *restrict ldc, int *restrict beta) {
  /*
   *       So, this routine computes a 4x4 block of matrix A
   *
   *            C( 0, 0 ), C( 0, 1 ), C( 0, 2 ), C( 0, 3 ).
   *            C( 1, 0 ), C( 1, 1 ), C( 1, 2 ), C( 1, 3 ).
   *            C( 2, 0 ), C( 2, 1 ), C( 2, 2 ), C( 2, 3 ).
   *            C( 3, 0 ), C( 3, 1 ), C( 3, 2 ), C( 3, 3 ).
   *
   *      Notice that this routine is called with c = C( i, j ) in the
   *      previous routine, so these are actually the elements
   *
   *           C( i  , j ), C( i  , j+1 ), C( i  , j+2 ), C( i  , j+3 )
   *           C( i+1, j ), C( i+1, j+1 ), C( i+1, j+2 ), C( i+1, j+3 )
   *           C( i+2, j ), C( i+2, j+1 ), C( i+2, j+2 ), C( i+2, j+3 )
   *           C( i+3, j ), C( i+3, j+1 ), C( i+3, j+2 ), C( i+3, j+3 )
   *
   *      in the original matrix C
   *
   *    if (*beta == 0) scal c -> 0;
   *
   *    C : load 4 c_vec to store the C
   *
   *    for (int p = 0; p < k; p++) {
   *    A :  load a_vector only 1 vector;
   *    B :  inc the ptr and dup the b value to each b_vec, we need 4 b_vec and 4 b_ptr
   *
   *    c_00_c_10_c_20_c_30 += a_0p_a_1p_a_2p_a_3p * b_p0_b_p0_b_p0_b_p0;
   *    c_01_c_11_c_21_c_31 += a_0p_a_1p_a_2p_a_3p * b_p1_b_p1_b_p1_b_p1;
   *    c_02_c_12_c_22_c_32 += a_0p_a_1p_a_2p_a_3p * b_p2_b_p2_b_p2_b_p2;
   *    c_03_c_13_c_23_c_33 += a_0p_a_1p_a_2p_a_3p * b_p2_b_p3_b_p3_b_p3;
   *
   *    } */

  /* 4 times unrolled */
  uint64_t k_iter = k / 4;
  uint64_t k_left = k % 4;

  __asm__ volatile(
      "                                            \n\t"
      " ldr x0, %[a]                               \n\t" /* load the address of the A(0, 0) */
      " ldr x1, %[b]                               \n\t" /* load the address of the B(0, 0) */
      " ldr x2, %[c]                               \n\t" /* load the address of the C(0, 0) */
      "                                            \n\t"
      /* " ldr x3, %[lda]                             \n\t" */
      /* " lsl x3, x3, #3                             \n\t" [> lda * sizeof(double) <] */
      "                                            \n\t"
      /* " ldr x4, %[ldb]                             \n\t" */
      /* " lsl x4, x4, #3                             \n\t" */
      "                                            \n\t"
      " ldr x5, %[ldc]                             \n\t"
      " lsl x5, x5, #3                             \n\t"
      "                                            \n\t"
      " ldr x6, %[alpha]                           \n\t"
      " ldr x7, %[beta]                            \n\t"
      "                                            \n\t"
      " ldr x8, %[k_iter]                          \n\t"
      "                                            \n\t"
      " prfm pldl1keep, [x2]                       \n\t" /* Prefetch c_row1 */
      " prfm pldl1keep, [x20]                      \n\t" /* Prefetch c_row2 */
      " prfm pldl1keep, [x21]                      \n\t" /* Prefetch c_row3 */
      " prfm pldl1keep, [x22]                      \n\t" /* Prefetch c_row4 */
      "                                            \n\t"
      " add x10, x1,  #8                           \n\t" /* get the address of the B(0, 1) */
      " add x11, x1,  #16                          \n\t" /* get the address of the B(0, 2) */
      " add x12, x1,  #24                          \n\t" /* get the address of the B(0, 3) */
      "                                            \n\t"
      " add x20, x2,  x5                           \n\t" /* get the address of the C(0, 1) */
      " add x21, x20, x5                           \n\t" /* get the address of the C(0, 2) */
      " add x22, x21, x5                           \n\t" /* get the address of the C(0, 3) */
      "                                            \n\t"
      " ptrue p0.d, VL4                            \n\t" /* 4 elements per vector , actually we could use 8 elements */
      "                                            \n\t"
      "                                            \n\t" /* !!!first load */
      " ld1d z0.d, p0/z, [x0]                      \n\t" /* load A(0, p) A(1, p) A(2, p) A(3, p)*/
      "                                            \n\t"
      " ld1rd z1.d, p0/z, [x1]                     \n\t" /* boradcast load vector B(p, 0) */
      " ld1rd z2.d, p0/z, [x10]                    \n\t" /* boradcast load vector B(p, 1) */
      " ld1rd z3.d, p0/z, [x11]                    \n\t" /* boradcast load vector B(p, 2) */
      " ld1rd z4.d, p0/z, [x12]                    \n\t" /* boradcast load vector B(p, 3) */
      "                                            \n\t"
      " dup z10.d, #0                              \n\t" /* tmp Vec prepare */
      " dup z11.d, #0                              \n\t"
      " dup z12.d, #0                              \n\t"
      " dup z13.d, #0                              \n\t"
      "                                            \n\t"
      " fmla z10.d, p0/m, z0.d, z1.d               \n\t" /* add_dot */
      " fmla z11.d, p0/m, z0.d, z2.d               \n\t"
      " fmla z12.d, p0/m, z0.d, z3.d               \n\t"
      " fmla z13.d, p0/m, z0.d, z4.d               \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t" /* !!!second load */
      " ld1d z0.d, p0/z, [x0, #1, MUL VL]          \n\t" /* load A(0, p + 1) A(1, p + 1) A(2, p + 1) A(3, p + 1)*/
      "                                            \n\t"
      " add x1,  x1,  #32                          \n\t" /* move the B ptr */
      " add x10, x10, #32                          \n\t"
      " add x11, x11, #32                          \n\t"
      " add x12, x21, #32                          \n\t"
      "                                            \n\t"
      " ld1rd z1.d, p0/z, [x1]                     \n\t" /* boradcast load vector B(p + 1, 0) */
      " ld1rd z2.d, p0/z, [x10]                    \n\t" /* boradcast load vector B(p + 1, 1) */
      " ld1rd z3.d, p0/z, [x11]                    \n\t" /* boradcast load vector B(p + 1, 2) */
      " ld1rd z4.d, p0/z, [x12]                    \n\t" /* boradcast load vector B(p + 1, 3) */
      "                                            \n\t"
      " fmla z10.d, p0/m, z0.d, z1.d               \n\t" /* add_dot */
      " fmla z11.d, p0/m, z0.d, z2.d               \n\t"
      " fmla z12.d, p0/m, z0.d, z3.d               \n\t"
      " fmla z13.d, p0/m, z0.d, z4.d               \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t" /* third load */
      " ld1d z0.d, p0/z, [x0, #2, MUL VL]          \n\t" /* load A(0, p + 2) A(1, p + 2) A(2, p + 2) A(3, p + 2)*/
      "                                            \n\t"
      " add x1,  x1,  #32                          \n\t" /* move the B ptr */
      " add x10, x10, #32                          \n\t"
      " add x11, x11, #32                          \n\t"
      " add x12, x21, #32                          \n\t"
      "                                            \n\t"
      " ld1rd z1.d, p0/z, [x1]                     \n\t" /* boradcast load vector B(p + 2, 0) */
      " ld1rd z2.d, p0/z, [x10]                    \n\t" /* boradcast load vector B(p + 2, 1) */
      " ld1rd z3.d, p0/z, [x11]                    \n\t" /* boradcast load vector B(p + 2, 2) */
      " ld1rd z4.d, p0/z, [x12]                    \n\t" /* boradcast load vector B(p + 2, 3) */
      "                                            \n\t"
      "                                            \n\t"
      " fmla z10.d, p0/m, z0.d, z1.d               \n\t" /* add_dot */
      " fmla z11.d, p0/m, z0.d, z2.d               \n\t"
      " fmla z12.d, p0/m, z0.d, z3.d               \n\t"
      " fmla z13.d, p0/m, z0.d, z4.d               \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t" /* forth load */
      " ld1d z0.d, p0/z, [x0, #3, MUL VL]          \n\t" /* load A(0, p + 3) A(1, p + 3) A(2, p + 3) A(3, p + 3)*/
      "                                            \n\t"
      " add x1,  x1,  #32                          \n\t" /* move the B ptr */
      " add x10, x10, #32                          \n\t"
      " add x11, x11, #32                          \n\t"
      " add x12, x21, #32                          \n\t"
      "                                            \n\t"
      " ld1rd z1.d, p0/z, [x1]                     \n\t" /* boradcast load vector B(p + 3, 0) */
      " ld1rd z2.d, p0/z, [x10]                    \n\t" /* boradcast load vector B(p + 3, 1) */
      " ld1rd z3.d, p0/z, [x11]                    \n\t" /* boradcast load vector B(p + 3, 2) */
      " ld1rd z4.d, p0/z, [x12]                    \n\t" /* boradcast load vector B(p + 3, 3) */
      "                                            \n\t"
      " fmla z10.d, p0/m, z0.d, z1.d               \n\t" /* add_dot */
      " fmla z11.d, p0/m, z0.d, z2.d               \n\t"
      " fmla z12.d, p0/m, z0.d, z3.d               \n\t"
      " fmla z13.d, p0/m, z0.d, z4.d               \n\t"
      "                                            \n\t"
      " add x0, x0, #128                           \n\t" /* after unrolled, we need to update the A ptr */
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " st1d {z10.d}, p0, [x2]                     \n\t"
      " st1d {z11.d}, p0, [x20]                    \n\t"
      " st1d {z12.d}, p0, [x21]                    \n\t"
      " st1d {z13.d}, p0, [x22]                    \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " ld1rd z10.d, p0/z, [x6]                    \n\t" /* broad load alpha */
      " ld1rd z11.d, p0/z, [x7]                    \n\t" /* broad load beta */
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      :                        // output operands (none)
      :                        // input operands
      [ a ] "m"(a),            // 0
      [ b ] "m"(b),            // 1
      [ c ] "m"(c),            // 2
      [ lda ] "m"(lda),        // 3
      [ ldb ] "m"(ldb),        // 4
      [ ldc ] "m"(ldc),        // 5
      [ alpha ] "m"(alpha),    // 6
      [ beta ] "m"(beta),      // 7
      [ k_iter ] "m"(k_iter),  // 8
      [ k_left ] "m"(k_left)   // 9
      :                        // Register clobber list
      "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9",
      /* use for the initial address */
      "x10", "x11", "x12",        /* use for B ptr */
      "x20", "x21", "x22",        /* use for C ptr */
      "z0",                       /* use for A  vec */
      "z1", "z2", "z3", "z4",     /* use for B vec */
      "z10", "z11", "z12", "z13", /* use for tmp-vec to accumulate the result of AxB */
      "p0");
}
