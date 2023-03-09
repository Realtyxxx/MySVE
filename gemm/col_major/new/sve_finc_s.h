

#define LOAD1VEC_S(vec,preg,areg)\
    " ld1w  " #vec ".s, " #preg "/z, [" #areg "]           \n\t"

#define LOAD1VEC_VOFF_S(vec, preg, areg, off)\
    " ld1w   " #vec ".s, " #preg "/z, [" #areg ", #" #off", MUL VL]\n\t"

#define LOAD1VEC_CONT_S(vec,preg,areg,avec) LOAD1VEC_S(vec,preg,areg)

#define LOAD1VEC_GENI_S(vec,preg,areg,avec)\
    " ld1w  " #vec ".s, " #preg "/z, [" #areg "," #avec ".s, UXTW #2]\n\t"

#define LOAD2VEC_S(vec1,vec2,preg,areg)\
    " ld1w  " #vec1 ".s, " #preg "/z, [" #areg "]           \n\t"\
    " ld1w  " #vec2 ".s, " #preg "/z, [" #areg ",#1, MUL VL]\n\t"

#define LOAD2VEC_CONT_S(vec1,vec2,preg,areg,avec1,avec2) LOAD2VEC_S(vec1,vec2,preg,areg)

#define LOAD2VEC_GENI_S(vec1,vec2,preg,areg,avec1,avec2)\
    " ld1w  " #vec1 ".s, " #preg "/z, [" #areg "," #avec1 ".s, UXTW #2]\n\t"\
    " ld1w  " #vec2 ".s, " #preg "/z, [" #areg "," #avec2 ".s, UXTW #2]\n\t"

#define LDR_NOADDR_S(vec1,preg)\
    " ld1rw  " #vec1 ".s, " #preg "/z"
#define OA_S(areg,offset)\
    ",[" #areg ", #" #offset"]"

#define LOAD1VEC_DIST_OFF_S(vec1,preg,areg,off)\
    LDR_NOADDR_S(vec1,preg)OA_S(areg,off)"\n\t"

#define LOADVEC_DIST_S(vec1,preg,areg)\
    LDR_NOADDR_S(vec1,preg)OA_S(areg,0)"\n\t"

#define LOAD2VEC_DIST_S(vec1,vec2,preg,areg)\
    LDR_NOADDR_S(vec1,preg)OA_S(areg,0)"\n\t"\
    LDR_NOADDR_S(vec2,preg)OA_S(areg,4)"\n\t"

#define LOAD4VEC_DIST_S(vec1,vec2,vec3,vec4,preg,areg)\
    LDR_NOADDR_S(vec1,preg)OA_S(areg,0)"\n\t"\
    LDR_NOADDR_S(vec2,preg)OA_S(areg,4)"\n\t"\
    LDR_NOADDR_S(vec3,preg)OA_S(areg,8)"\n\t"\
    LDR_NOADDR_S(vec4,preg)OA_S(areg,12)"\n\t"

#define LOAD8VEC_DIST_S(vec1,vec2,vec3,vec4,vec5,vec6,vec7,vec8,preg,areg)\
    LDR_NOADDR_S(vec1,preg)OA_S(areg,0)"\n\t"\
    LDR_NOADDR_S(vec2,preg)OA_S(areg,4)"\n\t"\
    LDR_NOADDR_S(vec3,preg)OA_S(areg,8)"\n\t"\
    LDR_NOADDR_S(vec4,preg)OA_S(areg,12)"\n\t"\
    LDR_NOADDR_S(vec5,preg)OA_S(areg,16)"\n\t"\
    LDR_NOADDR_S(vec6,preg)OA_S(areg,20)"\n\t"\
    LDR_NOADDR_S(vec7,preg)OA_S(areg,24)"\n\t"\
    LDR_NOADDR_S(vec8,preg)OA_S(areg,28)"\n\t"


#if defined(USE_SVE_CMLA_INSTRUCTION)
// When using the fused complex mupliply-accumulate
// load {real, imag} into every 128 bit part of the vector

// Load 2 vectors worth of complex numbers
#define LOAD2VEC_C(vec1, vec2, preg, areg) LOAD2VEC_S(vec1, vec2, preg, areg)

#define LOAD2VEC_VOFF_C(vec1, vec2, preg, areg, off1, off2)\
    " ld1w  " #vec1 ".s, " #preg "/z, [" #areg ",#"#off1",   MUL VL]           \n\t"\
    " ld1w  " #vec2 ".s, " #preg "/z, [" #areg ",#"#off2", MUL VL]\n\t"

