/*
 * @Author       : Realtyxxx
 * @Descripttion : GEMM_TEST
 * @version      : 1.0
 * @Date         : 2022-01-11 19:30:04
 * @LastEditors  : Realtyxxx
 * @LastEditTime : 2022-01-15 04:02:38
 * @FilePath     : /sve/sve_gemm/main.c
 * @ToDo         :
 */

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

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("USAGE: ./main $(M) $(N) $(K) $(alpha) $(beta)\n");
    return -1;
  }
  int             argM        = atoi(argv[1]);
  int             argN        = atoi(argv[2]);
  int             argK        = atoi(argv[3]);
  VALUE_TYPE      alpha       = atof(argv[4]);
  VALUE_TYPE      beta        = atof(argv[5]);
  int             a_length    = argM * argK;
  int             b_length    = argK * argN;
  int             c_length    = argM * argN;
  CBLAS_ORDER     store_order = CblasColMajor;
  CBLAS_TRANSPOSE Atrans      = CblasNoTrans;
  CBLAS_TRANSPOSE Btrans      = CblasNoTrans;
  // printf("%f %f\n", alpha, beta);

  /* prepare MATRIX and init it; */
  MATRIX_TYPE a     = (MATRIX_TYPE)malloc(a_length * sizeof(VALUE_TYPE));
  MATRIX_TYPE b     = (MATRIX_TYPE)malloc(b_length * sizeof(VALUE_TYPE));
  MATRIX_TYPE c     = (MATRIX_TYPE)malloc(c_length * sizeof(VALUE_TYPE));
  MATRIX_TYPE c_ref = (MATRIX_TYPE)malloc(c_length * sizeof(VALUE_TYPE));
  assert(a != NULL && b != NULL && c != NULL && c_ref != NULL);
  // randomInit(a, a_length);
  // randomInit(b, b_length);
  integerInit(a, a_length, 1);
  integerInit(b, b_length, 1);
  /* memset(c, 0, c_length); */
  memset(c_ref, 0, c_length);
  memset(c, 0, c_length);

  /* print the  matrix before operation */
  /* printFloat(a, argM, argK, argM, "a  :"); */
  /* printFloat(b, argK, argN, argK, "b  :"); */
  /* printFloat(c_ref, argM, argN, argM, "C_ref  naive ways:"); */
  /* printFloat(c, argM, argN, argM, "C  my ways:"); */

  double time;

  /* gemm operation */
  tic();
  naive_gemm(store_order, Atrans, Btrans, argM, argN, argK, alpha, a, argM, b, argK, beta, c_ref, argM);
  time = toc();
  printf("naive : %lf\n", time);
  printf("runned\n");
  tic();
  my_dgemm(store_order, Atrans, Btrans, argM, argN, argK, alpha, a, argM, b, argK, beta, c, argM);
  time = toc();
  printf("my : %lf\n", time);

  /* print the matrix after operation */
  /* printFloat(c_ref, argM, argN, argM, "C_ref  naive ways:"); */
  /* printFloat(c, argM, argN, argM, "C  my ways:"); */

  /* free */
  free(a);
  free(b);
  free(c);
  free(c_ref);
}
