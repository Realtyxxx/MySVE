#include <stdint.h>
#include <stdio.h>
#include "sve_finc_s.h"

#ifdef __ARM_FEATURE_SVE
#include <arm_sve.h>
#endif

/* 2 vectors in m_r, n_r = 8
 */
void sgemm_armv8a_sve_asm_2vx8(int k0, float* restrict alpha, float* restrict a, float* restrict b,
                                   float* restrict beta, float* restrict c, int rs_c0, int cs_c0, void* a_next,
                                   void* b_next) {

  // 4 k iterations in unrolled loop
  uint64_t k_iter = k0 / 4;
  // rest is handled separately
  uint64_t k_left = k0 % 4;

  // Typecast local copies of integers in case dim_t and inc_t are a
  // different size than is expected by load instructions.

  uint64_t rs_c = rs_c0;
  uint64_t cs_c = cs_c0;

  __asm__ volatile
(
"                                            \n\t" 
" ldr x0,%[aaddr]                            \n\t" // Load address of A 
" ldr x1,%[baddr]                            \n\t" // Load address of B
" ldr x2,%[caddr]                            \n\t" // Load address of C
"                                            \n\t"
" ldr x3,%[a_next]                           \n\t" // Move pointer
" ldr x4,%[b_next]                           \n\t" // Move pointer
"                                            \n\t"
" ldr x5,%[k_iter]                           \n\t" // number of 4xk iterations
" ldr x6,%[k_left]                           \n\t" // number of k iterations afterward
"                                            \n\t" 
" ldr x7,%[alpha]                            \n\t" // Alpha address      
" ldr x8,%[beta]                             \n\t" // Beta address      
"                                            \n\t" 
" ldr x9,%[cs_c]                             \n\t" // Load cs_c
" lsl x10,x9,#2                              \n\t" // cs_c * sizeof(float)
"                                            \n\t"
" ldr x13,%[rs_c]                            \n\t" // Load rs_c.
//" lsl x14,x13,#3                             \n\t" // rs_c * sizeof(float). 
"                                            \n\t"
" mov x11,#0                                 \n\t"
" incw x11                                   \n\t"
"                                            \n\t"
" add x20,x2,x10                             \n\t" //Load address Column 1 of C
" add x21,x20,x10                            \n\t" //Load address Column 2 of C
" add x22,x21,x10                            \n\t" //Load address Column 3 of C
" add x23,x22,x10                            \n\t" //Load address Column 4 of C
" add x24,x23,x10                            \n\t" //Load address Column 5 of C
" add x25,x24,x10                            \n\t" //Load address Column 6 of C
" add x26,x25,x10                            \n\t" //Load address Column 7 of C
"                                            \n\t"
" prfm pldl1keep,[x2]                        \n\t" // Prefetch c.
" prfm pldl1keep,[x20]                       \n\t" // Prefetch c.
" prfm pldl1keep,[x21]                       \n\t" // Prefetch c.
" prfm pldl1keep,[x22]                       \n\t" // Prefetch c.
" prfm pldl1keep,[x23]                       \n\t" // Prefetch c.
" prfm pldl1keep,[x24]                       \n\t" // Prefetch c.
" prfm pldl1keep,[x25]                       \n\t" // Prefetch c.
" prfm pldl1keep,[x26]                       \n\t" // Prefetch c.
"                                            \n\t"
" ptrue p0.s                                 \n\t" // Creating all true predicate
"                                            \n\t"
LOAD2VEC_S(z0,z1,p0,x0) // load 2 vec
"                                            \n\t"
LOAD8VEC_DIST_S(z2,z3,z4,z5,z6,z7,z8,z9,p0,x1) // load broad cast 
"                                            \n\t"
"                                            \n\t"
ZERO4VEC_S(z10,z11,z12,z13)                          // c columns 0-1
" prfm PLDL1KEEP, [x1, #64]                  \n\t"
ZERO4VEC_S(z14,z15,z16,z17)                          // c columns 2-3
ZERO4VEC_S(z18,z19,z20,z21)                          // c columns 4-5
" prfd pldl1keep,p0, [x0, #2, MUL VL]        \n\t" // prefetch next a vector
ZERO4VEC_S(z22,z23,z24,z25)                          // c columns 6-7
" prfd pldl1keep,p0, [x0, #3, MUL VL]        \n\t" // prefetch next a vector
"                                            \n\t"
"                                            \n\t"
" cmp x5,#0                                  \n\t" // If k_iter == 0, jump to k_left.
" beq .S2VX8CONSIDERKLEFT                    \n\t"
"                                            \n\t"
" incb x0, ALL, MUL #2                       \n\t" // A = A+vecsize*2
" add x1, x1, #32                            \n\t" // B = B+8*sizeof(float)
"                                            \n\t"
" cmp x5,1                                   \n\t" // If there is just one k_iter, jump to that one. 
" beq .S2VX8LASTITER                         \n\t" // (as loop is do-while-like).
"                                            \n\t" 
"                                            \n\t"
" .S2VX8LOOP:                                \n\t" // Body
MLA2ROW_LA_LB_S(z10, z11, z0, z1, z2, p0, z26, x0,0,x1,0) 
" prfd pldl1keep,p0, [x0, #2, MUL VL]        \n\t" // prefetch next a vector
MLA2ROW_LA_LB_S(z12, z13, z0, z1, z3, p0, z27, x0,1,x1,4) 
" prfd pldl1keep,p0, [x0, #3, MUL VL]        \n\t" // prefetch next a vector
MLA2ROW_LB_S(z14, z15, z0, z1, z4, p0, x1,8)
" prfd pldl1keep,p0, [x0, #4, MUL VL]        \n\t" // prefetch next a vector
MLA2ROW_LB_S(z16, z17, z0, z1, z5, p0, x1,12)
" prfd pldl1keep,p0, [x0, #5, MUL VL]        \n\t" // prefetch next a vector
MLA2ROW_LB_S(z18, z19, z0, z1, z6, p0, x1,16)
" prfd pldl1keep,p0, [x0, #6, MUL VL]        \n\t" // prefetch next a vector
MLA2ROW_LB_S(z20, z21, z0, z1, z7, p0, x1,20)
" prfd pldl1keep,p0, [x0, #7, MUL VL]        \n\t" // prefetch next a vector
MLA2ROW_LB_S(z22, z23, z0, z1, z8, p0, x1,24)
" prfd pldl1keep,p0, [x0, #8, MUL VL]        \n\t" // prefetch next a vector
MLA2ROW_LB_S(z24, z25, z0, z1, z9, p0, x1,28)
" prfd pldl1keep,p0, [x0, #9, MUL VL]        \n\t" // prefetch next a vector
"                                            \n\t"
MLA2ROW_LA_LB_S(z10, z11, z26, z27, z2, p0, z0, x0,2,x1,32) 
MLA2ROW_LA_LB_S(z12, z13, z26, z27, z3, p0, z1, x0,3,x1,36) 
" prfm PLDL1KEEP, [x1, #96]                 \n\t"
MLA2ROW_LB_S(z14, z15, z26, z27, z4, p0, x1,40)
MLA2ROW_LB_S(z16, z17, z26, z27, z5, p0, x1,44)
" prfm PLDL1KEEP, [x1, #160]                 \n\t"
MLA2ROW_LB_S(z18, z19, z26, z27, z6, p0, x1,48)
MLA2ROW_LB_S(z20, z21, z26, z27, z7, p0, x1,52)
MLA2ROW_LB_S(z22, z23, z26, z27, z8, p0, x1,56)
MLA2ROW_LB_S(z24, z25, z26, z27, z9, p0, x1,60)
" incb x0, ALL, MUL #4                       \n\t" // Next 4 A vectors
MLA2ROW_LA_LB_S(z10, z11, z0, z1, z2, p0, z26, x0,0,x1,64) 
MLA2ROW_LA_LB_S(z12, z13, z0, z1, z3, p0, z27, x0,1,x1,68) 
MLA2ROW_LB_S(z14, z15, z0, z1, z4, p0, x1,72)
MLA2ROW_LB_S(z16, z17, z0, z1, z5, p0, x1,76)
MLA2ROW_LB_S(z18, z19, z0, z1, z6, p0, x1,80)
MLA2ROW_LB_S(z20, z21, z0, z1, z7, p0, x1,84)
MLA2ROW_LB_S(z22, z23, z0, z1, z8, p0, x1,88)
MLA2ROW_LB_S(z24, z25, z0, z1, z9, p0, x1,92)
"                                            \n\t"
MLA2ROW_LA_LB_S(z10, z11, z26, z27, z2, p0, z0, x0,2,x1,96) 
MLA2ROW_LA_LB_S(z12, z13, z26, z27, z3, p0, z1, x0,3,x1,100) 
MLA2ROW_LB_S(z14, z15, z26, z27, z4, p0, x1,104)
MLA2ROW_LB_S(z16, z17, z26, z27, z5, p0, x1,108)
MLA2ROW_LB_S(z18, z19, z26, z27, z6, p0, x1,112)
MLA2ROW_LB_S(z20, z21, z26, z27, z7, p0, x1,116)
MLA2ROW_LB_S(z22, z23, z26, z27, z8, p0, x1,120)
MLA2ROW_LB_S(z24, z25, z26, z27, z9, p0, x1,124)
" incb x0, ALL, MUL #4                       \n\t" // Next 4 A vectors
" add x1, x1, #128                           \n\t" // B = B+4*8*sizeof(float)
"                                            \n\t"
" sub x5,x5,1                                \n\t" // i-=1
" cmp x5,1                                   \n\t" // Iterate again if we are not in k_iter == 1.
" bne .S2VX8LOOP                             \n\t"
" .S2VX8LASTITER:                            \n\t" // Body
MLA2ROW_LA_LB_S(z10, z11, z0, z1, z2, p0, z26, x0,0,x1,0)
" prfd pldl1keep,p0, [x0, #2, MUL VL]        \n\t" // prefetch next a vector
MLA2ROW_LA_LB_S(z12, z13, z0, z1, z3, p0, z27, x0,1,x1,4)
" prfd pldl1keep,p0, [x0, #3, MUL VL]        \n\t" // prefetch next a vector
MLA2ROW_LB_S(z14, z15, z0, z1, z4, p0, x1,8)
" prfd pldl1keep,p0, [x0, #4, MUL VL]        \n\t" // prefetch next a vector
MLA2ROW_LB_S(z16, z17, z0, z1, z5, p0, x1,12)
" prfd pldl1keep,p0, [x0, #5, MUL VL]        \n\t" // prefetch next a vector
MLA2ROW_LB_S(z18, z19, z0, z1, z6, p0, x1,16)
MLA2ROW_LB_S(z20, z21, z0, z1, z7, p0, x1,20)
MLA2ROW_LB_S(z22, z23, z0, z1, z8, p0, x1,24)
MLA2ROW_LB_S(z24, z25, z0, z1, z9, p0, x1,28)
"                                            \n\t"
MLA2ROW_LA_LB_S(z10, z11, z26, z27, z2, p0, z0, x0,2,x1,32)
MLA2ROW_LA_LB_S(z12, z13, z26, z27, z3, p0, z1, x0,3,x1,36)
MLA2ROW_LB_S(z14, z15, z26, z27, z4, p0, x1,40)
MLA2ROW_LB_S(z16, z17, z26, z27, z5, p0, x1,44)
MLA2ROW_LB_S(z18, z19, z26, z27, z6, p0, x1,48)
MLA2ROW_LB_S(z20, z21, z26, z27, z7, p0, x1,52)
MLA2ROW_LB_S(z22, z23, z26, z27, z8, p0, x1,56)
MLA2ROW_LB_S(z24, z25, z26, z27, z9, p0, x1,60)
"                                            \n\t"
MLA2ROW_LA_LB_S(z10, z11, z0, z1, z2, p0, z26, x0,4,x1,64)
MLA2ROW_LA_LB_S(z12, z13, z0, z1, z3, p0, z27, x0,5,x1,68)
MLA2ROW_LB_S(z14, z15, z0, z1, z4, p0, x1,72)
MLA2ROW_LB_S(z16, z17, z0, z1, z5, p0, x1,76)
MLA2ROW_LB_S(z18, z19, z0, z1, z6, p0, x1,80)
MLA2ROW_LB_S(z20, z21, z0, z1, z7, p0, x1,84)
MLA2ROW_LB_S(z22, z23, z0, z1, z8, p0, x1,88)
MLA2ROW_LB_S(z24, z25, z0, z1, z9, p0, x1,92)
"                                            \n\t"
MLA2ROW_S(z10, z11, z26, z27, z2, p0)
MLA2ROW_S(z12, z13, z26, z27, z3, p0)
MLA2ROW_S(z14, z15, z26, z27, z4, p0)
MLA2ROW_S(z16, z17, z26, z27, z5, p0)
MLA2ROW_S(z18, z19, z26, z27, z6, p0)
MLA2ROW_S(z20, z21, z26, z27, z7, p0)
MLA2ROW_S(z22, z23, z26, z27, z8, p0)
MLA2ROW_S(z24, z25, z26, z27, z9, p0)
" incb x0, ALL, MUL #6                       \n\t" // 6 Vectors loaded
" add x1, x1, #96                            \n\t" // B = B+3*8*sizeof(float)
"                                            \n\t"
" .S2VX8CONSIDERKLEFT:                       \n\t" 
" cmp x6,0                                   \n\t" // If k_left == 0, we are done.
" beq .S2VX8POSTACCUM                        \n\t" // else, we enter the k_left loop.
"                                            \n\t"
".S2VX8LOOPKLEFT:                            \n\t"
"                                            \n\t"
LOAD2VEC_S(z0,z1,p0,x0)
" incb x0, ALL, MUL #2                       \n\t" // Advance a pointer by 2 vectors
"                                            \n\t"
LOAD8VEC_DIST_S(z2,z3,z4,z5,z6,z7,z8,z9,p0,x1)
" add x1, x1, #32                            \n\t" // advance b pointer by 8 floats
"                                            \n\t"
" sub x6,x6,1                                \n\t"
"                                            \n\t"
MLA2ROW_S(z10,z11,z0,z1,z2,p0)
MLA2ROW_S(z12,z13,z0,z1,z3,p0)
MLA2ROW_S(z14,z15,z0,z1,z4,p0)
MLA2ROW_S(z16,z17,z0,z1,z5,p0)
MLA2ROW_S(z18,z19,z0,z1,z6,p0)
MLA2ROW_S(z20,z21,z0,z1,z7,p0)
MLA2ROW_S(z22,z23,z0,z1,z8,p0)
MLA2ROW_S(z24,z25,z0,z1,z9,p0)
"                                            \n\t"
" cmp x6,0                                   \n\t" // Iterate again.
" bne .S2VX8LOOPKLEFT                        \n\t" // if i!=0.
"                                            \n\t"
" .S2VX8POSTACCUM:                           \n\t"
" prfm PLDL2KEEP, [x3]                       \n\t"
" prfm PLDL2KEEP, [x4]                       \n\t"
"                                            \n\t"
" ld1rw  z29.s, p0/z, [x7]                   \n\t" // Load alpha
" ld1rw  z30.s, p0/z, [x8]                   \n\t" // Load beta
"                                            \n\t"
" cmp x13,#1                                 \n\t" // If rs_c != 1 (column-major)
" bne .S2VX8GENSTORED                        \n\t"
"                                            \n\t"
" .S2VX8COLSTORED:                           \n\t" // C is column-major.
"                                            \n\t"
//FINC_2COL(2VX8,CONT,z0,z1,z2,z3,     x2,x20,  no,no, 29, 30, z10,z11,z12,z13,1)
//FINC_2COL(2VX8,CONT,z4,z5,z6,z7,     x21,x22, no,no, 29, 30, z14,z15,z16,z17,2)
//FINC_2COL(2VX8,CONT,z8,z9,z10,z11,   x23,x24, no,no, 29, 30, z18,z19,z20,z21,3)
//FINC_2COL(2VX8,CONT,z12,z13,z14,z15, x25,x26, no,no, 29, 30, z22,z23,z24,z25,4)
FINC_4COL(2VX8,CONT_S, z0,z1,z2,z3,z4,z5,z6,z7, x2,x20,x21,x22, no,no, 29,30, z10,z11,z12,z13,z14,z15,z16,z17, 1)
FINC_4COL(2VX8,CONT_S, z8,z9,z10,z11,z12,z13,z14,z15, x23,x24,x25,x26, no,no, 29,30, z18,z19,z20,z21,z22,z23,z24,z25, 2)
"                                            \n\t"
" b .S2VX8END                                \n\t"
"                                            \n\t"
" .S2VX8GENSTORED:                           \n\t" // C is general-stride stored.
"                                            \n\t" // Creating index for stride load&store access
" index z8.s, wzr, w13                       \n\t" // 0, stride*float, 2*stride*float, ...
" mul x3, x13, x11                           \n\t" // x3 <- stride*float*vecsize
" index z9.s, w3, w13                        \n\t" // stride*float*vecsize, (vecsize+1)*stride*float, (vecsize+2)*stride*float, ...
"                                            \n\t"
//FINC_2COL(2VX8,GENI,z0,z1,z2,z3,     x2,x20,  z8,z9, 29, 30, z10,z11,z12,z13,1)
//FINC_2COL(2VX8,GENI,z4,z5,z6,z7,     x21,x22, z8,z9, 29, 30, z14,z15,z16,z17,2)
//FINC_2COL(2VX8,GENI,z10,z11,z12,z13, x23,x24, z8,z9, 29, 30, z18,z19,z20,z21,3)
//FINC_2COL(2VX8,GENI,z14,z15,z16,z17, x25,x26, z8,z9, 29, 30, z22,z23,z24,z25,4)
FINC_4COL(2VX8,GENI_S, z0,z1,z2,z3,z4,z5,z6,z7, x2,x20,x21,x22, z8,z9, 29,30, z10,z11,z12,z13,z14,z15,z16,z17, 1)
FINC_4COL(2VX8,GENI_S, z10,z11,z12,z13,z14,z15,z16,z17, x23,x24,x25,x26, z8,z9, 29,30, z18,z19,z20,z21,z22,z23,z24,z25, 2)
"                                            \n\t"
" .S2VX8END:                                 \n\t" // Done!
"                                            \n\t"
:// output operands (none)
:// input operands
 [aaddr]  "m" (a),      // 0
 [baddr]  "m" (b),      // 1
 [caddr]  "m" (c),      // 2
 [k_iter] "m" (k_iter), // 3
 [k_left] "m" (k_left), // 4
 [alpha]  "m" (alpha),  // 5
 [beta]   "m" (beta),   // 6
 [rs_c]   "m" (rs_c),   // 6
 [cs_c]   "m" (cs_c),   // 7
 [a_next] "m" (a_next), // 8
 [b_next] "m" (b_next)  // 9
:// Register clobber list
 "x0","x1","x2","x3",
 "x4","x5","x6",
 "x7","x8","x9",
 "x10","x11","x13",
 "x20","x21","x22","x23","x24","x25","x26",
 "z0","z1","z2",
 "z3","z4","z5",
 "z6","z7","z8",
 "z9","z10","z11",
 "z12","z13","z14",
 "z15","z16","z17","z18","z19",
 "z20","z21","z22","z23",
 "z24","z25","z26","z27",
 "z28","z29","z30","z31", 
 "p0"
);
}
