/*
 * @Author       : Realtyxxx
 * @Descripttion : SVE_GEMM
 * @version      : 1.0
 * @Date         : 2022-01-11 19:30:51
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-03-18 19:49:28
 * @FilePath     : /sve/sve_gemm/gemm.h
 * @ToDo         :
 */

#include <math.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#pragma once

// macro function
#define min(a, b) ((a) < (b) ? (a) : (b))

// col major store order
#define A(i, j) a[(j)*lda + (i)]  // A : m x k   lda = m;
#define B(i, j) b[(j)*ldb + (i)]  // B : k x n   ldb = k;
#define C(i, j) c[(j)*ldc + (i)]  // C : m x n   ldc = m;

// define block_size
#define mc 32
#define kc 64
#define nc 64

typedef double      VALUE_TYPE;
typedef VALUE_TYPE* VALUE_PTR;
typedef int         DIM_TYPE;
typedef DIM_TYPE*   DIM_PTR;
typedef VALUE_TYPE* MATRIX_TYPE;

// clang-format off
typedef enum CBLAS_ORDER { CblasRowMajor = 101, CblasColMajor = 102 } CBLAS_ORDER;

typedef enum CBLAS_TRANSPOSE {
  CblasNoTrans     = 111,
  CblasTrans       = 112,
  CblasConjTrans   = 113,
  CblasConjNoTrans = 114
} CBLAS_TRANSPOSE;

void randomInit(MATRIX_TYPE data, int size);

void randomInit2(MATRIX_TYPE data1, MATRIX_TYPE data2, int size);

void InstantInit(MATRIX_TYPE data, int size, VALUE_TYPE value);

void printFloat1(void* A, int row, int col, int lda, const char* arg);

void printFloat(void* A, int row, int col, int lda, const char* arg);

void printDiff(MATRIX_TYPE data1, MATRIX_TYPE data2, long width, long height, int iListLength, float fListTol) ;

void add_dot_4x4_sve(int k, MATRIX_TYPE restrict a, int lda, MATRIX_TYPE restrict b, int ldb,
                     VALUE_PTR restrict alpha, MATRIX_TYPE restrict c, int ldc, VALUE_PTR restrict beta);

void add_dot_8x4_sve(int k, MATRIX_TYPE restrict a, int lda, MATRIX_TYPE restrict b, int ldb,
                     VALUE_PTR restrict alpha, MATRIX_TYPE restrict c, int ldc, VALUE_PTR restrict beta);

void naive_gemm(const CBLAS_ORDER ordej,
                const CBLAS_TRANSPOSE Atrans,
                const CBLAS_TRANSPOSE Btrans,
                const int M, const int N, const int K,
                const VALUE_TYPE alpha,
                const MATRIX_TYPE a, const int lda,
                const MATRIX_TYPE b, const int ldb,
                const VALUE_TYPE beta,
                MATRIX_TYPE c, const int ldc);


void my_dgemm(const CBLAS_ORDER order,
              const CBLAS_TRANSPOSE Atrans,
              const CBLAS_TRANSPOSE Btrans,
              const int M, const int N, const int K,
              const VALUE_TYPE alpha,
              const MATRIX_TYPE a, const int lda,
              const MATRIX_TYPE b, const int ldb,
              const VALUE_TYPE beta,
              MATRIX_TYPE c, const int ldc);

void my_dgemm_inside(const int M, const int N, const int K,
                     const VALUE_TYPE alpha,
                     const MATRIX_TYPE a, const int lda,
                     const MATRIX_TYPE b, const int ldb,
                     const VALUE_TYPE beta,
                     MATRIX_TYPE c, const int ldc,
                     bool first_time);
//clang-format on
