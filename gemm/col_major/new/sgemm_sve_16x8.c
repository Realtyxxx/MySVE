#include <stdint.h>
#include <stdio.h>

#ifdef __ARM_FEATURE_SVE
#include <arm_sve.h>
#endif

void sgemm_armv8a_sve_asm_16x8(int k0, float* restrict alpha, float* restrict a, float* restrict b,
                               float* restrict beta, float* restrict c, int rs_c0, int cs_c0, void* a_next,
                               void* b_next) {
#ifndef __ARM_FEATURE_SVE
#warn "the kernel returned cause there has no sve"
  return;
#endif

  // safety check
  if (svcntb() != 64) {
    fprintf(stderr, "Wrong SVE vector length! You compiled for vector length of 256bits, but your length is %ld.\n",
            svcntb() * 8);
    return;
  }

  // TYX_NOTES: only used for 256 bits also means 8 floats

  // void* a_next = bli_auxinfo_next_a(data);
  // void* b_next = bli_auxinfo_next_b(data);
  // Typecast local copies of integers in case dim_t and inc_t are a
  // different size than is expected by load instructions.
  uint64_t k_iter = k0 / 4;
  uint64_t k_left = k0 % 4;
  uint64_t rs_c   = rs_c0;
  uint64_t cs_c   = cs_c0;

  __asm__ volatile(
      "                                            \n\t"
      " ldr x0,%[aaddr]                            \n\t"  // Load address of A
      " ldr x1,%[baddr]                            \n\t"  // Load address of B
      " ldr x2,%[caddr]                            \n\t"  // Load address of C
      "                                            \n\t"
      " ldr w3,%[a_next]                           \n\t"  // Move pointer
      " ldr x4,%[b_next]                           \n\t"  // Move pointer
      "                                            \n\t"
      " ldr x5,%[k_iter]                           \n\t"  // Init guard (k_iter)
      " ldr x6,%[k_left]                           \n\t"  // Init guard (k_iter)
      "                                            \n\t"
      // Moved to where the values are loaded and reusing x0 and x1
      //" ldr x7,%[alpha]                            \n\t" // Alpha address
      //" ldr x8,%[beta]                             \n\t" // Beta address
      "                                            \n\t"
      " ldr x10,%[cs_c]                             \n\t"  // Load cs_c
      " lsl x10,x10,#2                              \n\t"  // cs_c * sizeof(single)
      "                                            \n\t"
      " ldr w13,%[rs_c]                            \n\t"  // Load rs_c.
      //" lsl x14,x13,#2                             \n\t" // rs_c * sizeof(single).
      "                                            \n\t"
      " mov w11, #8                                \n\t"
      "                                            \n\t"
      " add x20,x2,x10                             \n\t"  // Load address Column 1 of C
      " add x21,x20,x10                            \n\t"  // Load address Column 2 of C
      " add x22,x21,x10                            \n\t"  // Load address Column 3 of C
      " add x23,x22,x10                            \n\t"  // Load address Column 4 of C
      " add x24,x23,x10                            \n\t"  // Load address Column 5 of C
      " add x25,x24,x10                            \n\t"  // Load address Column 6 of C
      " add x26,x25,x10                            \n\t"  // Load address Column 7 of C
      "                                            \n\t"
      " prfm pldl1keep,[x2]                        \n\t"  // Prefetch c.
      " prfm pldl1keep,[x20]                       \n\t"  // Prefetch c.
      " prfm pldl1keep,[x21]                       \n\t"  // Prefetch c.
      " prfm pldl1keep,[x22]                       \n\t"  // Prefetch c.
      " prfm pldl1keep,[x23]                       \n\t"  // Prefetch c.
      " prfm pldl1keep,[x24]                       \n\t"  // Prefetch c.
      " prfm pldl1keep,[x25]                       \n\t"  // Prefetch c.
      " prfm pldl1keep,[x26]                       \n\t"  // Prefetch c.
      "                                            \n\t"
      " ptrue p0.s                                 \n\t"  // Creating all true predicate //TYX_NOTES: 8 float one time
      "                                            \n\t"
      " ld1w  z0.s, p0/z, [x0]                     \n\t"  // Load a
      " ld1w  z1.s, p0/z, [x0, #1, MUL VL]         \n\t"
      "                                            \n\t"
      " ld1rw  z2.s, p0/z, [x1]                    \n\t"  // Load b
      " ld1rw  z3.s, p0/z, [x1, #4]                \n\t"
      " ld1rw  z4.s, p0/z, [x1, #8]                \n\t"
      " ld1rw  z5.s, p0/z, [x1, #12]               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #16]               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #20]               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #24]               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #28]               \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " dup z12.s, #0                              \n\t"  // Vector for accummulating column 0
      " dup z13.s, #0                              \n\t"  // Vector for accummulating column 0
      " dup z14.s, #0                              \n\t"  // Vector for accummulating column 1
      " dup z15.s, #0                              \n\t"  // Vector for accummulating column 1
      " prfm PLDL1KEEP, [x1, #128]                 \n\t"
      " dup z16.s, #0                              \n\t"  // Vector for accummulating column 2
      " prfm PLDL1KEEP, [x1, #192]                 \n\t"
      "                                            \n\t"
      " dup z17.s, #0                              \n\t"  // Vector for accummulating column 2
      " dup z18.s, #0                              \n\t"  // Vector for accummulating column 3
      " dup z19.s, #0                              \n\t"  // Vector for accummulating column 3
      "                                            \n\t"
      " prfm PLDL1KEEP, [x0, #256]                 \n\t"
      "                                            \n\t"
      " dup z20.s, #0                              \n\t"  // Vector for accummulating column 4
      " prfm PLDL1KEEP, [x0, #320]                 \n\t"
      " dup z21.s, #0                              \n\t"  // Vector for accummulating column 4
      " prfm PLDL1KEEP, [x0, #384]                \n\t"
      " dup z22.s, #0                              \n\t"  // Vector for accummulating column 5
      "                                            \n\t"
      " dup z23.s, #0                              \n\t"  // Vector for accummulating column 5
      " prfm PLDL1KEEP, [x0, #448]                \n\t"
      " dup z24.s, #0                              \n\t"  // Vector for accummulating column 6
      " dup z25.s, #0                              \n\t"  // Vector for accummulating column 6
      "                                            \n\t"
      " dup z26.s, #0                              \n\t"  // Vector for accummulating column 7
      " dup z27.s, #0                              \n\t"  // Vector for accummulating column 7
      "                                            \n\t"
      " cmp x5,#0                                  \n\t"  // If k_iter == 0, jump to k_left.
      " beq .S256CONSIDERKLEFT                     \n\t"
      "                                            \n\t"
      " add x0, x0, #64                            \n\t"  // update address of A
      " add x1, x1, #32                            \n\t"  // update address of B
      "                                            \n\t"
      " cmp x5,1                                   \n\t"  // If there is just one k_iter, jump to that one.
      " beq .S256LASTITER                          \n\t"  // (as loop is do-while-like).
      "                                            \n\t"
      " S256LOOP:                                  \n\t"  // Body
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " prfm PLDL1KEEP, [x1, #224]                 \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      " prfm PLDL1KEEP, [x1, #288]                 \n\t"
      " ld1rw  z2.s, p0/z, [x1]                    \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      " ld1rw  z3.s, p0/z, [x1, #4]                \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      " ld1rw  z4.s, p0/z, [x1, #8]                \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #12]               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #16]               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " prfm PLDL1KEEP, [x0, #448]                \n\t"  // 448 + 64 -64
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      " prfm PLDL1KEEP, [x0, #512]                \n\t"
      " ld1rw  z7.s, p0/z, [x1, #20]               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " prfm PLDL1KEEP, [x0, #576]                \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      " prfm PLDL1KEEP, [x0, #640]                \n\t"
      " ld1rw  z8.s, p0/z, [x1, #24]               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #28]               \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " ld1w   z0.s, p0/z, [x0]                    \n\t"
      " ld1w   z1.s, p0/z, [x0, #1, MUL VL]        \n\t"
      // " ld1w   z1.s, p0/z, [x0, #1, LSL #3]        \n\t"
      "                                            \n\t"  // End it 1.
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      " ld1rw  z2.s, p0/z, [x1, #32]               \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      " ld1rw  z3.s, p0/z, [x1, #36]               \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      " ld1rw  z4.s, p0/z, [x1, #40]               \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #44]               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #48]               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #52]               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #56]               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #60]              \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " ld1w   z0.s, p0/z, [x0, #2, MUL VL]        \n\t"
      " ld1w   z1.s, p0/z, [x0, #3, MUL VL]        \n\t"
      // " ld1w   z0.s, p0/z, [x0, #2, LSL #3]        \n\t"
      // " ld1w   z1.s, p0/z, [x0, #3, LSL #3]        \n\t"
      "                                            \n\t"  // End it 2.
      "                                            \n\t"
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      " ld1rw  z2.s, p0/z, [x1, #64]               \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      " ld1rw  z3.s, p0/z, [x1, #68]               \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      " ld1rw  z4.s, p0/z, [x1, #72]               \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #76]               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #80]               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #84]               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #88]               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #92]              \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " ld1w   z0.s, p0/z, [x0, #4, MUL VL]        \n\t"
      " ld1w   z0.s, p0/z, [x0, #5, MUL VL]        \n\t"
      // " ld1w   z1.s, p0/z, [x0, #4, LSL #3]        \n\t"
      // " ld1w   z1.s, p0/z, [x0, #5, LSL #3]        \n\t"
      "                                            \n\t"  // End it 3.
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      " ld1rw  z2.s, p0/z, [x1, #96]               \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      " ld1rw  z3.s, p0/z, [x1, #100]               \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      " ld1rw  z4.s, p0/z, [x1, #104]               \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #108]               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #112]               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #116]               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #120]               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #124]              \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " ld1w   z0.s, p0/z, [x0, #6, MUL VL]        \n\t"
      " ld1w   z1.s, p0/z, [x0, #7, MUL VL]        \n\t"
      // " ld1w   z0.s, p0/z, [x0, #6, LSL #3]        \n\t"
      // " ld1w   z1.s, p0/z, [x0, #7, LSL #3]        \n\t"
      "                                            \n\t"
      "                                            \n\t"  // End it 4.
      "                                            \n\t"
      " add x0, x0, #256                           \n\t"  // incremenenting by 256 {64 floats = 2*[8 vec length] * 4
                                                          // unroll}
      " add x1, x1, #128                           \n\t"  //                       {32 floats = [8 broadcast vec]* 4
                                                          //                       unroll}
      "                                            \n\t"
      " sub x5,x5,1                                \n\t"  // i-=1
      " cmp x5,1                                   \n\t"  // Iterate again if we are not in k_iter == 1.
      " bne S256LOOP                               \n\t"
      "                                            \n\t"
      " .S256LASTITER:                             \n\t"  //                       last iter no need to prefetch
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      " ld1rw  z2.s, p0/z, [x1]                    \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      " ld1rw  z3.s, p0/z, [x1, #4]                \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      " ld1rw  z4.s, p0/z, [x1, #8]                \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #12]               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #16]               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #20]               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #24]               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #28]               \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " ld1w   z0.s, p0/z, [x0]                    \n\t"
      " ld1w   z1.s, p0/z, [x0, #1, MUL VL]        \n\t"
      "                                            \n\t"  // End it 1.
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      " ld1rw  z2.s, p0/z, [x1, #32]               \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      " ld1rw  z3.s, p0/z, [x1, #36]               \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      " ld1rw  z4.s, p0/z, [x1, #40]               \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #44]               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #48]               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #52]               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #56]               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #60]              \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " ld1w   z0.s, p0/z, [x0, #2, MUL VL]        \n\t"
      " ld1w   z1.s, p0/z, [x0, #3, MUL VL]        \n\t"
      "                                            \n\t"  // End it 2.
      "                                            \n\t"
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      " ld1rw  z2.s, p0/z, [x1, #64]               \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      " ld1rw  z3.s, p0/z, [x1, #68]               \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      " ld1rw  z4.s, p0/z, [x1, #72]               \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #76]               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #80]               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #84]               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #88]               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #92]              \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " ld1w   z0.s, p0/z, [x0, #4, MUL VL]        \n\t"
      " ld1w   z1.s, p0/z, [x0, #5, MUL VL]        \n\t"
      "                                            \n\t"  // End it 3.
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"  // End it 4.
      "                                            \n\t"
      " add x0, x0, #192                           \n\t"  // incremenenting by 256*3/4
                                                          // 前面已经加了一次，而且已经ld好了B的
      " add x1, x1, #96                           \n\t"   // incremenenting by 128*3/4
      "                                            \n\t"
      " .S256CONSIDERKLEFT:                        \n\t"
      " cmp x6,0                                   \n\t"  // If k_left == 0, we are done.
      " beq .S256POSTACCUM                         \n\t"  // else, we enter the k_left loop.
      "                                            \n\t"
      ".S256LOOPKLEFT:                             \n\t"
      "                                            \n\t"
      " ld1w  z0.s, p0/z, [x0]                     \n\t"  // Load a
      " ld1w  z1.s, p0/z, [x0, #1, MUL VL]         \n\t"
      " add x0, x0, #64                            \n\t"
      "                                            \n\t"
      " ld1rw  z2.s, p0/z, [x1]                    \n\t"  // Load b
      " ld1rw  z3.s, p0/z, [x1, #4]                \n\t"
      " ld1rw  z4.s, p0/z, [x1, #8]               \n\t"
      " ld1rw  z5.s, p0/z, [x1, #12]               \n\t"
      " ld1rw  z6.s, p0/z, [x1, #16]               \n\t"
      " ld1rw  z7.s, p0/z, [x1, #20]               \n\t"
      " ld1rw  z8.s, p0/z, [x1, #24]               \n\t"
      " ld1rw  z9.s, p0/z, [x1, #28]               \n\t"
      " add x1, x1, #32                            \n\t"
      "                                            \n\t"
      " sub x6,x6,1                                \n\t"
      "                                            \n\t"
      " fmla z12.s, p0/m, z0.s, z2.s               \n\t"
      " fmla z13.s, p0/m, z1.s, z2.s               \n\t"
      "                                            \n\t"
      " fmla z14.s, p0/m, z0.s, z3.s               \n\t"
      " fmla z15.s, p0/m, z1.s, z3.s               \n\t"
      "                                            \n\t"
      " fmla z16.s, p0/m, z0.s, z4.s               \n\t"
      " fmla z17.s, p0/m, z1.s, z4.s               \n\t"
      "                                            \n\t"
      " fmla z18.s, p0/m, z0.s, z5.s               \n\t"
      " fmla z19.s, p0/m, z1.s, z5.s               \n\t"
      "                                            \n\t"
      " fmla z20.s, p0/m, z0.s, z6.s               \n\t"
      " fmla z21.s, p0/m, z1.s, z6.s               \n\t"
      "                                            \n\t"
      " fmla z22.s, p0/m, z0.s, z7.s               \n\t"
      " fmla z23.s, p0/m, z1.s, z7.s               \n\t"
      "                                            \n\t"
      " fmla z24.s, p0/m, z0.s, z8.s               \n\t"
      " fmla z25.s, p0/m, z1.s, z8.s               \n\t"
      "                                            \n\t"
      " fmla z26.s, p0/m, z0.s, z9.s               \n\t"
      " fmla z27.s, p0/m, z1.s, z9.s               \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " cmp x6,0                                   \n\t"  // Iterate again.
      " bne .S256LOOPKLEFT                         \n\t"  // if i!=0.
      "                                            \n\t"
      " .S256POSTACCUM:                            \n\t"
      "                                            \n\t"
      " ldr x0,%[alpha]                            \n\t"  // Alpha address
      " ldr x1,%[beta]                             \n\t"  // Beta address
      " ld1rw  z8.s, p0/z, [x0]                    \n\t"  // Load alpha
      " ld1rw  z9.s, p0/z, [x1]                    \n\t"  // Load beta
      "                                            \n\t"
      " cmp w13,#1                                 \n\t"  // If rs_c != 1 (column-major)
      " bne .S256GENSTORED                         \n\t"
      "                                            \n\t"
      " .S256COLSTORED:                            \n\t"  // C is column-major.
      "                                            \n\t"
      " dup z0.s, #0                               \n\t"
      " dup z1.s, #0                               \n\t"
      " dup z2.s, #0                               \n\t"
      " dup z3.s, #0                               \n\t"
      " dup z4.s, #0                               \n\t"
      " dup z5.s, #0                               \n\t"
      " dup z6.s, #0                               \n\t"
      " dup z7.s, #0                               \n\t"
      "                                            \n\t"
      " fcmp d9,#0.0                               \n\t"
      " beq .S256BETAZEROCOLSTOREDS1               \n\t"  // Taking care of the beta==0 case.
      "                                            \n\t"
      " ld1w  {z0.s}, p0/z, [x2]                   \n\t"  // Load column 0 of C
      " ld1w  {z1.s}, p0/z, [x2, #1, MUL VL]       \n\t"
      "                                            \n\t"
      " ld1w  {z2.s}, p0/z, [x20]                  \n\t"  // Load column 1 of C
      " ld1w  {z3.s}, p0/z, [x20, #1, MUL VL]      \n\t"
      "                                            \n\t"
      " ld1w  {z4.s}, p0/z, [x21]                  \n\t"  // Load column 2 of C
      " ld1w  {z5.s}, p0/z, [x21, #1, MUL VL]      \n\t"
      "                                            \n\t"
      " ld1w  {z6.s}, p0/z, [x22]                  \n\t"  // Load column 3 of C
      " ld1w  {z7.s}, p0/z, [x22, #1, MUL VL]      \n\t"
      "                                            \n\t"
      " fmul z0.s, p0/m, z0.s, z9.s                \n\t"  // Scale by beta
      " fmul z1.s, p0/m, z1.s, z9.s                \n\t"  // Scale by beta
      " fmul z2.s, p0/m, z2.s, z9.s                \n\t"  // Scale by beta
      " fmul z3.s, p0/m, z3.s, z9.s                \n\t"  // Scale by beta
      " fmul z4.s, p0/m, z4.s, z9.s                \n\t"  // Scale by beta
      " fmul z5.s, p0/m, z5.s, z9.s                \n\t"  // Scale by beta
      " fmul z6.s, p0/m, z6.s, z9.s                \n\t"  // Scale by beta
      " fmul z7.s, p0/m, z7.s, z9.s                \n\t"  // Scale by beta
      "                                            \n\t"
      " .S256BETAZEROCOLSTOREDS1:                  \n\t"
      "                                            \n\t"
      " fmla z0.s, p0/m, z12.s, z8.s               \n\t"  // Scale by alpha
      " fmla z1.s, p0/m, z13.s, z8.s               \n\t"  // Scale by alpha
      " fmla z2.s, p0/m, z14.s, z8.s               \n\t"  // Scale by alpha
      " fmla z3.s, p0/m, z15.s, z8.s               \n\t"  // Scale by alpha
      " fmla z4.s, p0/m, z16.s, z8.s               \n\t"  // Scale by alpha
      " fmla z5.s, p0/m, z17.s, z8.s               \n\t"  // Scale by alpha
      " fmla z6.s, p0/m, z18.s, z8.s               \n\t"  // Scale by alpha
      " fmla z7.s, p0/m, z19.s, z8.s               \n\t"  // Scale by alpha
      "                                            \n\t"
      " st1w  {z0.s}, p0, [x2]                     \n\t"  // Store column 0 of C
      " st1w  {z1.s}, p0, [x2, #1, MUL VL]         \n\t"
      "                                            \n\t"
      " st1w  {z2.s}, p0, [x20]                    \n\t"  // Store column 1 of C
      " st1w  {z3.s}, p0, [x20, #1, MUL VL]        \n\t"
      "                                            \n\t"
      " st1w  {z4.s}, p0, [x21]                    \n\t"  // Store column 2 of C
      " st1w  {z5.s}, p0, [x21, #1, MUL VL]        \n\t"
      "                                            \n\t"
      " st1w  {z6.s}, p0, [x22]                    \n\t"  // Store column 3 of C
      " st1w  {z7.s}, p0, [x22, #1, MUL VL]        \n\t"
      "                                            \n\t"
      " dup z0.s, #0                               \n\t"
      " dup z1.s, #0                               \n\t"
      " dup z2.s, #0                               \n\t"
      " dup z3.s, #0                               \n\t"
      " dup z4.s, #0                               \n\t"
      " dup z5.s, #0                               \n\t"
      " dup z6.s, #0                               \n\t"
      " dup z7.s, #0                               \n\t"
      "                                            \n\t"
      " fcmp d9,#0.0                               \n\t"
      " beq .S256BETAZEROCOLSTOREDS2               \n\t"  // Taking care of the beta==0 case.
      "                                            \n\t"
      " ld1w  {z0.s}, p0/z, [x23]                   \n\t"  // Load column 5 of C
      " ld1w  {z1.s}, p0/z, [x23, #1, MUL VL]       \n\t"
      "                                            \n\t"
      " ld1w  {z2.s}, p0/z, [x24]                  \n\t"  // Load column 6 of C
      " ld1w  {z3.s}, p0/z, [x24, #1, MUL VL]      \n\t"
      "                                            \n\t"
      " ld1w  {z4.s}, p0/z, [x25]                  \n\t"  // Load column 7 of C
      " ld1w  {z5.s}, p0/z, [x25, #1, MUL VL]      \n\t"
      "                                            \n\t"
      " ld1w  {z6.s}, p0/z, [x26]                  \n\t"  // Load column 8 of C
      " ld1w  {z7.s}, p0/z, [x26, #1, MUL VL]      \n\t"
      "                                            \n\t"
      " fmul z0.s, p0/m, z0.s, z9.s                \n\t"  // Scale by beta
      " fmul z1.s, p0/m, z1.s, z9.s                \n\t"  // Scale by beta
      " fmul z2.s, p0/m, z2.s, z9.s                \n\t"  // Scale by beta
      " fmul z3.s, p0/m, z3.s, z9.s                \n\t"  // Scale by beta
      " fmul z4.s, p0/m, z4.s, z9.s                \n\t"  // Scale by beta
      " fmul z5.s, p0/m, z5.s, z9.s                \n\t"  // Scale by beta
      " fmul z6.s, p0/m, z6.s, z9.s                \n\t"  // Scale by beta
      " fmul z7.s, p0/m, z7.s, z9.s                \n\t"  // Scale by beta
      "                                            \n\t"
      " .S256BETAZEROCOLSTOREDS2:                  \n\t"
      "                                            \n\t"
      " fmla z0.s, p0/m, z20.s, z8.s               \n\t"  // Scale by alpha
      " fmla z1.s, p0/m, z21.s, z8.s               \n\t"  // Scale by alpha
      " fmla z2.s, p0/m, z22.s, z8.s               \n\t"  // Scale by alpha
      " fmla z3.s, p0/m, z23.s, z8.s               \n\t"  // Scale by alpha
      " fmla z4.s, p0/m, z24.s, z8.s               \n\t"  // Scale by alpha
      " fmla z5.s, p0/m, z25.s, z8.s               \n\t"  // Scale by alpha
      " fmla z6.s, p0/m, z26.s, z8.s               \n\t"  // Scale by alpha
      " fmla z7.s, p0/m, z27.s, z8.s               \n\t"  // Scale by alpha
      "                                            \n\t"
      " st1w  {z0.s}, p0, [x23]                     \n\t"  // Store column 5 of C
      " st1w  {z1.s}, p0, [x23, #1, MUL VL]         \n\t"
      "                                            \n\t"
      " st1w  {z2.s}, p0, [x24]                    \n\t"  // Store column 6 of C
      " st1w  {z3.s}, p0, [x24, #1, MUL VL]        \n\t"
      "                                            \n\t"
      " st1w  {z4.s}, p0, [x25]                    \n\t"  // Store column 7 of C
      " st1w  {z5.s}, p0, [x25, #1, MUL VL]        \n\t"
      "                                            \n\t"
      " st1w  {z6.s}, p0, [x26]                    \n\t"  // Store column 8 of C
      " st1w  {z7.s}, p0, [x26, #1, MUL VL]        \n\t"
      "                                            \n\t"
      "                                            \n\t"
      "                                            \n\t"
      " b .S256END                                 \n\t"
      "                                            \n\t"
      " .S256GENSTORED:                            \n\t"  // C is general-stride stored.
      "                                            \n\t"
      " index z10.s, wzr, w13                       \n\t"  // Creating index for stride load&store access
      " mul w3, w13, w11                          \n\t"
      " index z11.s, w3, w13                       \n\t"  // Creating index for stride load&store access
      "                                            \n\t"
      " dup z0.s, #0                               \n\t"
      " dup z1.s, #0                               \n\t"
      " dup z2.s, #0                               \n\t"
      " dup z3.s, #0                               \n\t"
      " dup z4.s, #0                               \n\t"
      " dup z5.s, #0                               \n\t"
      " dup z6.s, #0                               \n\t"
      " dup z7.s, #0                               \n\t"
      "                                            \n\t"
      " fcmp d9,#0.0                               \n\t"
      " beq .S256BETAZEROGENSTOREDS1               \n\t"  // Taking care of the beta==0 case.
      "                                            \n\t"
      " ld1w {z0.s}, p0/z, [x2, z10.s, UXTW #2]     \n\t"  // Load column 0 of C
      " ld1w {z1.s}, p0/z, [x2, z11.s, UXTW #2]     \n\t"
      "                                            \n\t"
      " ld1w {z2.s}, p0/z, [x20, z10.s, UXTW #2]    \n\t"  // Load column 1 of C
      " ld1w {z3.s}, p0/z, [x20, z11.s, UXTW #2]    \n\t"
      "                                            \n\t"
      " ld1w {z4.s}, p0/z, [x21, z10.s, UXTW #2]    \n\t"  // Load column 2 of C
      " ld1w {z5.s}, p0/z, [x21, z11.s, UXTW #2]    \n\t"
      "                                            \n\t"
      " ld1w {z6.s}, p0/z, [x22, z10.s, UXTW #2]    \n\t"  // Load column 3 of C
      " ld1w {z7.s}, p0/z, [x22, z11.s, UXTW #2]    \n\t"
      "                                            \n\t"
      " fmul z0.s, p0/m, z0.s, z9.s                \n\t"  // Scale by beta
      " fmul z1.s, p0/m, z1.s, z9.s                \n\t"  // Scale by beta
      " fmul z2.s, p0/m, z2.s, z9.s                \n\t"  // Scale by beta
      " fmul z3.s, p0/m, z3.s, z9.s                \n\t"  // Scale by beta
      " fmul z4.s, p0/m, z4.s, z9.s                \n\t"  // Scale by beta
      " fmul z5.s, p0/m, z5.s, z9.s                \n\t"  // Scale by beta
      " fmul z6.s, p0/m, z6.s, z9.s                \n\t"  // Scale by beta
      " fmul z7.s, p0/m, z7.s, z9.s                \n\t"  // Scale by beta
      "                                            \n\t"
      " .S256BETAZEROGENSTOREDS1:                  \n\t"
      "                                            \n\t"
      " fmla z0.s, p0/m, z12.s, z8.s               \n\t"  // Scale by alpha
      " fmla z1.s, p0/m, z13.s, z8.s               \n\t"  // Scale by alpha
      " fmla z2.s, p0/m, z14.s, z8.s               \n\t"  // Scale by alpha
      " fmla z3.s, p0/m, z15.s, z8.s               \n\t"  // Scale by alpha
      " fmla z4.s, p0/m, z16.s, z8.s               \n\t"  // Scale by alpha
      " fmla z5.s, p0/m, z17.s, z8.s               \n\t"  // Scale by alpha
      " fmla z6.s, p0/m, z18.s, z8.s               \n\t"  // Scale by alpha
      " fmla z7.s, p0/m, z19.s, z8.s               \n\t"  // Scale by alpha
      "                                            \n\t"
      " st1w {z0.s}, p0, [x2, z10.s, UXTW #2]        \n\t"  // Store column 0 of C
      " st1w {z1.s}, p0, [x2, z11.s, UXTW #2]        \n\t"
      "                                            \n\t"
      " st1w {z2.s}, p0, [x20, z10.s, UXTW #2]       \n\t"  // Store column 1 of C
      " st1w {z3.s}, p0, [x20, z11.s, UXTW #2]       \n\t"
      "                                            \n\t"
      " st1w {z4.s}, p0, [x21, z10.s, UXTW #2]        \n\t"  // Store column 2 of C
      " st1w {z5.s}, p0, [x21, z11.s, UXTW #2]        \n\t"
      "                                            \n\t"
      " st1w {z6.s}, p0, [x22, z10.s, UXTW #2]       \n\t"  // Store column 3 of C
      " st1w {z7.s}, p0, [x22, z11.s, UXTW #2]       \n\t"
      "                                            \n\t"
      " dup z0.s, #0                               \n\t"
      " dup z1.s, #0                               \n\t"
      " dup z2.s, #0                               \n\t"
      " dup z3.s, #0                               \n\t"
      " dup z4.s, #0                               \n\t"
      " dup z5.s, #0                               \n\t"
      " dup z6.s, #0                               \n\t"
      " dup z7.s, #0                               \n\t"
      "                                            \n\t"
      " fcmp d9,#0.0                               \n\t"
      " beq .S256BETAZEROGENSTOREDS2               \n\t"  // Taking care of the beta==0 case.
      "                                            \n\t"
      " ld1w {z0.s}, p0/z, [x23, z10.s, UXTW #2]     \n\t"  // Load column 5 of C
      " ld1w {z1.s}, p0/z, [x23, z11.s, UXTW #2]     \n\t"
      "                                            \n\t"
      " ld1w {z2.s}, p0/z, [x24, z10.s, UXTW #2]    \n\t"  // Load column 6 of C
      " ld1w {z3.s}, p0/z, [x24, z11.s, UXTW #2]    \n\t"
      "                                            \n\t"
      " ld1w {z4.s}, p0/z, [x25, z10.s, UXTW #2]    \n\t"  // Load column 7 of C
      " ld1w {z5.s}, p0/z, [x25, z11.s, UXTW #2]    \n\t"
      "                                            \n\t"
      " ld1w {z6.s}, p0/z, [x26, z10.s, UXTW #2]    \n\t"  // Load column 8 of C
      " ld1w {z7.s}, p0/z, [x26, z11.s, UXTW #2]    \n\t"
      "                                            \n\t"
      " fmul z0.s, p0/m, z0.s, z9.s                \n\t"  // Scale by beta
      " fmul z1.s, p0/m, z1.s, z9.s                \n\t"  // Scale by beta
      " fmul z2.s, p0/m, z2.s, z9.s                \n\t"  // Scale by beta
      " fmul z3.s, p0/m, z3.s, z9.s                \n\t"  // Scale by beta
      " fmul z4.s, p0/m, z4.s, z9.s                \n\t"  // Scale by beta
      " fmul z5.s, p0/m, z5.s, z9.s                \n\t"  // Scale by beta
      " fmul z6.s, p0/m, z6.s, z9.s                \n\t"  // Scale by beta
      " fmul z7.s, p0/m, z7.s, z9.s                \n\t"  // Scale by beta
      "                                            \n\t"
      " .S256BETAZEROGENSTOREDS2:                  \n\t"
      "                                            \n\t"
      " fmla z0.s, p0/m, z20.s, z8.s               \n\t"  // Scale by alpha
      " fmla z1.s, p0/m, z21.s, z8.s               \n\t"  // Scale by alpha
      " fmla z2.s, p0/m, z22.s, z8.s               \n\t"  // Scale by alpha
      " fmla z3.s, p0/m, z23.s, z8.s               \n\t"  // Scale by alpha
      " fmla z4.s, p0/m, z24.s, z8.s               \n\t"  // Scale by alpha
      " fmla z5.s, p0/m, z25.s, z8.s               \n\t"  // Scale by alpha
      " fmla z6.s, p0/m, z26.s, z8.s               \n\t"  // Scale by alpha
      " fmla z7.s, p0/m, z27.s, z8.s               \n\t"  // Scale by alpha
      "                                            \n\t"
      " st1w {z0.s}, p0, [x23, z10.s, UXTW #2]       \n\t"  // Store column 5 of C
      " st1w {z1.s}, p0, [x23, z11.s, UXTW #2]       \n\t"
      "                                            \n\t"
      " st1w {z2.s}, p0, [x24, z10.s, UXTW #2]      \n\t"  // Store column 6 of C
      " st1w {z3.s}, p0, [x24, z11.s, UXTW #2]      \n\t"
      "                                            \n\t"
      " st1w {z4.s}, p0, [x25, z10.s, UXTW #2]      \n\t"  // Store column 7 of C
      " st1w {z5.s}, p0, [x25, z11.s, UXTW #2]      \n\t"
      "                                            \n\t"
      " st1w {z6.s}, p0, [x26, z10.s, UXTW #2]      \n\t"  // Store column 8 of C
      " st1w {z7.s}, p0, [x26, z11.s, UXTW #2]      \n\t"
      "                                            \n\t"
      " .S256END:                                  \n\t"  // Done!
      "                                            \n\t"
      :                      // output operands (none)
      :                      // input operands
      [aaddr] "m"(a),        // 0
      [baddr] "m"(b),        // 1
      [caddr] "m"(c),        // 2
      [k_iter] "m"(k_iter),  // 3
      [k_left] "m"(k_left),  // 4
      [alpha] "m"(alpha),    // 5
      [beta] "m"(beta),      // 6
      [rs_c] "m"(rs_c),      // 6
      [cs_c] "m"(cs_c),      // 7
      [a_next] "m"(a_next),  // 8
      [b_next] "m"(b_next)   // 9
      :                      // Register clobber list
      "x0", "x1", "x2", "w3", "x4", "x5", "x6", "x10", "w11", "w13", "x20", "x21", "x22", "x23", "x24", "x25", "x26",
      "x27", "x28", "z0", "z1", "z2", "z3", "z4", "z5", "z6", "z7", "z8", "z9", "z10", "z11", "z12", "z13", "z14",
      "z15", "z16", "z17", "z18", "z19", "z20", "z21", "z22", "z23", "z24", "z25", "z26", "z27", "z28", "z29", "z30",
      "z31", "p0");
}