#define LDC2_DIST_NOADDR_S(vec1, vec2, preg)\
    " ld1rd {" #vec1 ".d}, " #preg "/z"
#define LOAD2VEC_DIST_OFF_C(vec1, vec2, preg, areg, off1, off2)\
    LDC2_DIST_NOADDR_S(vec1, vec2, preg)OA_S(areg,off1)"\n\t"

// Load 2 vectors and distribute the first complex number to all 128 parts
#define LOAD2VEC_DIST_C(vec1, vec2, preg, areg) LOAD2VEC_DIST_OFF_C(vec1, vec2, preg, areg, 0)

#define LOAD4VEC_DIST_OFF_C(vec1, vec2, vec3, vec4, preg, areg, off1, off2, off3, off4)\
    LOAD2VEC_DIST_OFF_C(vec1, vec2, preg, areg, off1, off2)\
    LOAD2VEC_DIST_OFF_C(vec3, vec4, preg, areg, off3, off4)

#define LOAD4VEC_DIST_C(vec1, vec2, vec3, vec4, preg, areg)\
    LOAD4VEC_DIST_OFF_C(vec1, vec2, vec3, vec4, preg, areg, 0, 4, 8, 12)

#define LOAD8VEC_DIST_C(vec1, vec2, vec3, vec4, vec5, vec6, vec7, vec8, preg, areg)\
    LOAD4VEC_DIST_OFF_C(vec1, vec2, vec3, vec4, preg, areg, 0, 4, 8, 12)\
    LOAD4VEC_DIST_OFF_C(vec5, vec6, vec7, vec8, preg, areg, 16, 20, 24, 28)

// Make an index for gather-load/scatter-store
// Here it has to be
// stride = estride*sizeof(double complex)
// ivec1 = {0,8,stride, stride+8, ... }
// ivec2 = {nelem*stride, nelem*stride+8, (nelem+1)*stride, (nelem+1)*stride+8, ... }
#define MKINDC_2VEC_S(ivec1, ivec2, stridereg, nelemreg, sparereg)\
    " lsl " #sparereg", "#stridereg", #1\n\t"\
    " index "#ivec1".s,wzr," #sparereg "\n\t"\
    " mov "  #ivec2".d," #ivec1".d\n\t"\
    " add "  #ivec2".s," #ivec2".s,#1\n\t"\
    " zip1 " #ivec1".s," #ivec1".s," #ivec2".s\n\t"\
    " mul "  #sparereg", "#sparereg", "#nelemreg"\n\t"\
    " lsr " #sparereg", " #sparereg", #1\n\t"\
    " dup "  #ivec2".s," #sparereg"\n\t"\
    " add "  #ivec2".s," #ivec2 ".s," #ivec1".s\n\t"

// Load 2 vectors worth of complex numbers
#define STOR2VEC_C(vec1, vec2, preg, areg) STOR2VEC_S(vec1, vec2, preg, areg)

#else
// Otherwise
// load {real} {imag} into separate vectors

#define LOAD2VEC_C(vec1, vec2, preg, areg)\
    " ld2w {" #vec1 ".s," #vec2 ".s}, " #preg "/z, [" #areg "]\n\t"

#define LOAD2VEC_VOFF_C(vec1, vec2, preg, areg, off1, off2)\
    " ld2w {" #vec1 ".s," #vec2 ".s}, " #preg "/z, [" #areg ",#"#off1", MUL VL]\n\t"

#define LOAD2VEC_DIST_OFF_C(vec_r, vec_i, preg,areg,off1,off2)\
    LDR_NOADDR_S(vec_r,preg)OA_S(areg,off1)"\n\t"\
    LDR_NOADDR_S(vec_i,preg)OA_S(areg,off2)"\n\t"

#define LOAD2VEC_DIST_C(vec_r, vec_i, preg, areg) LOAD2VEC_DIST_S(vec_r, vec_i, preg, areg)
#define LOAD4VEC_DIST_C(vec1_r, vec1_i, vec2_r, vec2_i, preg, areg)\
    LOAD4VEC_DIST_S(vec1_r, vec1_i, vec2_r, vec2_i, preg, areg)
#define LOAD8VEC_DIST_C(vec1_r, vec1_i, vec2_r, vec2_i, vec3_r, vec3_i, vec4_r, vec4_i, preg, areg)\
    LOAD8VEC_DIST_S(vec1_r, vec1_i, vec2_r, vec2_i, vec3_r, vec3_i, vec4_r, vec4_i, preg, areg)

