#include <arm_sve.h>

// for m < mr || n < nr case

// vl x k x1
inline vl_1() {}

// vl x k x 4
inline vl_4() {}

// vl x k x 8
inline vl_8(int m, int n, int k, float* alpha, float* a, float* b, float* beta,
            float* c int rs_c, int cs_c, void* a_next, void* b_next) {
  int    k_4   = k / 4;
  int    k_1   = j % 4;
  float* ptr_a = a;
  float* ptr_b = b;

  svfloat32_t z0, z1,                                 /* A */
      z2, z3, z4, z5, z6, z7, z8, z9,                 /* B */
      z12 = svdup_n_f32(0.f), z13 = svdup_n_f32(0.f), /* C */
      z14 = svdup_n_f32(0.f), z15 = svdup_n_f32(0.f), z16 = svdup_n_f32(0.f),
      z17 = svdup_n_f32(0.f), z18 = svdup_n_f32(0.f), z19 = svdup_n_f32(0.f),
      z20 = svdup_n_f32(0.f), z21 = svdup_n_f32(0.f), z22 = svdup_n_f32(0.f),
      z23 = svdup_n_f32(0.f), z24 = svdup_n_f32(0.f), z25 = svdup_n_f32(0.f),
      z26 = svdup_n_f32(0.f), z27 = svdup_n_f32(0.f);

  int      vl  = svcntw();
  svbool_t pg0 = svwhilelt_b32(0, m);
  svbool_t pg1 = svwhilelt_b32(vl, m);

  z0 = svld1(pg0, ptr_a);
  z1 = svld1(pg0, ptr_b);

  z2 = svdup_n_f32(b_ptr[0]);
  z3 = svdup_n_f32(b_ptr[1]);
  z4 = svdup_n_f32(b_ptr[2]);
  z5 = svdup_n_f32(b_ptr[3]);
  z6 = svdup_n_f32(b_ptr[4]);
  z7 = svdup_n_f32(b_ptr[5]);
  z8 = svdup_n_f32(b_ptr[6]);
  z9 = svdup_n_f32(b_ptr[7]);
  b_ptr += 8;

  while (--j_4) {  // 结束时候留一轮
    // 1st
    svmla_f32_z(pg0, z12, z0, z2);
    svmla_f32_z(pg0, z13, z1, z2);
    z2 = svdup_n_f32(b_ptr[0]);

    svmla_f32_z(pg0, z14, z0, z3);
    svmla_f32_z(pg0, z15, z1, z3);
    z3 = svdup_n_f32(b_ptr[1]);

    svmla_f32_z(pg0, z16, z0, z4);
    svmla_f32_z(pg0, z17, z1, z4);
    z4 = svdup_n_f32(b_ptr[2]);

    svmla_f32_z(pg0, z17, z0, z5);
    svmla_f32_z(pg0, z18, z1, z5);
    z5 = svdup_n_f32(b_ptr[3]);

    svmla_f32_z(pg0, z17, z0, z6);
    svmla_f32_z(pg0, z18, z1, z6);
    z5 = svdup_n_f32(b_ptr[4]);
  }
}

void mini_impl(int k, float* alpha, float* a, float* b, float* beta, float* c,
               int rs_c, int sc_c, void* a_next, void* b_next) {}
