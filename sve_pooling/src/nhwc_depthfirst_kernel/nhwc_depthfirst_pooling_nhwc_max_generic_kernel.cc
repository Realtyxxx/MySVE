#include <algorithm>
#include <cstddef>
#include <cstdint>

void sve_fp32_nhwc_max_generic_depthfirst_impl(  // TYX_NOTES: pooling op2kerneli generic depthfirst impl max
    const uint64_t,
    const uint64_t            n_valid_cells,  //
    uint64_t                  n_channels,
    const float *const *const inptrs,
    float                    *outptr,
    void *) {
  __asm__ __volatile__(
      "mov x28, #0x0\n"                      // x28 = 0
      "cntw x27\n"                           // x27 is the sve_len | sve_len is the lenght of sve register for word(4 bytes)
      "cntw x26, ALL, MUL #2\n"              // x26 is 2 x sve_len
      "cntw x25, ALL, MUL #3\n"              // x25 is 3 x sve_len
      "whilelt p4.s, x28, %x[n_channels]\n"  // 0 - sve_len
      "whilelt p3.s, x27, %x[n_channels]\n"  // sve_len - 2sve_len
      "whilelt p2.s, x26, %x[n_channels]\n"  // 2sve_len - 3sve_len
      "whilelt p1.s, x25, %x[n_channels]\n"  // 3sve_len - 4sve_len

      "ptrue p0.b\n"  // set p0 all true by byte

      "b.none 7f\n"

      "1:"                                  // TODO 4-vectors of channels
      "lsr x24, %x[n_valid_cells], #0x2\n"  // n >> 2, n = n_valid_cells / 4, for 4 iter 1 loop
      "mov z8.s, #0xff800000\n"             // ff80 0000 | 1111 1111 1000 0000, 0000 0000 0000 0000　minimum of float -inf
      "mov z7.s, #0xff800000\n"
      "mov x19, %x[inptrs]\n"  // x19 presents inptrs
      "mov z6.s, #0xff800000\n"
      "mov z5.s, #0xff800000\n"
      "cbz x24, 4f\n"                 // cbz compare-branch-zero jump to label 4 if x24 is zero(n_valid_cells < 4)
      "ldp x23, x22, [x19, #0x0]\n"   // load pair, first 2 address of float* inptrs inptrs[0], inptrs[1]
      "ldp x21, x20, [x19, #0x10]\n"  // load pair, first 2 address of float* inptrs inptrs[2], inptrs[3]

      "subs x24, x24, #0x1\n"  //! --n
      "add x19, x19, #0x20\n"  //! inptrs += 4;

      "ld1w { z4.s }, p4/Z, [x23, x28, LSL #2]\n"  // load inptrs[0] | for 0 - sve_len
      "ld1w { z3.s }, p4/Z, [x22, x28, LSL #2]\n"  // load inptrs[1] | for 0 - sve_len
      "ld1w { z2.s }, p4/Z, [x21, x28, LSL #2]\n"  // load inptrs[2] | for 0 - sve_len
      "ld1w { z1.s }, p4/Z, [x20, x28, LSL #2]\n"  // load inptrs[3] | for 0 - sve_len

      "ld1w { z0.s  }, p3/Z, [x23, x27, LSL #2]\n"  // load inptrs[0] | for sve_len - sve_len2
      "ld1w { z31.s }, p3/Z, [x22, x27, LSL #2]\n"  // load inptrs[1] | for sve_len - sve_len2
      "ld1w { z22.s }, p3/Z, [x21, x27, LSL #2]\n"  // load inptrs[2] | for sve_len - sve_len2
      "ld1w { z30.s }, p3/Z, [x20, x27, LSL #2]\n"  // load inptrs[3] | for sve_len - sve_len2

      "ld1w { z29.s }, p2/Z, [x23, x26, LSL #2]\n"  // load inptrs[0] | for sve_len2 - sve_len3
      "ld1w { z28.s }, p2/Z, [x22, x26, LSL #2]\n"  // load inptrs[1] | for sve_len2 - sve_len3
      "ld1w { z21.s }, p2/Z, [x21, x26, LSL #2]\n"  // load inptrs[2] | for sve_len2 - sve_len3
      "ld1w { z27.s }, p2/Z, [x20, x26, LSL #2]\n"  // load inptrs[3] | for sve_len2 - sve_len3

      "ld1w { z26.s }, p1/Z, [x23, x25, LSL #2]\n"  // load inptrs[0] | for sve_len3  -
      "ld1w { z25.s }, p1/Z, [x22, x25, LSL #2]\n"  // load inptrs[1] | for sve_len3  -
      "ld1w { z20.s }, p1/Z, [x21, x25, LSL #2]\n"  // load inptrs[2] | for sve_len3  -
      "ld1w { z24.s }, p1/Z, [x20, x25, LSL #2]\n"  // load inptrs[3] | for sve_len3  -
      "beq 3f\n"

      "2:"                                                   // TODO 4-vectors of channels: 4 inputs loop
      "movprfx z19, z4\n fmax z19.s, p0/M, z19.s, z3.s\n"    // z19 = max(z4, z3) | z4 = inptrs[0](0), z3 = inptrs[1](0)                                                    {computational}
      "movprfx z23, z2\n fmax z23.s, p0/M, z23.s, z1.s\n"    // z23 = max(z2, z1) | z2 = inptrs[2](0), z1 = inptrs[3](0)                                                    {computational}
      "ldp x23, x22, [x19, #0x0]\n"                          // x23 = inptrs[4], x22 = inptrs[5]                                                                            {   memoric   }
      "ldp x21, x20, [x19, #0x10]\n"                         // x21 = inptrs[6], x20 = inptrs[7]                                                                            {   memoric   }
      "movprfx z18, z0\n fmax z18.s, p0/M, z18.s, z31.s\n"   // z18 = max(z0, z31) | z0 = inptrs[0](1), z31 = inptrs[1](1)                                                  {computational}
      "fmax z22.s, p0/M, z22.s, z30.s\n"                     // z22 = max(z22, z30) | z22 = inptrs[2](1), z30 = inptrs[3](1)                                                {computational}
      "ld1w { z4.s }, p4/Z, [x23, x28, LSL #2]\n"            // load inptrs[4](0) , load to the place of inptrs[0](0)                                                       {   memoric   }
      "ld1w { z3.s }, p4/Z, [x22, x28, LSL #2]\n"            // load inptrs[5](0)                                                                                           {   memoric   }
      "movprfx z17, z29\n fmax z17.s, p0/M, z17.s, z28.s\n"  // z17 = max(z29, z28) | z29 = inptrs[0](2), z28 = inptrs[1](2)                                                {computational}
      "fmax z21.s, p0/M, z21.s, z27.s\n"                     // z21 = max(z21, z27) | z21 = inptrs[2](2), z27 = inptrs[3](2)                                                {computational}
      "ld1w { z2.s }, p4/Z, [x21, x28, LSL #2]\n"            // load inptrs[6](0)                                                                                           {   memoric   }
      "ld1w { z1.s }, p4/Z, [x20, x28, LSL #2]\n"            // load inptrs[7](0)                                                                                           {   memoric   }
      "movprfx z16, z26\n fmax z16.s, p0/M, z16.s, z25.s\n"  // 126 = max(z26, z25) | z26 = inptrs[0](3), z25 = inptrs[1](3)                                                {computational}
      "fmax z20.s, p0/M, z20.s, z24.s\n"                     // z20 = max(z20, z24) | z20 = inptrs[2](3), z24 = inptrs[3](3)                                                {computational}
      "ld1w { z0.s }, p3/Z, [x23, x27, LSL #2]\n"            // load inptrs[4](1)                                                                                           {   memoric   }
      "ld1w { z31.s }, p3/Z, [x22, x27, LSL #2]\n"           // load inptrs[5](1)                                                                                           {   memoric   }
      "fmax z19.s, p0/M, z19.s, z23.s\n"                     // z19 = max(z19, z23) | max(z4, z2 ) = max({inptrs[0], inptrs[1], inptrs[2], inptrs[3]})(0)                   {computational}
      "fmax z18.s, p0/M, z18.s, z22.s\n"                     // z18 = max(z18, z22) | max(z0, z22) = max({inptrs[0], inptrs[1], inptrs[2], inptrs[3]})(1)                   {computational}
      "ld1w { z22.s }, p3/Z, [x21, x27, LSL #2]\n"           // load inptrs[6](1)                                                                                           {   memoric   }
      "ld1w { z30.s }, p3/Z, [x20, x27, LSL #2]\n"           // load inptrs[7](1)                                                                                           {   memoric   }
      "fmax z17.s, p0/M, z17.s, z21.s\n"                     // z17 = max(z17, z21) | max(z4_prev, z2_prev) = max({inptrs[0], inptrs[1], inptrs[2], inptrs[3]})(2)          {computational}
      "fmax z16.s, p0/M, z16.s, z20.s\n"                     // z16 = max(z16, z20) | max(z26, z22_prev) = max({inptrs[0], inptrs[1], inptrs[2], inptrs[3]})(3)             {computational}
      "ld1w { z29.s }, p2/Z, [x23, x26, LSL #2]\n"           // load inptrs[4](2)                                                                                           {   memoric   }
      "ld1w { z28.s }, p2/Z, [x22, x26, LSL #2]\n"           // load inptrs[5](2)                                                                                           {   memoric   }
      "subs x24, x24, #0x1\n"                                // ! --n subs could update flags                                                                               {computational}{n}
      "fmax z8.s, p0/M, z8.s, z19.s\n"                       // z8 = max(z8, z19) |z8 initialized by -inf, and now just update the max-most value                           {computational}
      "ld1w { z21.s }, p2/Z, [x21, x26, LSL #2]\n"           // load inptrs[6](2)                                                                                           {   memoric   }
      "ld1w { z27.s }, p2/Z, [x20, x26, LSL #2]\n"           // load inptrs[7](2)                                                                                           {   memoric   }
      "fmax z7.s, p0/M, z7.s, z18.s\n"                       //  z7 = max(z7, z18) |                                                                                        {computational}
      "fmax z6.s, p0/M, z6.s, z17.s\n"                       //  z6 = max(z6, z17) |                                                                                        {computational}
      "ld1w { z26.s }, p1/Z, [x23, x25, LSL #2]\n"           // laod inptrs[4](3)                                                                                           {   memoric   }
      "ld1w { z25.s }, p1/Z, [x22, x25, LSL #2]\n"           // laod inptrs[5](3)                                                                                           {   memoric   }
      "fmax z5.s, p0/M, z5.s, z16.s\n"                       // z5 = max( z5, z16)                                                                                          {computational}
      "add x19, x19, #0x20\n"                                // ! inptrs += 4                                                                                               {computational}{inptrs}
      "ld1w { z20.s }, p1/Z, [x21, x25, LSL #2]\n"           // load inptrs[6](3)                                                                                           {   memoric   }
      "ld1w { z24.s }, p1/Z, [x20, x25, LSL #2]\n"           // load inptrs[7](3)                                                                                           {   memoric   }
                                                             // saw as 0-1-2-3 as [i : i + 3], then load [i+4 : i + 7]

      "bgt 2b\n"  // if n > 0 then loop

      "3:"                                                   // TODO 4-vectors of channels: 4 inputs tail
      "movprfx z19, z4\n fmax z19.s, p0/M, z19.s, z3.s\n"    // z19 = max(i, i + 1)(0)
      "movprfx z23, z2\n fmax z23.s, p0/M, z23.s, z1.s\n"    // z23 = max(i + 2, i + 3)(0)
      "movprfx z18, z0\n fmax z18.s, p0/M, z18.s, z31.s\n"   // z18 = max(i, i + 1)(1)
      "fmax z22.s, p0/M, z22.s, z30.s\n"                     // z22 = max(i + 2, i + 3)(1)
      "movprfx z17, z29\n fmax z17.s, p0/M, z17.s, z28.s\n"  // z17 = max(i, i + 1)(2)
      "fmax z21.s, p0/M, z21.s, z27.s\n"                     // z21 = max(i + 2, i + 3)(2)
      "movprfx z16, z26\n fmax z16.s, p0/M, z16.s, z25.s\n"  // z16 = max(i, i + 1)(3)
      "fmax z20.s, p0/M, z20.s, z24.s\n"                     // z20 = max(i + 2, i + 3)(3)
      "fmax z19.s, p0/M, z19.s, z23.s\n"                     // z19 = max(:)(0)
      "fmax z18.s, p0/M, z18.s, z22.s\n"                     // z18 = max(:)(1)
      "fmax z17.s, p0/M, z17.s, z21.s\n"                     // z17 = max(:)(2)
      "fmax z16.s, p0/M, z16.s, z20.s\n"                     // z16 = max(:)(3)
      "fmax z8.s, p0/M, z8.s, z19.s\n"                       // update max(:)(0)
      "fmax z7.s, p0/M, z7.s, z18.s\n"                       // update max(:)(1)
      "fmax z6.s, p0/M, z6.s, z17.s\n"                       // update max(:)(1)
      "fmax z5.s, p0/M, z5.s, z16.s\n"                       // update max(:)(1)

      "4:"                                   // TODO 4-vectors of channels: After loop
      "ands x20, %x[n_valid_cells], #0x3\n"  // x20 = n_valid_cells % 4
      "beq 6f\n"                             // no active

      "5:"  // TODO 4-vectors of channels: Single input loop
      "ldr x23, [x19], #0x8\n"
      "ld1w { z4.s }, p4/Z, [x23, x28, LSL #2]\n"
      "subs x20, x20, #0x1\n"
      "fmax z8.s, p0/M, z8.s, z4.s\n"
      "ld1w { z0.s }, p3/Z, [x23, x27, LSL #2]\n"
      "ld1w { z29.s }, p2/Z, [x23, x26, LSL #2]\n"
      "fmax z7.s, p0/M, z7.s, z0.s\n"
      "fmax z6.s, p0/M, z6.s, z29.s\n"
      "ld1w { z26.s }, p1/Z, [x23, x25, LSL #2]\n"
      "fmax z5.s, p0/M, z5.s, z26.s\n"
      "bgt 5b\n"

      "6:"                                              // TODO 4-vectors of channels: Single input loop: End
      "st1w { z8.s }, p4, [%x[outptr], x28, LSL #2]\n"  // store z8 -> output[0]
      "incw x28, ALL, MUL #4\n"                         // ? x28 + sve_len * 4 ?
      "st1w { z7.s }, p3, [%x[outptr], x27, LSL #2]\n"  // store z7 -> output[1]
      "incw x27, ALL, MUL #4\n"                         // x27 + sve_len * 4
      "st1w { z6.s }, p2, [%x[outptr], x26, LSL #2]\n"  // store z6-> output[2]
      "incw x26, ALL, MUL #4\n"                         // x26 + sve_len * 4
      "st1w { z5.s }, p1, [%x[outptr], x25, LSL #2]\n"  // store z6-> output[2]
      "incw x25, ALL, MUL #4\n"                         // x25 + sve_len * 4
      "whilelt p1.s, x25, %x[n_channels]\n"             // if x25 < n_channel then go to 1
      "b.any 1b\n"

      // TODO judge if exists 1 channel datas
      "7:"  // TODO Single vector of channels
      "whilelt p4.s, x28, %x[n_channels]\n"
      "b.none 14f\n"

      "8:"                                  // TODO Single vector of channels: Loop
      "lsr x24, %x[n_valid_cells], #0x2\n"  // reset n_valid_cores
      "mov z8.s, #0xff800000\n"             // reset min
      "mov x19, %x[inptrs]\n"
      "cbz x24, 11f\n"
      "ldp x23, x22, [x19, #0x0]\n"
      "ldp x21, x20, [x19, #0x10]\n"
      "subs x24, x24, #0x1\n"
      "add x19, x19, #0x20\n"
      "ld1w { z4.s }, p4/Z, [x23, x28, LSL #2]\n"
      "ld1w { z3.s }, p4/Z, [x22, x28, LSL #2]\n"
      "ld1w { z2.s }, p4/Z, [x21, x28, LSL #2]\n"
      "ld1w { z1.s }, p4/Z, [x20, x28, LSL #2]\n"
      "beq 10f\n"

      "9:"  // TODO Single vector of channels: Loop: 4 inputs loop
      "movprfx z19, z4\n fmax z19.s, p0/M, z19.s, z3.s\n"
      "movprfx z23, z2\n fmax z23.s, p0/M, z23.s, z1.s\n"
      "ldp x23, x22, [x19, #0x0]\n"
      "ldp x21, x20, [x19, #0x10]\n"
      "fmax z19.s, p0/M, z19.s, z23.s\n"
      "subs x24, x24, #0x1\n"
      "ld1w { z4.s }, p4/Z, [x23, x28, LSL #2]\n"
      "ld1w { z3.s }, p4/Z, [x22, x28, LSL #2]\n"
      "fmax z8.s, p0/M, z8.s, z19.s\n"
      "add x19, x19, #0x20\n"
      "ld1w { z2.s }, p4/Z, [x21, x28, LSL #2]\n"
      "ld1w { z1.s }, p4/Z, [x20, x28, LSL #2]\n"
      "bgt 9b\n"

      "10:"  // TODO Single vector of channels: Loop: 4 inputs tail
      "movprfx z19, z4\n fmax z19.s, p0/M, z19.s, z3.s\n"
      "movprfx z23, z2\n fmax z23.s, p0/M, z23.s, z1.s\n"
      "fmax z19.s, p0/M, z19.s, z23.s\n"
      "fmax z8.s, p0/M, z8.s, z19.s\n"

      "11:"                                  // TODO Single vector of channels: Loop: After loop
      "ands x20, %x[n_valid_cells], #0x3\n"  // x20 = n_valid_cells % 4
      "beq 13f\n"

      "12:"  // TODO Single vector of channels: Loop: Single input loop
      "ldr x23, [x19], #0x8\n"
      "ld1w { z4.s }, p4/Z, [x23, x28, LSL #2]\n"
      "subs x20, x20, #0x1\n"
      "fmax z8.s, p0/M, z8.s, z4.s\n"
      "bgt 12b\n"

      "13:"  // TODO Single vector of channels: Loop: Single input loop: End
      "st1w { z8.s }, p4, [%x[outptr], x28, LSL #2]\n"
      "incw x28\n"
      "whilelt p4.s, x28, %x[n_channels]\n"
      "b.any 8b\n"
      "14:"  // End
      :
      : [inptrs] "r"(inptrs), [n_channels] "r"(n_channels), [n_valid_cells] "r"(n_valid_cells), [outptr] "r"(outptr)
      : "cc", "memory", "p0", "p1", "p2", "p3", "p4", "x19", "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "z0", "z1", "z2", "z3", "z4", "z5", "z6", "z7", "z8", "z16", "z17", "z18",
        "z19", "z20", "z21", "z22", "z23", "z24", "z25", "z26", "z27", "z28", "z29", "z30", "z31");
  // cc : conditional codes 标志位
}

// bgt (label)b/f b is back f is front
