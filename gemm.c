/*
 * @Author       : Realtyxxx
 * @Descripttion : SVE_GEMM
 * @version      : 1.0
 * @Date         : 2022-01-11 19:29:26
 * @LastEditors  : Realtyxxx
 * @LastEditTime : 2022-01-15 04:01:25
 * @FilePath     : /sve/sve_gemm/gemm.c
 * @ToDo         :
 */

#include "gemm.h"

// clang-format off
void naive_gemm(const CBLAS_ORDER order,
              const CBLAS_TRANSPOSE Atrans,
              const CBLAS_TRANSPOSE Btrans,
              const int M, const int N, const int K,
              const VALUE_TYPE alpha,
              const MATRIX_TYPE a, const int lda,
              const MATRIX_TYPE b, const int ldb,
              const VALUE_TYPE beta,
              MATRIX_TYPE c, const int ldc)
// clang-format on
{
  MATRIX_TYPE T = (MATRIX_TYPE)malloc((unsigned long)M * N * sizeof(VALUE_TYPE));
  memset(T, 0, M * N * sizeof(VALUE_TYPE));
  int i, j, k;
  for (j = 0; j < N; j++) {
    for (i = 0; i < M; i++) {
      C(i, j) = beta * C(i, j);
    }
  }
  for (j = 0; j < N; j++) {
    for (i = 0; i < M; i++) {
      for (k = 0; k < K; k++) {
        T[i + j * ldc] += A(i, k) * B(k, j);
      }
    }
  }
  for (j = 0; j < N; j++) {
    for (i = 0; i < M; i++) {
      C(i, j) += alpha * T[i + j * ldc];
    }
  }
  free(T);
}

void PackMatrixA(int k, double *a, int lda, double *a_to) {
  int j;

  for (j = 0; j < k; j++) { /* loop over columns of A */
    double *a_ij_pntr = &A(0, j);

    *a_to++ = *a_ij_pntr;
    *a_to++ = *(a_ij_pntr + 1);
    *a_to++ = *(a_ij_pntr + 2);
    *a_to++ = *(a_ij_pntr + 3);
  }
}

void PackMatrixB(int k, double *b, int ldb, double *b_to) {
  int i;

  double *b_i0_pntr = &B(0, 0), *b_i1_pntr = &B(0, 1), *b_i2_pntr = &B(0, 2), *b_i3_pntr = &B(0, 3);

  for (i = 0; i < k; i++) { /* loop over rows of B */
    *b_to++ = *b_i0_pntr++;
    *b_to++ = *b_i1_pntr++;
    *b_to++ = *b_i2_pntr++;
    *b_to++ = *b_i3_pntr++;
  }
}

// clang-format off


void my_dgemm_inside(const int M, const int N, const int K,
                     const VALUE_TYPE alpha,
                     const MATRIX_TYPE a, const int lda,
                     const MATRIX_TYPE b, const int ldb,
                     const VALUE_TYPE beta,
                     MATRIX_TYPE c, const int ldc,
                     bool first_time)
// clang-format on
{
  int           i, j;
  double        packedA[M * K];
  static double packedB[kc * nb];

  // nr is 4
  //  Loop over the columns of C, unrolled by 4
  for (j = 0; j < N; j += 4) {

    if (first_time) PackMatrixB(K, &B(0, j), ldb, &packedB[j * K]);

    // mr is 4
    for (i = 0; i < M; i += 4) {

      // Loop over the rows of C
      // Update C( i,j ), C( i,j+1 ), C( i,j+2 ), and C( i,j+3 )
      // in one routine (four inner products)

      if (j == 0) PackMatrixA(K, &A(i, 0), lda, &packedA[i * K]);

      // in register computation
      // C (mr(4) * nr(4)) = A (mr * kc) * B (kc  * nr) * alpha + beta * C;

      add_dot_4x4_sve(K, &packedA[i * K], 4, &packedB[j * K], K, &alpha, &C(i, j), ldc, &beta);
    }
  }
}

// clang-format off
void my_dgemm(const CBLAS_ORDER order,
              const CBLAS_TRANSPOSE Atrans,
              const CBLAS_TRANSPOSE Btrans,
              const int M, const int N, const int K,
              const VALUE_TYPE alpha,
              const MATRIX_TYPE a, const int lda,
              const MATRIX_TYPE b, const int ldb,
              const VALUE_TYPE beta,
              MATRIX_TYPE c, const int ldc)
// clang-format on
{
  if (order == CblasRowMajor) {
    if (lda != M || ldb != K || ldc != M) {
      printf("not for this gemm\n");
      return;
    }
  }
  int i, p, pb, ib;
  for (p = 0; p < K; p += kc) {
    pb = min(K - p, kc);
    for (i = 0; i < M; i += mc) {
      ib = min(M - i, mc);
      // gebp Ablock(mc * kc) x Bpanel(kc * nc) but now the nc is N
      printf("i == %d\n", i);
      my_dgemm_inside(ib, N, pb, alpha, &A(i, p), lda, &B(p, 0), ldb, beta, &C(i, 0), ldc, i == 0);
    }
  }
}
