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
#define MC 32
#define NC 80
#define KC 32

#define MR 32
#define NR 8
#define SIMD_ALIGN_SIZE 32

typedef float       VALUE_TYPE;
typedef VALUE_TYPE* VALUE_PTR;
typedef int         DIM_TYPE;
typedef DIM_TYPE*   DIM_PTR;
typedef float*      MATRIX_TYPE;

void sgemm_armv8a_sve_asm_2vx8(int k0, float* alpha, float* a, float* b,
                               float* beta, float* c, int rs_c0, int cs_c0,
                               void* a_next, void* b_next);

float* malloc_aligned(int m, int n, int size);

void sgemm(const int M, const int N, const int K, const VALUE_TYPE alpha,
           const MATRIX_TYPE a, const int lda, const MATRIX_TYPE b,
           const int ldb, const VALUE_TYPE beta, MATRIX_TYPE c, const int ldc);

void my_dgemm_Macro(const int M, const int N, const int K,
                    const VALUE_TYPE alpha, const MATRIX_TYPE a, const int lda,
                    const MATRIX_TYPE b, const int ldb, const VALUE_TYPE beta,
                    MATRIX_TYPE c, const int ldc);
//clang-format on