// Make an index for gather-load/scatter-store
// Here it has to be
// stride = estride*sizeof(double complex)
// ivec1 = {0,stride,  2*stride,    ... }
// ivec2 = {8,stride+8,2*stride+8,  ... }
#define MKINDC_2VEC_S(ivec1, ivec2, stridereg, nelemreg, sparereg)\
    " lsl " #sparereg", "#stridereg", #1\n\t"\
    " index "#ivec1".s,wzr," #sparereg "\n\t"\
    " mov "#ivec2".d, "#ivec1".d\n\t"\
    " add "#ivec2".s, "#ivec2".s,#1\n\t"

#define STOR2VEC_C(vec1, vec2, preg, areg)\
    " st2w {" #vec1 ".s," #vec2 ".s}, " #preg ", [" #areg "]\n\t"

#endif

#define LOAD2VEC_CONT_C(vec1,vec2,preg,areg,avec1,avec2) LOAD2VEC_C(vec1,vec2,preg,areg)

#define LOAD2VEC_GENI_C(vec1,vec2,preg,areg,avec1,avec2)\
    " ld1w  " #vec1 ".s, " #preg "/z, [" #areg "," #avec1 ".s, UXTW #2]\n\t"\
    " ld1w  " #vec2 ".s, " #preg "/z, [" #areg "," #avec2 ".s, UXTW #2]\n\t"

// Store 1 vector to contiguous memory
#define STOR1VEC_S(vec,preg,areg)\
    " st1w  {" #vec ".s}, " #preg ", [" #areg "]           \n\t"

#define STOR1VEC_CONT_S(vec,preg,areg,avec) STOR1VEC_S(vec,preg,areg)

// Store 1 vector with generic indexing (scatter-store)
#define STOR1VEC_GENI_S(vec,preg,areg,avec)\
    " st1w  {" #vec ".s}, " #preg ", [" #areg "," #avec ".s, UXTW #2]\n\t"

// Store 2 vectors to contiguous memory
#define STOR2VEC_S(vec1,vec2,preg,areg)\
    " st1w  {" #vec1 ".s}, " #preg ", [" #areg "]           \n\t"\
    " st1w  {" #vec2 ".s}, " #preg ", [" #areg ",#1, MUL VL]\n\t"

#define STOR2VEC_CONT_S(vec1,vec2,preg,areg,avec1,avec2) STOR2VEC_S(vec1,vec2,preg,areg)

// Store 2 vectors with generic indexing (scatter-store)
#define STOR2VEC_GENI_S(vec1,vec2,preg,areg,avec1,avec2)\
    " st1w  {" #vec1 ".s}, " #preg ", [" #areg "," #avec1 ".s, UXTW #2]\n\t"\
    " st1w  {" #vec2 ".s}, " #preg ", [" #areg "," #avec2 ".s, UXTW #2]\n\t"

#define STOR2VEC_CONT_C(vec1,vec2,preg,areg,avec1,avec2) STORC2VEC_S(vec1,vec2,preg,areg)

// Store 2 vectors with generic indexing (scatter-store)
#define STOR2VEC_GENI_C(vec1,vec2,preg,areg,avec1,avec2)\
    " st1w  {" #vec1 ".s}, " #preg ", [" #areg "," #avec1 ".s, UXTW #2]\n\t"\
    " st1w  {" #vec2 ".s}, " #preg ", [" #areg "," #avec2 ".s, UXTW #2]\n\t"


// Some macros used for fixed size kernels

#if defined(USE_SVE_CMLA_INSTRUCTION)
#define LDR_BVEC_S(vec1, vec2, preg, reg, offset1, offset2)\
    " ld1rd  " #vec1 ".s, " #preg "/z, [" #reg ", #" #offset1 "]   \n\t"
#else
#define LDR_BVEC_S(vec1, vec2, preg, reg, offset1, offset2)\
    " ld1rw  " #vec1 ".s, " #preg "/z, [" #reg ", #" #offset1 "]   \n\t"\
    " ld1rw  " #vec2 ".s, " #preg "/z, [" #reg ", #" #offset2 "]   \n\t"
#endif

#if defined(USE_SVE_CMLA_INSTRUCTION)
#define LD_AVEC_S(vec1, vec2, preg, reg)\
    " ld1w  " #vec1 ".s, " #preg "/z, [" #reg "]           \n\t"\
    " ld1w  " #vec2 ".s, " #preg "/z, [" #reg ",#1, MUL VL]\n\t"
