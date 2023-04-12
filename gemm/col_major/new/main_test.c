/*  ______    _______  _______  ___      _______  __   __  __   __  __   __  __   __
|    _ |  |       ||   _   ||   |    |       ||  | |  ||  |_|  ||  |_|  ||  |_|  |
|   | ||  |    ___||  |_|  ||   |    |_     _||  |_|  ||       ||       ||       |
|   |_||_ |   |___ |       ||   |      |   |  |       ||       ||       ||       |
|    __  ||    ___||       ||   |___   |   |  |_     _| |     |  |     |  |     |
|   |  | ||   |___ |   _   ||       |  |   |    |   |  |   _   ||   _   ||   _   |
|___|  |_||_______||__| |__||_______|  |___|    |___|  |__| |__||__| |__||__| |__|*/

#include <arm_sve.h>
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "gemm.h"

// time count function
static struct timeval start;
static struct timeval end;

void tic(void) { gettimeofday(&start, NULL); }

double toc(void) {
  gettimeofday(&end, NULL);
  return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

#define TIME_START(num)                      \
  struct timeval tv_start##num, tv_end##num; \
  gettimeofday(&tv_start##num, NULL);
#define TIME_END(num, tag)                                    \
  gettimeofday(&tv_end##num, NULL);                           \
  printf("[%s-%d]:%s Cost=%fms\n", __func__, __LINE__, #tag,  \
         ((tv_end##num.tv_sec - tv_start##num.tv_sec) * 1e6 + \
          (tv_end##num.tv_usec - tv_start##num.tv_usec)) /    \
             1000.0f);

int get_vector_length() {
  int size = 0;

  __asm__ volatile(
      " mov  %[size], #0          \n\t"
      " incb %[size]              \n\t"
      : [size] "=r"(size)
      :
      :);
  return size * 8;
}

int main(int argc, char **argv) {
  // printf("%d bits\n", get_vector_length());
  // printf("%ld floats\n", svcntw());

  bool       P = false;
  int        argM;
  int        argN;
  int        argK;
  int        mc, nc, kc;
  VALUE_TYPE alpha;
  VALUE_TYPE beta;
  if (argc < 10) {
    printf(
        "USAGE: ./main $(M) $(N) $(K) $(alpha) $(beta)\n \
            USE default ./main 4 4 4 1 0 1");
    P     = true;
    argM  = 320;
    argN  = 320;
    argK  = 320;
    alpha = 1.0f;
    beta  = 0;
  } else {
    argM  = atoi(argv[1]);
    argN  = atoi(argv[2]);
    argK  = atoi(argv[3]);
    alpha = atof(argv[4]);
    beta  = atof(argv[5]);
    mc    = atoi(argv[6]);
    nc    = atoi(argv[7]);
    kc    = atoi(argv[8]);
    P     = (bool)(atoi(argv[9]));
  }
  // printf("M == %d, N == %d, K == %d\nMC == %d, NC == %d, KC == %d\nMR = %d, NR = %d\nalpha == %lf, beta == %lf\n", argM, argN, argK, MC, NC, KC, MR, NR, alpha, beta);
  int a_length = argM * argK;
  int b_length = argK * argN;
  int c_length = argM * argN;
  // printf("%f %f\n", alpha, beta);

  /* prepare MATRIX and init it; */
  MATRIX_TYPE a = (MATRIX_TYPE)malloc(a_length * sizeof(VALUE_TYPE));
  MATRIX_TYPE b = (MATRIX_TYPE)malloc(b_length * sizeof(VALUE_TYPE));
  // MATRIX_TYPE c     = (MATRIX_TYPE)malloc(c_length * sizeof(VALUE_TYPE));
  MATRIX_TYPE c     = (MATRIX_TYPE)malloc_aligned(c_length, 1, sizeof(VALUE_TYPE));
  MATRIX_TYPE c_ref = (MATRIX_TYPE)malloc_aligned(c_length, 1, sizeof(VALUE_TYPE));
  // MATRIX_TYPE c_ref = (MATRIX_TYPE)malloc(c_length * sizeof(VALUE_TYPE));
  assert(a != NULL && b != NULL && c != NULL && c_ref != NULL);
  randomInit(a, a_length);
  randomInit(b, b_length);
  // InstantInit(a, a_length, 1.f);
  // InstantInit(b, b_length, 1.f);
  InstantInit(c, c_length, 9.f);
  InstantInit(c_ref, c_length, 9.f);

  printf("omp has %d threads\n", omp_get_max_threads());

  /* print the  matrix before operation */
  if (P) {
    printFloat(a, argM, argK, argM, "a  :");
    printFloat(b, argK, argN, argK, "b  :");
  }
  /* printFloat(c_ref, argM, argN, argM, "C_ref  naive ways:"); */
  /* printFloat(c, argM, argN, argM, "C  my ways:"); */

  double time1, time2;

  /* naive gemm operation */
  CBLAS_ORDER     store_order = CblasColMajor;
  CBLAS_TRANSPOSE Atrans      = CblasNoTrans;
  CBLAS_TRANSPOSE Btrans      = CblasNoTrans;
  // tic();
  TIME_START(naive);
  naive_gemm(store_order, Atrans, Btrans, argM, argN, argK, alpha, a, argM, b, argK, beta, c_ref, argM);
  TIME_END(naive, naive);
  // time1 = toc();

  /* my gemm operation */
  // tic();
  TIME_START(sve);
  sgemm(argM, argN, argK, alpha, a, argM, b, argK, beta, c, argM, mc, nc, kc);
  TIME_END(sve, sve);
  // time2 = toc();

  /* print the matrix after operation */
  if (P) {
    printFloat(c_ref, argM, argN, argM, "C_ref  naive ways:");
    printFloat(c, argM, argN, argM, "C  my ways:");
  }

  // printf("naive : %lf\n", time1);
  // printf("my : %lf\n", time2);

  // printDiff(c_ref, c, argM, argN, 0, 1e-5);
  // printf("(compute / time ) my : %.3f,\t\t\t naive : %.3f\n", (2 * argM * argN * argK) * 1.f / time2, (2 * argM * argN * argK) * 1.f / time1);
  printf(" %.6f,\t\t\t  %.6f\n", (2 * argM * argN * argK) * 1.f / time2 / 1e9, (2 * argM * argN * argK) * 1.f / time1 / 1e9);

  /* free */
  free(a);
  free(b);
  free(c);
  free(c_ref);
}
