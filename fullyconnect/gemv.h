#include <arm_sve.h>

// void impl_vm();
// void impl_mv();
//
// void gemv(float* a, float* b, float* c, int m, int n, int k) {
//   if (m == 1) {
//     impl_vm();
//   } else if (n == 1) {
//     impl_mv();
//   }
// }

// (mb, ic) * (oc, ic)
void fully_connect_naive_ccc(float* input, float* weight, float* output, int MB,
                             int IC, int OC) {
  for (int mb = 0; mb < MB; ++mb) {
#pragma omp parallel for
    for (int oc = 0; oc < OC; ++oc) {
      for (int ic = 0; ic < IC; ++ic) {
        output[mb * OC + oc] += input[mb * IC + ic] * weight[oc * IC + ic];
      }
    }
  }
}

// 逐个OC进行求值
void fully_connect_ccc_2(float* input, float* weight, float* output, int MB,
                         int IC, int OC) {
  memset(output, 0, OC * sizeof(float));
  int len   = svcntw();
  int len_2 = 2 * len;
  for (int mb = 0; mb < MB; ++mb) {
    float* in  = input + mb * IC;
    float* out = output + mb * OC;
#pragma omp parallel for
    for (int oc = 0; oc < OC; ++oc) {
      svfloat32_t    lvec0, lvec1, rvec0, rvec1, out0, out1;
      svbool_t       pg0, pg1;
      const svbool_t all_true_pg = svptrue_b32();
      out0                       = svdup_n_f32(0.f);
      out1                       = svdup_n_f32(0.f);
      for (int ic = 0; ic < IC; ic += len_2) {
        float* lptr = in + ic;
        pg0         = svwhilelt_b32(ic, IC);
        pg1         = svwhilelt_b32(ic + len, IC);

        lvec0 = svld1_f32(pg0, lptr);
        lvec1 = svld1_f32(pg1, lptr + len);

        float* rptr = weight + oc * IC + ic;
        rvec0       = svld1_f32(pg0, rptr);
        rvec1       = svld1_f32(pg1, rptr + len);
        // out0        = svmul_f32_z(pg0, lvec0, rvec0);
        // out1        = svmul_f32_z(pg1, lvec1, rvec1);
        out0 = svmla_f32_m(pg0, out0, lvec0, rvec0);
        out1 = svmla_f32_m(pg1, out1, lvec1, rvec1);
      }
      out[oc] += svaddv(all_true_pg, out0) + svaddv(all_true_pg, out1);
    }
  }
}

// 面向 (mb, ic) (oc, ic) 的情况, 重用ic
void fully_connect_ccc(float* input, float* weight, float* output, int MB,
                       int IC, int OC) {
  memset(output, 0, OC * sizeof(float));
  int len   = svcntw();
  int len_2 = 2 * len;
  for (int mb = 0; mb < MB; ++mb) {
    float* in  = input + mb * IC;
    float* out = output + mb * OC;
#pragma omp parallel for
    for (int ic = 0; ic < IC; ic += len_2) {
      svfloat32_t lvec0, lvec1, rvec0, rvec1, out0, out1;
      svbool_t    pg0, pg1;
      float*      lptr = in + ic;
      pg0              = svwhilelt_b32(ic, IC);
      pg1              = svwhilelt_b32(ic + len, IC);
      lvec0            = svld1_f32(pg0, lptr);
      lvec1            = svld1_f32(pg1, lptr + len);
      for (int oc = 0; oc < OC; ++oc) {
        float* rptr = weight + oc * IC + ic;
        rvec0       = svld1_f32(pg0, rptr);
        rvec1       = svld1_f32(pg1, rptr + len);
        out0        = svmul_f32_z(pg0, lvec0, rvec0);
        out1        = svmul_f32_z(pg1, lvec1, rvec1);
        float val   = svaddv(pg0, out0) + svaddv(pg1, out1);
        out[oc] += val;
      }
    }
  }
}