#else
#define LD_AVEC_S(vec1, vec2, preg, reg)\
    " ld2w {" #vec1 ".s," #vec2 ".s}, " #preg "/z, [" #reg "]\n\t"
#endif

#if defined(USE_SVE_CMLA_INSTRUCTION)
#define ST_AVEC_S(vec1, vec2, preg, reg)\
    " st1w  " #vec1 ".s, " #preg ", [" #reg "]           \n\t"\
    " st1w  " #vec2 ".s, " #preg ", [" #reg ",#1, MUL VL]\n\t"
#else
#define ST_AVEC_S(vec1, vec2, preg, reg)\
    " st2w {" #vec1 ".s," #vec2 ".s}, " #preg ", [" #reg "]\n\t"
#endif


#if defined(PREFETCH64)
    #define PREF64(type,areg,off)\
" prfm " #type ",[" #areg ",#" #off "]\n\t"
#else
    #define PREF64(type,areg,off)
#endif

#if defined(PREFETCH256)
    #define PREF256(type,areg,off)\
" prfm " #type ",[" #areg ",#" #off "]\n\t"
#else
    #define PREF256(type,areg,off)
#endif

#if defined(PREFETCH64) || defined(PREFETCH256)
    #define PREFANY(type,areg,off)\
" prfm " #type ",[" #areg ",#" #off "]\n\t"
#else
    #define PREFANY(type,areg,off)
#endif

#define COMBINE2(a,b) a ## _ ## b


#define ZEROVEC(vec1)\
    " dup " #vec1 ".s, #0\n\t"

#define ZERO2VEC_S(vec1,vec2)\
    ZEROVEC(vec1)\
    ZEROVEC(vec2)

#define ZERO4VEC_S(vec1,vec2,vec3,vec4)\
    ZERO2VEC_S(vec1,vec2)\
    ZERO2VEC_S(vec3,vec4)

#define ZERO8VEC_S(vec1,vec2,vec3,vec4,vec5,vec6,vec7,vec8)\
    ZERO4VEC_S(vec1,vec2,vec3,vec4)\
    ZERO4VEC_S(vec5,vec6,vec7,vec8)

#define MLA1ROW_S(cvec, avec, bvec, preg)\
    " fmla " #cvec ".s, " #preg "/m, " #avec ".s, " #bvec ".s\n\t"

#define MLA2ROW_S(c1, c2 , a1, a2, bvec, preg)\
    MLA1ROW_S(c1,a1,bvec,preg)\
    MLA1ROW_S(c2,a2,bvec,preg)

#define MLA4ROW_S(c1, c2, c3, c4, a1, a2, a3, a4, bvec, preg)\
    MLA2ROW_S(c1,c2,a1,a2,bvec,preg)\
    MLA2ROW_S(c3,c4,a3,a4,bvec,preg)


#define MUL1ROW_S(c1, a1, bvec, preg)\
    " fmul " #c1 ".s, " #preg "/m, " #a1 ".s, " #bvec ".s\n\t"

#define MUL2ROW_S(c1, c2 , a1, a2, bvec, preg)\
    MUL1ROW_S(c1,a1,bvec,preg)\
    MUL1ROW_S(c2,a2,bvec,preg)

#define MUL4ROW_S(c1, c2, c3, c4, a1, a2, a3, a4, bvec, preg)\
    MUL2ROW_S(c1,c2,a1,a2,bvec,preg)\
    MUL2ROW_S(c3,c4,a3,a4,bvec,preg)

#define MLA2X2ROW_S(c11, c12, c21, c22, a1, a2, bvec1,bvec2, preg)\
    MLA2ROW_S(c11, c12, a1, a2, bvec1, preg)\
    MLA2ROW_S(c21, c22, a1, a2, bvec2, preg)

#define MLA1ROW_LA_LB_S(cvec, avec, bvec, preg, nextavec, aareg, avoff, bareg, bboff)\
    MLA1ROW_S(cvec, avec, bvec, preg)\
    " ld1w   " #nextavec ".s, " #preg "/z, [" #aareg ", #" #avoff", MUL VL]\n\t"\
    " ld1rw  " #bvec ".s, "#preg"/z, [" #bareg",#" #bboff "]\n\t"

