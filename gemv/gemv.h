#include <arm_sve.h>

void impl_vm();
void impl_mv();

void gemv(float* a, float* b, float* c, int m, int n, int k) {
  if (m == 1) {
    impl_vm();
  } else if (n == 1) {
    impl_mv();
  }
}

// (mb, ic) * (ic, oc)
void fully_connect_naive_ccc(float* input, float* weight, float* output, int MB, int IC, int OC) {
  for (int mb = 0; mb < MB; ++mb) {
    for (int ic = 0; ic < IC; ++ic) {
      for (int oc = 0; oc < OC; ++oc) {
        output[mb * oc] += input[mb * ic] * weight[ic * oc];
      }
    }
  }
}

void fully_connect_ccc(float* input, float* weight, float* output, int MB, int IC, int OC) {
  memset(out, 0, OC * sizeof(float));
  static int  len   = svcntw();
  static int  len_2 = 2 * len;
  svfloat32_t lvec0, lvec1, rvec0, rvec1, out0, out1;
  svbool_t    pg0, pg1;
  for (int mb = 0; mb < MB; ++mb) {
    float* in     = input + mb * IC;
    float* output = output + mb * OC;
    for (int ic = 0; ic < IC; ic += len_2) {
      float* lptr = in + ic;
      pg0         = svwhile_lt(ic, IC);
      pg1         = svwhile_lt(ic + len, IC);
      lvec0       = svld1_f32(pg0, lptr);
      lvec1       = svld1_f32(pg1, lptr + len);
      for (int oc = 0; oc < OC; ++oc) {
        float* rptr = weight + oc * IC + ic;
        rvec0       = svld1_f32(pg0, rptr);
        rvec1       = svld1_f32(pg0, rptr + len);
        out0        = fmul(pg0, lvec0, rvec0);
        out1        = fmul(pg1, lvec1, rvec1);
        out[oc] += svaddv(pg0, out0) + svaddv(pg1, out1);
      }
    }
  }
}
