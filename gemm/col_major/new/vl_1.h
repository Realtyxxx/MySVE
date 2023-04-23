#include <arm_sve.h>

void vl_1(int m, int rs_n, int k, float* alpha, float* a, float* b, float* beta,
          float* c, int rs_c, int cs_c, void* a_next, void* b_next) {
  int k_4 = k / 4;
  int k_1 = k % 4;

  float* a_ptr = a;
  float* b_ptr = b;
  float* c_ptr = c;

  const int m_2 = 2 * m;
  const int m_3 = 3 * m;
  const int m_4 = 4 * m;

  svfloat32_t z0, z1,                                 /* A */
      z2,                                             /* B */
      z12 = svdup_n_f32(0.f), z13 = svdup_n_f32(0.f), /* C */
      z8, z9;

  int      vl  = svcntw();
  int      x   = 0;
  svbool_t pg0 = svwhilelt_b32(x, m);
  svbool_t pg1 = svwhilelt_b32(x + vl, m);

  z0 = svld1(pg0, a_ptr);
  z1 = svld1(pg1, a_ptr + vl);
  z2 = svdup_n_f32(*b_ptr);

  while (k--) {

    z12 = svmla_f32_z(pg0, z12, z0, z2);
    z13 = svmla_f32_z(pg1, z13, z1, z2);
    b_ptr += rs_n;
    a_ptr += m;
    z2 = svdup_n_f32(*b_ptr);
    z0 = svld1(pg0, a_ptr);
    z1 = svld1(pg1, a_ptr + vl);
  }

  z0 = svdup_n_f32(0.f);
  z1 = svdup_n_f32(0.f);

  z8 = svdup_n_f32(*alpha);
  z9 = svdup_n_f32(*beta);

  if (0 != *beta) {
    z0 = svld1(pg0, c_ptr);
    z1 = svld1(pg1, c_ptr + vl);
    z0 = svmul_f32_z(pg0, z0, z9);
    z1 = svmul_f32_z(pg1, z1, z9);
  }
  z0 = svmla_f32_z(pg0, z0, z12, z8);
  z1 = svmla_f32_z(pg1, z1, z13, z8);

  svst1_f32(pg0, c_ptr, z0);
  svst1_f32(pg1, c_ptr + vl, z1);
}
