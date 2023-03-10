#include "gemm.h"
#include "omp.h"
#include "stdlib.h"

// clang-format off


void packA_mcxkc_d(  // ? 实际上打包 mr * kc , 外部循环打包了 mc * kc
    int m, int k, float *XA, int ldXA,
    int    offseta,  // ? 从上倒下， 第 ic 个大块 的
    float *packA) {
  int    i, p;
  float *a_pntr[MR];  // ? 指针数组指向一纵列， 每次 一纵列一纵列循环赋值

  for (i = 0; i < m; i++) {
    a_pntr[i] = XA + (offseta + i);
  }

  for (i = m; i < MR; i++) {
    a_pntr[i] = XA + (offseta + 0);
  }

  for (p = 0; p < k; p++) {
    for (i = 0; i < MR; i++) {
      *packA = *a_pntr[i];
      packA++;
      a_pntr[i] = a_pntr[i] + ldXA;
    }
  }
}

/*
 * --------------------------------------------------------------------------
 */

void packB_kcxnc_d(  // ? 实际上只是打包了 kc * nr
    int n, int k, float *XB,
    int ldXB,  // ldXB is the original k
    int offsetb, float *packB) {
  int    j, p;
  float *b_pntr[NR];

  for (j = 0; j < n; j++) {  // ? 横向留住最顶上的指针位置， 这时候他们是跨 ldXB 的跨度, 但在后面将会放在一块
    b_pntr[j] = XB + ldXB * (offsetb + j);
  }

  for (j = n; j < NR; j++) {  // ? 当且仅当 jb - j  < NR 时候, 最后一块，做填充
    b_pntr[j] = XB + ldXB * (offsetb + 0);
  }

  for (p = 0; p < k; p++) {
    for (j = 0; j < NR; j++) {
      *packB++ = *b_pntr[j]++;
    }
  }
}
/*
 * --------------------------------------------------------------------------
 */

// clang-format off
void sgemm_Macro(const int M, const int N, const int K,
                     const VALUE_TYPE alpha,
                     const MATRIX_TYPE packA, const int lda,
                     const MATRIX_TYPE packB, const int ldb,
                     const VALUE_TYPE beta,
                     MATRIX_TYPE c, const int ldc)
// clang-format on
{
  /*
   *   first_time is true when update  A_block and C_pannel
   *   M = mc, N = nc, K = kc
   *  */

  int   i, j;
  void *a_next, *b_next;
  // aux_t aux;
  // char *str;
  a_next = packA;
  b_next = packB;

  for (j = 0; j < N; j += NR) {  // 2-th loop around micro-kernel
    for (i = 0; i < M; i += MR) {
      if (i + MR >= M) {
        b_next += NR * K;
      }
      if (i + MR >= M) {
        a_next = packA;
      } else {
        a_next += MR * K;
      }
      // (*bl_micro_kernel)(k, &packA[i * k], &packB[j * k], &C[j * ldc + i], (unsigned long long)ldc, &aux);
      sgemm_armv8a_sve_asm_2vx8(K, &alpha, &packA[i * K], &packB[j * K], &beta, &C(i, j), 1, ldc, a_next, b_next);
    }  // 1-th loop around micro-kernel
  }    // 2-th loop around micro-kernel
}

// clang-format off
void sgemm(const int M, const int N, const int K,
           const VALUE_TYPE alpha,
           const MATRIX_TYPE a, const int lda,
           const MATRIX_TYPE b, const int ldb,
           const VALUE_TYPE beta,
           MATRIX_TYPE c, const int ldc)
// clang-format on
{

  // iterator
  int jc;  //* jc : nc iterate N by stride of jc
  int pc;  //* iterate  K by stride of kc
  int ic;  //* iterate M by stride of mc

  // size value
  int jb;  // value gets by iterator jc qb : nc
  int pb;  // value gets by iterator p  pb : kc
  int ib;  // value gets by iterator i  ib : mc

  // iterator for pack
  int i;
  int j;

  // pack ptr
  float *packA;
  float *packB;

  // beta
  float _beta;

  int total_threads = 16;

  packA = malloc_aligned(KC, (MC + 1) * total_threads, sizeof(float));
  packB = malloc_aligned(KC, (NC + 1), sizeof(float));

  for (jc = 0; jc < N; jc += NC) { /* iterate nc */
    jb    = min(N - jc, NC);
    _beta = beta;

    for (pc = 0; pc < K; pc += KC) { /* iterate kc , and now B pannel maked*/
      if (pc != 0) {
        _beta = 1.f;
      }
      pb = min(K - pc, KC);

// TODO: here need to pack B
#pragma omp parallel for num_threads(total_threads) private(j)
      for (j = 0; j < jb; j += NR) {
        packB_kcxnc_d(min(jb - j, NR), pb, &b[pc], K, jc + j, &packB[j * pb]);
      }

#pragma omp parallel for num_threads(total_threads) private(ic, ib, i)
      for (ic = 0; ic < M; ic += MC) { /* iterate mc, and A block maked */
        int tid = omp_get_thread_num();
        ib      = min(M - ic, MC);

        for (i = 0; i < ib; i += MR) {
          packA_mcxkc_d(min(ib - i, MR), pb, &a[pc * lda], M, ic + i, &packA[tid * MC * pb + i * pb]);
        }
        sgemm_Macro(ib, jb, pb, alpha, packA + tid * MC * pb, lda, packB, ldb, _beta, &C(ic, jc), ldc);
      }
    }
  }
  free(packA);
  free(packB);
}

/*
 * A block  is mc * kc size
 * B pannel is kc * nc size
 * C block  is mc * nc size
 */
