#include "gemm.h"
// clang-format off
// void naive_gemm(const CBLAS_ORDER order,
//               const CBLAS_TRANSPOSE Atrans,
//               const CBLAS_TRANSPOSE Btrans,
//               const int M, const int N, const int K,
//               const VALUE_TYPE alpha,
//               const MATRIX_TYPE a, const int lda,
//               const MATRIX_TYPE b, const int ldb,
//               const VALUE_TYPE beta,
//               MATRIX_TYPE c, const int ldc)
// // clang-format on
// {
//   MATRIX_TYPE T = (MATRIX_TYPE)malloc((unsigned long)M * N * sizeof(VALUE_TYPE));
//   memset(T, 0, M * N * sizeof(VALUE_TYPE));
//   int i, j, k;
//   for (j = 0; j < N; j++) {
//     for (i = 0; i < M; i++) {
//       C(i, j) = beta * C(i, j);
//     }
//   }
//   for (j = 0; j < N; j++) {
//     for (i = 0; i < M; i++) {
//       for (k = 0; k < K; k++) {
//         T[i + j * ldc] += A(i, k) * B(k, j);
//       }
//     }
//   }
//   for (j = 0; j < N; j++) {
//     for (i = 0; i < M; i++) {
//       C(i, j) += alpha * T[i + j * ldc];
//     }
//   }
//   free(T);
// }

void naive_gemm(const BLAS_ORDER order,
              const BLAS_TRANSPOSE Atrans,
              const BLAS_TRANSPOSE Btrans,
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
#if MULTI_THREADS
  // printf("MULTI_THREADS\n");
#endif
#if MULTI_THREADS
#pragma omp parallel for private(i, j)
#endif
  for (j = 0; j < N; j++) {
    for (i = 0; i < M; i++) {
      C(i, j) = beta * C(i, j);
    }
  }
#if MULTI_THREADS
#pragma omp parallel for private(j, i, k)
#endif
  for (j = 0; j < N; j++) {
    for (i = 0; i < M; i++) {
      for (k = 0; k < K; k++) {
        T[i + j * ldc] += A(i, k) * B(k, j);
      }
    }
  }
#if MULTI_THREADS
#pragma omp parallel for private(j, i)
#endif
  for (j = 0; j < N; j++) {
    for (i = 0; i < M; i++) {
      C(i, j) += alpha * T[i + j * ldc];
    }
  }
  free(T);
}