#define MLA2ROW_LA_LB_S(c1, c2 , a1, a2, bvec, preg, nextavec, aareg, avoff, bareg, bboff)\
    MLA2ROW_S(c1, c2, a1, a2, bvec, preg)\
    " ld1w   " #nextavec ".s, " #preg "/z, [" #aareg ", #" #avoff", MUL VL]\n\t"\
    " ld1rw  " #bvec ".s, "#preg"/z, [" #bareg",#" #bboff "]\n\t"

#define MLA2X2ROW_LA_LB_S(c11,c12,c21,c22, a1, a2, bvec1,bvec2, preg, nextavec, aareg, avoff, bareg, bboff1,bboff2)\
    MLA2ROW_S(c11, c12, a1, a2, bvec1, preg)\
    MLA2ROW_S(c21, c22, a1, a2, bvec2, preg)\
    " ld1w   " #nextavec ".s, " #preg "/z, [" #aareg ", #" #avoff", MUL VL]\n\t"\
    " ld1rw  " #bvec1 ".s, "#preg"/z, [" #bareg",#" #bboff1 "]\n\t"\
    " ld1rw  " #bvec2 ".s, "#preg"/z, [" #bareg",#" #bboff2 "]\n\t"

#define MLA4ROW_LA_LB_S(c1, c2, c3, c4, a1, a2, a3, a4, bvec, preg, nextavec, aareg, avoff, bareg, bboff)\
    MLA4ROW_S(c1, c2, c3, c4, a1, a2, a3, a4, bvec, preg)\
    " ld1w   " #nextavec ".s, " #preg "/z, [" #aareg ", #" #avoff", MUL VL]\n\t"\
    " ld1rw  " #bvec ".s, "#preg"/z, [" #bareg",#" #bboff "]\n\t"


#define MLA2X2ROW_LB_S(c11, c12, c21, c22, a1, a2, bvec1,bvec2, preg, bareg, bboff1,bboff2)\
    MLA2ROW_S(c11, c12, a1, a2, bvec1, preg)\
    MLA2ROW_S(c21, c22, a1, a2, bvec2, preg)\
    " ld1rw  " #bvec1 ".s, "#preg"/z, [" #bareg",#" #bboff1 "]\n\t"\
    " ld1rw  " #bvec2 ".s, "#preg"/z, [" #bareg",#" #bboff2 "]\n\t"

#define MLA1ROW_LB_S(cvec, avec, bvec, preg,  bareg, bboff)\
    MLA1ROW_S(cvec, avec, bvec, preg)\
    " ld1rw  " #bvec ".s, "#preg"/z, [" #bareg",#" #bboff "]\n\t"

#define MLA2ROW_LB_S(c1, c2 , a1, a2, bvec, preg,  bareg, bboff)\
    MLA2ROW_S(c1, c2, a1, a2, bvec, preg)\
    " ld1rw  " #bvec ".s, "#preg"/z, [" #bareg",#" #bboff "]\n\t"

#define MLA4ROW_LB_S(c1, c2, c3, c4, a1, a2, a3, a4, bvec, preg,  bareg, bboff)\
    MLA4ROW_S(c1, c2, c3, c4, a1, a2, a3, a4, bvec, preg)\
    " ld1rw  " #bvec ".s, "#preg"/z, [" #bareg",#" #bboff "]\n\t"

#define ZEROVEC_C(vec1) ZEROVEC_S(vec1)

#define ZERO2VEC_C(vec1,vec2) ZERO2VEC_S(vec1,vec2)

#define ZERO4VEC_C(vec1,vec2,vec3,vec4) ZERO4VEC_S(vec1,vec2,vec3,vec4)

#define ZERO8VEC_C(vec1,vec2,vec3,vec4,vec5,vec6,vec7,vec8)\
    ZERO8VEC_S(vec1,vec2,vec3,vec4,vec5,vec6,vec7,vec8)

#if defined(USE_SVE_CMLA_INSTRUCTION)
    #define MLA1ROW_ILV_C(cvec1, cvec2, avec1, avec2, bvec1, bvec2, preg, ilv1, ilv2, ilv3, ilv4)\
        " fcmla " #cvec1 ".s, " #preg "/m, " #avec1 ".s, " #bvec1 ".s, #0\n\t"\
        ilv1\
        " fcmla " #cvec2 ".s, " #preg "/m, " #avec2 ".s, " #bvec1 ".s, #0\n\t"\
        ilv2\
        " fcmla " #cvec1 ".s, " #preg "/m, " #avec1 ".s, " #bvec1 ".s, #90\n\t"\
        ilv3\
        " fcmla " #cvec2 ".s, " #preg "/m, " #avec2 ".s, " #bvec1 ".s, #90\n\t"\
        ilv4\
        "\n\t"
