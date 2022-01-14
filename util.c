#include "gemm.h"


/*
 * @name:
 * @msg: random initialize the matrix
 * @param {VALUE_TYPE} *data
 * @param {int} size
 * @return {*}
 */
void randomInit(MATRIX_TYPE data, int size) {
  long i;
  for (i = 0; i < size; ++i) {
    data[i] = rand() / (float)RAND_MAX;
  }
}

/*
 * @name:
 * @msg: random initialize 2 matrix with the same value
 * @param {MATRIX_TYPE} data1
 * @param {MATRIX_TYPE} data2
 * @param {int} size
 * @return {*}
 */
void randomInit2(MATRIX_TYPE data1, MATRIX_TYPE data2, int size) {
  long i;
  for (i = 0; i < size; ++i) {
    data2[i] = data1[i] = rand() / (float)RAND_MAX;
  }
}

/*
 * @name:  integerInit
 * @msg: use the integer to initialize the matrix
 * @param {VALUE_TYPE} *data
 * @param {int} size
 * @param {int} value
 * @return {*}
 */
void integerInit(MATRIX_TYPE data, int size, VALUE_TYPE value) {
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
  VALUE_TYPE* ptrA = (VALUE_TYPE*)A;
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
  VALUE_TYPE* ptrA = (VALUE_TYPE*)A;
  printf("%s :\n", arg);
  int offset = (lda - row);
  for (int i = 0; i < row; i++) {
    printf("row %d : ", i);
    for (int j = 0; j < col; j++) {
      printf("(%d,%d, %lf) ", i, j, *(ptrA + j * lda + i));
    }
    ptrA += offset;
    printf("\n");
  }
  printf("\n");
}