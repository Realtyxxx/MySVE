#include <stdio.h>
#include <stdlib.h>

/*
 * @name:
 * @msg: random initialize the matrix
 * @param {float} *data
 * @param {int} size
 * @return {*}
 */
void randomInit(float* data, int size) {
  long i;
  for (i = 0; i < size; ++i) {
    data[i] = (1.f * rand() / (float)RAND_MAX);
  }
}

/*
 * @name:
 * @msg: random initialize 2 matrix with the same value
 * @param {float*} data1
 * @param {float*} data2
 * @param {int} size
 * @return {*}
 */
void randomInit2(float* data1, float* data2, int size) {
  long i;
  for (i = 0; i < size; ++i) {
    data2[i] = data1[i] = rand() / (float)RAND_MAX;
  }
}

/*
 * @name:  InstantInit
 * @msg: use the integer to initialize the matrix
 * @param {float} *data
 * @param {int} size
 * @param {int} value
 * @return {*}
 */
void InstantInit(float* data, int size, float value) {
  long i;
  for (i = 0; i < size; ++i) {
    data[i] = value;
  }
}

/*
 * @name: print the matrix row major
 * @msg:
 * @param {void} *A
 * @param {int} M
 * @param {int} N
 * @param {int} lda
 * @param {char} *arg
 * @return {*}
 */
void printFloat1(void* A, int row, int col, int lda, const char* arg) {
  float* ptrA = (float*)A;
  printf("%s :\n", arg);
  int offset = (lda - row);
  for (int i = 0; i < row; i++) {
    printf("row %d : ", i);
    for (int j = 0; j < col; j++) {
      printf("(%d,%d, %lf) ", i, j, *(ptrA + i * lda + j));
    }
    ptrA += offset;
    printf("\n");
  }
  printf("\n");
}

/*
 * @name: print the matrix
 * @msg:
 * @param {void} *A
 * @param {int} M
 * @param {int} N
 * @param {int} lda
 * @param {char} *arg
 * @return {*}
 */
void printFloat(void* A, int row, int col, int lda, const char* arg) {
  float* ptrA = (float*)A;
  printf("%s :\n", arg);
  int offset = (lda - row);
  for (int i = 0; i < row; i++) {
    printf("row %d : ", i);
    for (int j = 0; j < col; j++) {
      // printf("(%d,%d, %lf) ", i, j, *(ptrA + j * lda + i));
      printf(" (%.5f)", *(ptrA + j * lda + i));
    }
    ptrA += offset;
    printf("\n");
  }
  printf("\n");
}

void printDiff(float* data1, float* data2, long width, long height, int iListLength, float fListTol) {
  printf("Listing first %d Differences > %.6f...\n", iListLength, fListTol);
  long i, j, k;
  int  error_count = 0;

  for (i = 0; i < width; i++) {

    for (j = 0; j < height; j++) {
      k           = i * height + j;
      float fDiff = fabs(data1[k] - data2[k]);

      if (fDiff > fListTol) {
        if (error_count < iListLength) {
          printf("    Loc(%d,%d)\tCPU=%.5f\tBLAS=%.5f\tDiff=%.6f\n", (int)j, (int)i, data1[k], data2[k], fDiff);
        }
        error_count++;
      }
    }
  }

  printf(" \n  Total Errors = %d\n", error_count);
}

float* malloc_aligned(int m, int n, int size) {
  const int SIMD_ALIGN = svcntb();
  float*    ptr;
  int       err;

  err = posix_memalign((void**)&ptr, (size_t)SIMD_ALIGN, size * m * n);

  if (err) {
    printf("bl_malloc_aligned(): posix_memalign() failures");
    exit(1);
  }

  return ptr;
}