#else
    #define MLA1ROW_ILV_C(cvec_r, cvec_i, avec_r, avec_i, bvec_r, bvec_i, preg, ilv1, ilv2, ilv3, ilv4)\
        " fmla " #cvec_r ".s, " #preg "/m, " #avec_r ".s, " #bvec_r ".s\n\t"\
        ilv1\
        " fmla " #cvec_i ".s, " #preg "/m, " #avec_r ".s, " #bvec_i ".s\n\t"\
        ilv2\
        " fmls " #cvec_r ".s, " #preg "/m, " #avec_i ".s, " #bvec_i ".s\n\t"\
        ilv3\
        " fmla " #cvec_i ".s, " #preg "/m, " #avec_i ".s, " #bvec_r ".s\n\t"\
        ilv4\
        "\n\t"
#endif

#define MLA1ROW_C(cvec1, cvec2, avec1, avec2, bvec1, bvec2, preg)\
        MLA1ROW_ILV_C(cvec1, cvec2, avec1, avec2, bvec1, bvec2, preg,"","","","")

#define MLA1ROW_ILV_LA_LB_C(c1, c2, a1, a2, bvec1, bvec2, preg, ilv1, ilv2, ilv3, ilv4, nextavec1, nextavec2, aareg, avoff1, avoff2, bareg, bboff1, bboff2)\
    MLA1ROW_ILV_C(c1, c2, a1, a2, bvec1, bvec2, preg, ilv1, ilv2, ilv3, ilv4 )\
    LOAD2VEC_VOFF_C(nextavec1, nextavec2, preg, aareg, avoff1, avoff2)\
    LOAD2VEC_DIST_OFF_C(bvec1, bvec2, preg, bareg, bboff1,bboff2)

#define MLA1ROW_ILV_LB_C(c1, c2, a1, a2, bvec1, bvec2, preg, ilv1, ilv2, ilv3, ilv4, bareg, bboff1, bboff2)\
    MLA1ROW_ILV_C(c1, c2, a1, a2, bvec1, bvec2, preg, ilv1, ilv2, ilv3, ilv4)\
    LOAD2VEC_DIST_OFF_C(bvec1, bvec2, preg, bareg, bboff1, bboff2)

#define MLA1ROW_LA_LB_C(c1, c2, a1, a2, bvec1, bvec2, preg, nextavec1, nextavec2, aareg, avoff1, avoff2, bareg, bboff1, bboff2)\
    MLA1ROW_ILV_LA_LB_C(c1, c2, a1, a2, bvec1, bvec2, preg, "", "", "", "", nextavec1, nextavec2, aareg, avoff1, avoff2, bareg, bboff1, bboff2)

#define MLA1ROW_LB_C(c1, c2, a1, a2, bvec1, bvec2, preg, bareg, bboff1, bboff2)\
    MLA1ROW_ILV_LB_C(c1, c2, a1, a2, bvec1, bvec2, preg, "", "", "", "", bareg, bboff1, bboff2)

#define MLA2ROW_C(cvec1, cvec2, cvec3, cvec4, avec1, avec2, avec3, avec4, bvec1, bvec2, preg)\
    MLA1ROW_C(cvec1, cvec2, avec1, avec2, bvec1, bvec2, preg)\
    MLA1ROW_C(cvec3, cvec4, avec3, avec4, bvec1, bvec2, preg)

#define PFL1(areg,preg,offset) " prfd pldl1keep, "#preg", [" #areg ", #" #offset ", MUL VL]\n\t"


// Check if a complex number is 0
// TODO: Only first complex number needs to be checked, 
#if defined(USE_SVE_CMLA_INSTRUCTION)
#define CMPCZB_S(vec1,vec2,label)\
" fcmeq p1.s, p0/z, " #vec1 ".s, #0.0\n\t"\
" nots p1.b, p0/z, p1.b\n\t"\
" b.none " label "\n\t"
#else
#define CMPCZB_S(vec1,vec2,label)\
" fcmeq p1.s, p0/z, " #vec1 ".s, #0.0\n\t"\
" fcmeq p2.s, p0/z, " #vec2 ".s, #0.0\n\t"\
" ands p1.b, p0/z, p1.b, p2.b\n\t"\
" b.any " label "\n\t"
#endif

