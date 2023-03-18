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
// #define MC 256
// #define NC 32
// #define KC 256

#define MR 32
#define NR 8
#define SIMD_ALIGN_SIZE 64

typedef float      VALUE_TYPE;
typedef VALUE_TYPE* VALUE_PTR;
typedef int         DIM_TYPE;
typedef DIM_TYPE*   DIM_PTR;
typedef float* MATRIX_TYPE;

// clang-format off
typedef enum BLAS_ORDER { blasRowMajor = 101, blasColMajor = 102 } BLAS_ORDER;

typedef enum BLAS_TRANSPOSE {
  blasNoTrans     = 111,
  blasTrans       = 112,
  blasConjTrans   = 113,
  blasConjNoTrans = 114
} BLAS_TRANSPOSE;

void randomInit(MATRIX_TYPE data, int size);

void randomInit2(MATRIX_TYPE data1, MATRIX_TYPE data2, int size);

void InstantInit(MATRIX_TYPE data, int size, VALUE_TYPE value);

void printFloat1(void* A, int row, int col, int lda, const char* arg);

void printFloat(void* A, int row, int col, int lda, const char* arg);

void printDiff(MATRIX_TYPE data1, MATRIX_TYPE data2, long width, long height, int iListLength, float fListTol) ;

void sgemm_armv8a_sve_asm_16x8(int k0, float* restrict alpha, float* restrict a, float* restrict b,
                                   float* restrict beta, float* restrict c, int rs_c0, int cs_c0, void* a_next,
                                   void* b_next);

void sgemm_armv8a_sve_asm_2vx8(int k0, float* restrict alpha, float* restrict a, float* restrict b,
                                   float* restrict beta, float* restrict c, int rs_c0, int cs_c0, void* a_next,
                                   void* b_next);

float *malloc_aligned(int m, int n, int size);

void naive_gemm(const BLAS_ORDER ordej,
                const BLAS_TRANSPOSE Atrans,
                const BLAS_TRANSPOSE Btrans,
                const int M, const int N, const int K,
                const VALUE_TYPE alpha,
                const MATRIX_TYPE a, const int lda,
                const MATRIX_TYPE b, const int ldb,
                const VALUE_TYPE beta,
                MATRIX_TYPE c, const int ldc);


void sgemm(const int M, const int N, const int K,
           const VALUE_TYPE alpha,
           const MATRIX_TYPE a, const int lda,
           const MATRIX_TYPE b, const int ldb,
           const VALUE_TYPE beta,
           MATRIX_TYPE c, const int ldc, int MC, int NC, int KC);

void my_dgemm_Macro(const int M, const int N, const int K,
                     const VALUE_TYPE alpha,
                     const MATRIX_TYPE a, const int lda,
                     const MATRIX_TYPE b, const int ldb,
                     const VALUE_TYPE beta,
                     MATRIX_TYPE c, const int ldc);
//clang-format on
