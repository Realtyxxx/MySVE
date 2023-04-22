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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "gemv.h"
#include "util.h"

// time count function
static struct timeval start;
static struct timeval end;

void tic(void) { gettimeofday(&start, NULL); }

double toc(void) {
  gettimeofday(&end, NULL);
  return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

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

int main(int argc, char** argv) {
  printf("%d bits\n", get_vector_length());
  printf("%ld floats\n", svcntw());

  bool P = false;
  int  argM;
  int  argN;
  int  argK;
  if (argc < 5) {
    printf(
        "USAGE: ./main $(M) $(N) $(K) $(alpha) $(beta)\n \
            USE default ./main 256 256 256 1 0 0");
    P    = false;
    argM = 256;
    argK = 256;
    argN = 256;
  } else {
    argM = atoi(argv[1]);
    argK = atoi(argv[2]);
    argN = atoi(argv[3]);
    P    = (bool)(atoi(argv[4]));
  }
  printf("MB == %d, IC == %d, OC == %d\n", argM, argK, argN);
  int a_length = argM * argK;
  int b_length = argK * argN;
  int c_length = argM * argN;
  // printf("%f %f\n", alpha, beta);

  /* prepare MATRIX and init it; */
  float* a     = (float*)malloc(a_length * sizeof(float));
  float* b     = (float*)malloc(b_length * sizeof(float));
  float* c     = (float*)malloc(c_length * sizeof(float));
  float* c2     = (float*)malloc(c_length * sizeof(float));
  float* c_ref = (float*)malloc(c_length * sizeof(float));

  assert(a != NULL && b != NULL && c != NULL && c_ref != NULL);

  randomInit(a, a_length);
  randomInit(b, b_length);
  // InstantInit(a, a_length, 1.f);
  // InstantInit(b, b_length, 1.f);

  InstantInit(c, c_length, 0.f);
  InstantInit(c2, c_length, 0.f);
  InstantInit(c_ref, c_length, 0.f);

  /* print the  matrix before operation */
  if (P) {
    printFloat(a, argM, argK, argM, "a  :");
    printFloat(b, argK, argN, argK, "b  :");
  }
  /* printFloat(c_ref, argM, argN, argM, "C_ref  naive ways:"); */
  /* printFloat(c, argM, argN, argM, "C  my ways:"); */

  double time1, time2, time3;

  /* naive gemm operation */
  tic();
  fully_connect_naive_ccc(a, b, c_ref, argM, argK, argN);
  time1 = toc();

  /* my gemm operation */
  tic();
  fully_connect_ccc_2(a, b, c2, argM, argK, argN);
  time2 = toc();


  tic();
  fully_connect_ccc(a, b, c, argM, argK, argN);
  time3 = toc();

  /* print the matrix after operation */
  if (P) {
    printFloat(c_ref, argM, argN, argM, "C_ref  naive ways:");
    printFloat(c2, argM, argN, argM, "C  2  ways:");
    printFloat(c, argM, argN, argM, "C  my ways:");
  }

  printf("time   :\n");
  printf("naive   : %lf\n", (time1));
  printf("compare : %lf\n", (time2));
  printf("my      : %lf\n", (time3));

  // printf("gflops :\n");
  // printf("naive : %lf\n", (2.f * argM * argN * argK) / (time1 * 1e9));
  // printf("my : %lf\n", (2.f * argM * argN * argK) / (time2 * 1e9));

  printDiff(c_ref, c2, argM, argN, 5, 1e-3);
  printDiff(c_ref, c, argM, argN, 5, 1e-3);
  printDiff(c2, c, argM, argN, 5, 1e-3);

  /* free */
  free(a);
  free(b);
  free(c);
  free(c_ref);
}
