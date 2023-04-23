#include <arm_sve.h>

// for m < mr || n < nr case

// vl x k x1

// vl x k x 4

// vl x k x 8
void vl_8(int m, int k, float* alpha, float* a, float* b, float* beta, float* c,
          int rs_c, int cs_c, void* a_next, void* b_next) {
  int k_4 = k / 4;
  int k_1 = k % 4;
  // static int x = 1;
  // while (x--)

  float* a_ptr = a;
  float* b_ptr = b;
  float* c_ptr = c;

  const int m_2 = 2 * m;
  const int m_3 = 3 * m;
  const int m_4 = 4 * m;

  svfloat32_t z0, z1,                                 /* A */
      z2, z3, z4, z5, z6, z7, z8, z9,                 /* B */
      z12 = svdup_n_f32(0.f), z13 = svdup_n_f32(0.f), /* C */
      z14 = svdup_n_f32(0.f), z15 = svdup_n_f32(0.f), z16 = svdup_n_f32(0.f),
      z17 = svdup_n_f32(0.f), z18 = svdup_n_f32(0.f), z19 = svdup_n_f32(0.f),
      z20 = svdup_n_f32(0.f), z21 = svdup_n_f32(0.f), z22 = svdup_n_f32(0.f),
      z23 = svdup_n_f32(0.f), z24 = svdup_n_f32(0.f), z25 = svdup_n_f32(0.f),
      z26 = svdup_n_f32(0.f), z27 = svdup_n_f32(0.f);

  int      vl  = svcntw();
  int      x   = 0;
  svbool_t pg0 = svwhilelt_b32(x, m);
  svbool_t pg1 = svwhilelt_b32(x + vl, m);

  z0 = svld1(pg0, a_ptr);
  z1 = svld1(pg1, a_ptr + vl);

  z2 = svdup_n_f32(b_ptr[0]);
  z3 = svdup_n_f32(b_ptr[1]);
  z4 = svdup_n_f32(b_ptr[2]);
  z5 = svdup_n_f32(b_ptr[3]);
  z6 = svdup_n_f32(b_ptr[4]);
  z7 = svdup_n_f32(b_ptr[5]);
  z8 = svdup_n_f32(b_ptr[6]);
  z9 = svdup_n_f32(b_ptr[7]);
  b_ptr += 8;
  while (k_4--) {  // 结束时候留一轮
    // 1st
    z12 = svmla_f32_z(pg0, z12, z0, z2);
    z13 = svmla_f32_z(pg1, z13, z1, z2);
    z2  = svdup_n_f32(b_ptr[0]);

    z14 = svmla_f32_z(pg0, z14, z0, z3);
    z15 = svmla_f32_z(pg1, z15, z1, z3);
    z3  = svdup_n_f32(b_ptr[1]);

    z16 = svmla_f32_z(pg0, z16, z0, z4);
    z17 = svmla_f32_z(pg1, z17, z1, z4);
    z4  = svdup_n_f32(b_ptr[2]);

    z18 = svmla_f32_z(pg0, z18, z0, z5);
    z19 = svmla_f32_z(pg1, z19, z1, z5);
    z5  = svdup_n_f32(b_ptr[3]);

    z20 = svmla_f32_z(pg0, z20, z0, z6);
    z21 = svmla_f32_z(pg1, z21, z1, z6);
    z6  = svdup_n_f32(b_ptr[4]);

    z22 = svmla_f32_z(pg0, z22, z0, z7);
    z23 = svmla_f32_z(pg1, z23, z1, z7);
    z7  = svdup_n_f32(b_ptr[5]);

    z24 = svmla_f32_z(pg0, z24, z0, z8);
    z25 = svmla_f32_z(pg1, z25, z1, z8);
    z8  = svdup_n_f32(b_ptr[6]);

    z26 = svmla_f32_z(pg0, z26, z0, z9);
    z27 = svmla_f32_z(pg1, z27, z1, z9);
    z9  = svdup_n_f32(b_ptr[7]);

    z0 = svld1(pg0, a_ptr + m);
    z1 = svld1(pg1, a_ptr + m + vl);

    // 2nd
    z12 = svmla_f32_z(pg0, z12, z0, z2);
    z13 = svmla_f32_z(pg1, z13, z1, z2);
    z2  = svdup_n_f32(b_ptr[8]);

    z14 = svmla_f32_z(pg0, z14, z0, z3);
    z15 = svmla_f32_z(pg1, z15, z1, z3);
    z3  = svdup_n_f32(b_ptr[9]);

    z16 = svmla_f32_z(pg0, z16, z0, z4);
    z17 = svmla_f32_z(pg1, z17, z1, z4);
    z4  = svdup_n_f32(b_ptr[10]);

    z18 = svmla_f32_z(pg0, z18, z0, z5);
    z19 = svmla_f32_z(pg1, z19, z1, z5);
    z5  = svdup_n_f32(b_ptr[11]);

    z20 = svmla_f32_z(pg0, z20, z0, z6);
    z21 = svmla_f32_z(pg1, z21, z1, z6);
    z6  = svdup_n_f32(b_ptr[12]);

    z22 = svmla_f32_z(pg0, z22, z0, z7);
    z23 = svmla_f32_z(pg1, z23, z1, z7);
    z7  = svdup_n_f32(b_ptr[13]);

    z24 = svmla_f32_z(pg0, z24, z0, z8);
    z25 = svmla_f32_z(pg1, z25, z1, z8);
    z8  = svdup_n_f32(b_ptr[14]);

    z26 = svmla_f32_z(pg0, z26, z0, z9);
    z27 = svmla_f32_z(pg1, z27, z1, z9);
    z9  = svdup_n_f32(b_ptr[15]);

    z0 = svld1(pg0, a_ptr + m_2);
    z1 = svld1(pg1, a_ptr + m_2 + vl);

    // 3rd
    z12 = svmla_f32_z(pg0, z12, z0, z2);
    z13 = svmla_f32_z(pg1, z13, z1, z2);
    z2  = svdup_n_f32(b_ptr[16]);

    z14 = svmla_f32_z(pg0, z14, z0, z3);
    z15 = svmla_f32_z(pg1, z15, z1, z3);
    z3  = svdup_n_f32(b_ptr[17]);

    z16 = svmla_f32_z(pg0, z16, z0, z4);
    z17 = svmla_f32_z(pg1, z17, z1, z4);
    z4  = svdup_n_f32(b_ptr[18]);

    z18 = svmla_f32_z(pg0, z18, z0, z5);
    z19 = svmla_f32_z(pg1, z19, z1, z5);
    z5  = svdup_n_f32(b_ptr[19]);

    z20 = svmla_f32_z(pg0, z20, z0, z6);
    z21 = svmla_f32_z(pg1, z21, z1, z6);
    z6  = svdup_n_f32(b_ptr[20]);

    z22 = svmla_f32_z(pg0, z22, z0, z7);
    z23 = svmla_f32_z(pg1, z23, z1, z7);
    z7  = svdup_n_f32(b_ptr[21]);

    z24 = svmla_f32_z(pg0, z24, z0, z8);
    z25 = svmla_f32_z(pg1, z25, z1, z8);
    z8  = svdup_n_f32(b_ptr[22]);

    z26 = svmla_f32_z(pg0, z26, z0, z9);
    z27 = svmla_f32_z(pg1, z27, z1, z9);
    z9  = svdup_n_f32(b_ptr[23]);

    z0 = svld1(pg0, a_ptr + m_3);
    z1 = svld1(pg1, a_ptr + m_3 + vl);

    // 4th
    z12 = svmla_f32_z(pg0, z12, z0, z2);
    z13 = svmla_f32_z(pg1, z13, z1, z2);
    z2  = svdup_n_f32(b_ptr[24]);

    z14 = svmla_f32_z(pg0, z14, z0, z3);
    z15 = svmla_f32_z(pg1, z15, z1, z3);
    z3  = svdup_n_f32(b_ptr[25]);

    z16 = svmla_f32_z(pg0, z16, z0, z4);
    z17 = svmla_f32_z(pg1, z17, z1, z4);
    z4  = svdup_n_f32(b_ptr[26]);

    z18 = svmla_f32_z(pg0, z18, z0, z5);
    z19 = svmla_f32_z(pg1, z19, z1, z5);
    z5  = svdup_n_f32(b_ptr[27]);

    z20 = svmla_f32_z(pg0, z20, z0, z6);
    z21 = svmla_f32_z(pg1, z21, z1, z6);
    z6  = svdup_n_f32(b_ptr[28]);

    z22 = svmla_f32_z(pg0, z22, z0, z7);
    z23 = svmla_f32_z(pg1, z23, z1, z7);
    z7  = svdup_n_f32(b_ptr[29]);

    z24 = svmla_f32_z(pg0, z24, z0, z8);
    z25 = svmla_f32_z(pg1, z25, z1, z8);
    z8  = svdup_n_f32(b_ptr[30]);

    z26 = svmla_f32_z(pg0, z26, z0, z9);
    z27 = svmla_f32_z(pg1, z27, z1, z9);
    z9  = svdup_n_f32(b_ptr[31]);

    a_ptr += m_4;
    b_ptr += 32;

    z0 = svld1(pg0, a_ptr);
    z1 = svld1(pg1, a_ptr + vl);
  }

  while (k_1--) {
    // z0 = svld1(pg0, a_ptr);
    // z1 = svld1(pg1, a_ptr + vl);

    z12 = svmla_f32_z(pg0, z12, z0, z2);
    z13 = svmla_f32_z(pg1, z13, z1, z2);
    z2  = svdup_n_f32(b_ptr[0]);

    z14 = svmla_f32_z(pg0, z14, z0, z3);
    z15 = svmla_f32_z(pg1, z15, z1, z3);
    z3  = svdup_n_f32(b_ptr[1]);

    z16 = svmla_f32_z(pg0, z16, z0, z4);
    z17 = svmla_f32_z(pg1, z17, z1, z4);
    z4  = svdup_n_f32(b_ptr[2]);

    z18 = svmla_f32_z(pg0, z18, z0, z5);
    z19 = svmla_f32_z(pg1, z19, z1, z5);
    z5  = svdup_n_f32(b_ptr[3]);

    z20 = svmla_f32_z(pg0, z20, z0, z6);
    z21 = svmla_f32_z(pg1, z21, z1, z6);
    z6  = svdup_n_f32(b_ptr[4]);

    z22 = svmla_f32_z(pg0, z22, z0, z7);
    z23 = svmla_f32_z(pg1, z23, z1, z7);
    z7  = svdup_n_f32(b_ptr[5]);

    z24 = svmla_f32_z(pg0, z24, z0, z8);
    z25 = svmla_f32_z(pg1, z25, z1, z8);
    z8  = svdup_n_f32(b_ptr[6]);

    z26 = svmla_f32_z(pg0, z26, z0, z9);
    z27 = svmla_f32_z(pg1, z27, z1, z9);
    z9  = svdup_n_f32(b_ptr[7]);

    a_ptr += m;
    b_ptr += 8;
  }

  // printf("%d\n", cs_c);
  int c_len   = cs_c;
  int c_len_2 = cs_c * 2;
  int c_len_3 = cs_c * 3;
  z0          = svdup_n_f32(0.f);
  z1          = svdup_n_f32(0.f);
  z2          = svdup_n_f32(0.f);
  z3          = svdup_n_f32(0.f);
  z4          = svdup_n_f32(0.f);
  z5          = svdup_n_f32(0.f);
  z6          = svdup_n_f32(0.f);
  z7          = svdup_n_f32(0.f);

  z8 = svdup_n_f32(*alpha);
  z9 = svdup_n_f32(*beta);
  if (0 != *beta) {
    // load 0-4 col
    z0 = svld1(pg0, c_ptr);
    z1 = svld1(pg1, c_ptr + vl);

    z2 = svld1(pg0, c_ptr + c_len);
    z3 = svld1(pg1, c_ptr + c_len + vl);

    z4 = svld1(pg0, c_ptr + c_len_2);
    z5 = svld1(pg1, c_ptr + c_len_2 + vl);

    z6 = svld1(pg0, c_ptr + c_len_3);
    z7 = svld1(pg1, c_ptr + c_len_3 + vl);

    z0 = svmul_f32_z(pg0, z0, z9);
    z1 = svmul_f32_z(pg1, z1, z9);

    z2 = svmul_f32_z(pg0, z2, z9);
    z3 = svmul_f32_z(pg1, z3, z9);

    z4 = svmul_f32_z(pg0, z4, z9);
    z5 = svmul_f32_z(pg1, z5, z9);

    z6 = svmul_f32_z(pg0, z6, z9);
    z7 = svmul_f32_z(pg1, z7, z9);
  }
  z0 = svmla_f32_z(pg0, z0, z12, z8);
  z1 = svmla_f32_z(pg1, z1, z13, z8);

  z2 = svmla_f32_z(pg0, z2, z14, z8);
  z3 = svmla_f32_z(pg1, z3, z15, z8);

  z4 = svmla_f32_z(pg0, z4, z16, z8);
  z5 = svmla_f32_z(pg1, z5, z17, z8);

  z6 = svmla_f32_z(pg0, z6, z18, z8);
  z7 = svmla_f32_z(pg1, z7, z19, z8);

  svst1_f32(pg0, c_ptr, z0);
  svst1_f32(pg1, c_ptr + vl, z1);

  svst1_f32(pg0, c_ptr + c_len, z2);
  svst1_f32(pg1, c_ptr + c_len + vl, z3);

  svst1_f32(pg0, c_ptr + c_len_2, z4);
  svst1_f32(pg1, c_ptr + c_len_2 + vl, z5);

  svst1_f32(pg0, c_ptr + c_len_3, z6);
  svst1_f32(pg1, c_ptr + c_len_3 + vl, z7);

  c_ptr += c_len * 4;

  z0 = svdup_n_f32(0.f);
  z1 = svdup_n_f32(0.f);
  z2 = svdup_n_f32(0.f);
  z3 = svdup_n_f32(0.f);
  z4 = svdup_n_f32(0.f);
  z5 = svdup_n_f32(0.f);
  z6 = svdup_n_f32(0.f);
  z7 = svdup_n_f32(0.f);

  z8 = svdup_n_f32(*alpha);
  z9 = svdup_n_f32(*beta);
  if (*beta != 0) {
    z0 = svld1(pg0, c_ptr);
    z1 = svld1(pg1, c_ptr + vl);

    z2 = svld1(pg0, c_ptr + c_len);
    z3 = svld1(pg1, c_ptr + c_len + vl);

    z4 = svld1(pg0, c_ptr + c_len_2);
    z5 = svld1(pg1, c_ptr + c_len_2 + vl);

    z6 = svld1(pg0, c_ptr + c_len_3);
    z7 = svld1(pg1, c_ptr + c_len_3 + vl);

    z0 = svmul_f32_z(pg0, z0, z9);
    z1 = svmul_f32_z(pg1, z1, z9);

    z2 = svmul_f32_z(pg0, z2, z9);
    z3 = svmul_f32_z(pg1, z3, z9);

    z4 = svmul_f32_z(pg0, z4, z9);
    z5 = svmul_f32_z(pg1, z5, z9);

    z6 = svmul_f32_z(pg0, z6, z9);
    z7 = svmul_f32_z(pg1, z7, z9);
  }
  z0 = svmla_f32_z(pg0, z0, z20, z8);
  z1 = svmla_f32_z(pg1, z1, z21, z8);

  z2 = svmla_f32_z(pg0, z2, z22, z8);
  z3 = svmla_f32_z(pg1, z3, z23, z8);

  z4 = svmla_f32_z(pg0, z4, z24, z8);
  z5 = svmla_f32_z(pg1, z5, z25, z8);

  z6 = svmla_f32_z(pg0, z6, z26, z8);
  z7 = svmla_f32_z(pg1, z7, z27, z8);

  svst1_f32(pg0, c_ptr, z0);
  svst1_f32(pg1, c_ptr + vl, z1);

  svst1_f32(pg0, c_ptr + c_len, z2);
  svst1_f32(pg1, c_ptr + c_len + vl, z3);

  svst1_f32(pg0, c_ptr + c_len_2, z4);
  svst1_f32(pg1, c_ptr + c_len_2 + vl, z5);

  svst1_f32(pg0, c_ptr + c_len_3, z6);
  svst1_f32(pg1, c_ptr + c_len_3 + vl, z7);
}

// void mini_impl(int k, float* alpha, float* a, float* b, float* beta, float*
// c,
//                int rs_c, int sc_c, void* a_next, void* b_next) {}