// Zero 2 columns of C,
// Load 2 columns of C and multiply by beta if beta !=0
// Add accumulated A*B values multiplied by alpha
// Store 2 columns of C
// Contiguous memory (CONT) or generic index (GENI) specified by addressing
// labelnr for the beta case jumps
#define FINC_2COL(fsuf, addressing,c0,c1,c2,c3,ca0,ca1, avec0,avec1, alpha, beta, acc0,acc1,acc2,acc3,labelnr)\
ZERO4VEC_S(c0,c1,c2,c3)\
"                                            \n\t"\
" fcmp d" #beta ",#0.0                       \n\t"\
" beq .S" #fsuf "BETAZERO" #addressing "COLSTOREDS" #labelnr "       \n\t"\
"                                            \n\t"\
COMBINE2(LOAD2VEC,addressing) (c0,c1,p0,ca0,avec0,avec1)\
COMBINE2(LOAD2VEC,addressing) (c2,c3,p0,ca1,avec0,avec1)\
"                                            \n\t"\
MUL4ROW_S(c0,c1,c2,c3,c0,c1,c2,c3,z ##beta,p0)\
"                                            \n\t"\
" .S" #fsuf "BETAZERO" #addressing "COLSTOREDS" #labelnr ":          \n\t"\
"                                            \n\t"\
MLA4ROW_S(c0,c1,c2,c3,acc0,acc1,acc2,acc3,z ##alpha,p0)\
"                                            \n\t"\
COMBINE2(STOR2VEC,addressing) (c0,c1,p0,ca0,avec0,avec1)\
COMBINE2(STOR2VEC,addressing) (c2,c3,p0,ca1,avec0,avec1)         

// 1x vector variant
// Zero 4 columns of C,
// Load 4 columns of C and multiply by beta if beta !=0
// Add accumulated A*B values multiplied by alpha
// Store 4 columns of C
// Contiguous memory (CONT) or generic index (GENI) specified by addressing
// labelnr for the beta case jumps
#define FINC_4COL_1X(fsuf, addressing,c0,c1,c2,c3, ca0,ca1,ca2,ca3, avec, alpha, beta, acc0,acc1,acc2,acc3, labelnr)\
ZERO4VEC_S(c0,c1,c2,c3)\
"                                            \n\t"\
" fcmp d" #beta ",#0.0                       \n\t"\
" beq .S" #fsuf "BETAZERO" #addressing "COLSTOREDS" #labelnr "       \n\t"\
"                                            \n\t"\
COMBINE2(LOAD1VEC,addressing) (c0,p0,ca0,avec)\
COMBINE2(LOAD1VEC,addressing) (c1,p0,ca1,avec)\
COMBINE2(LOAD1VEC,addressing) (c2,p0,ca2,avec)\
COMBINE2(LOAD1VEC,addressing) (c3,p0,ca3,avec)\
"                                            \n\t"\
MUL4ROW_S(c0,c1,c2,c3,c0,c1,c2,c3,z ##beta,p0)\
"                                            \n\t"\
" .S" #fsuf "BETAZERO" #addressing "COLSTOREDS" #labelnr ":          \n\t"\
"                                            \n\t"\
MLA4ROW_S(c0,c1,c2,c3,acc0,acc1,acc2,acc3,z ##alpha,p0)\
"                                            \n\t"\
COMBINE2(STOR1VEC,addressing) (c0,p0,ca0,avec)\
COMBINE2(STOR1VEC,addressing) (c1,p0,ca1,avec)\
COMBINE2(STOR1VEC,addressing) (c2,p0,ca2,avec)\
COMBINE2(STOR1VEC,addressing) (c3,p0,ca3,avec)

// 2x vector variant
// Zero 4 columns of C,
// Load 4 columns of C and multiply by beta if beta !=0
// Add accumulated A*B values multiplied by alpha
// Store 4 columns of C
// Contiguous memory (CONT) or generic index (GENI) specified by addressing
// labelnr for the beta case jumps
#define FINC_4COL(fsuf, addressing,c0,c1,c2,c3,c4,c5,c6,c7, ca0,ca1,ca2,ca3, avec0,avec1, alpha, beta, acc0,acc1,acc2,acc3,acc4,acc5,acc6,acc7, labelnr)\
ZERO8VEC_S(c0,c1,c2,c3,c4,c5,c6,c7)\
"                                            \n\t"\
" fcmp s" #beta ",#0.0                       \n\t"\
" beq .S" #fsuf "BETAZERO" #addressing "COLSTOREDS" #labelnr "       \n\t"\
"                                            \n\t"\
COMBINE2(LOAD2VEC,addressing) (c0,c1,p0,ca0,avec0,avec1)\
COMBINE2(LOAD2VEC,addressing) (c2,c3,p0,ca1,avec0,avec1)\
COMBINE2(LOAD2VEC,addressing) (c4,c5,p0,ca2,avec0,avec1)\
COMBINE2(LOAD2VEC,addressing) (c6,c7,p0,ca3,avec0,avec1)\
"                                            \n\t"\
MUL4ROW_S(c0,c1,c2,c3,c0,c1,c2,c3,z ##beta,p0)\
MUL4ROW_S(c4,c5,c6,c7,c4,c5,c6,c7,z ##beta,p0)\
"                                            \n\t"\
" .S" #fsuf "BETAZERO" #addressing "COLSTOREDS" #labelnr ":          \n\t"\
"                                            \n\t"\
MLA4ROW_S(c0,c1,c2,c3,acc0,acc1,acc2,acc3,z ##alpha,p0)\
MLA4ROW_S(c4,c5,c6,c7,acc4,acc5,acc6,acc7,z ##alpha,p0)\
"                                            \n\t"\
COMBINE2(STOR2VEC,addressing) (c0,c1,p0,ca0,avec0,avec1)\
COMBINE2(STOR2VEC,addressing) (c2,c3,p0,ca1,avec0,avec1)\
COMBINE2(STOR2VEC,addressing) (c4,c5,p0,ca2,avec0,avec1)\
COMBINE2(STOR2VEC,addressing) (c6,c7,p0,ca3,avec0,avec1)

// complex variant, 1row = 2 vectors worth of double complex
// Zero 4 columns of C,
// Load 4 columns of C and multiply by beta if beta !=0
// Add accumulated A*B values multiplied by alpha
// Store 4 columns of C
// Contiguous memory (CONT) or generic index (GENI) specified by addressing
// labelnr for the beta case jumps
// TODO: csX are needed because there is no fcmul instruction - find a better way without using
//       8 additional vector registers
#define CFINC_4COL(fsuf, addressing, c0,c1,c2,c3,c4,c5,c6,c7, cs0,cs1,cs2,cs3,cs4,cs5,cs6,cs7, ca0,ca1,ca2,ca3, avec1,avec2, alpha1,alpha2,beta1,beta2, acc0,acc1,acc2,acc3,acc4,acc5,acc6,acc7, labelnr)\
ZERO8VEC_S(c0,c1,c2,c3,c4,c5,c6,c7)\
"                                            \n\t"\
CMPCZB_S(z ##beta1, z ##beta2, ".C" #fsuf "BETAZERO" #addressing "COLSTOREDS" #labelnr)\
"                                            \n\t"\
COMBINE2(LOADC2VEC,addressing) (cs0,cs1,p0,ca0,avec1,avec2)\
COMBINE2(LOADC2VEC,addressing) (cs2,cs3,p0,ca1,avec1,avec2)\
COMBINE2(LOADC2VEC,addressing) (cs4,cs5,p0,ca2,avec1,avec2)\
COMBINE2(LOADC2VEC,addressing) (cs6,cs7,p0,ca3,avec1,avec2)\
"                                            \n\t"\
CMLA2ROW_S(c0,c1,c2,c3,cs0,cs1,cs2,cs3,z ##beta1,z ##beta2,p0)\
CMLA2ROW_S(c4,c5,c6,c7,cs4,cs5,cs6,cs7,z ##beta1,z ##beta2,p0)\
"                                            \n\t"\
" .C" #fsuf "BETAZERO" #addressing "COLSTOREDS" #labelnr ":          \n\t"\
"                                            \n\t"\
CMLA2ROW_S(c0,c1,c2,c3,acc0,acc1,acc2,acc3,z ##alpha1 , z ##alpha2 ,p0)\
CMLA2ROW_S(c4,c5,c6,c7,acc4,acc5,acc6,acc7,z ##alpha1 , z ##alpha2 ,p0)\
"                                            \n\t"\
COMBINE2(STORC2VEC,addressing) (c0,c1,p0,ca0,avec1,avec2)\
COMBINE2(STORC2VEC,addressing) (c2,c3,p0,ca1,avec1,avec2)\
COMBINE2(STORC2VEC,addressing) (c4,c5,p0,ca2,avec1,avec2)\
COMBINE2(STORC2VEC,addressing) (c6,c7,p0,ca3,avec1,avec2)
