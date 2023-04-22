#include <arm_sve.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// time count function
static struct timeval start;
static struct timeval end;

void tic(void) { gettimeofday(&start, NULL); }

double toc(void) {
  gettimeofday(&end, NULL);
  return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

void test_flops(long long num) {
  __asm__ volatile(
      "ldr x5, %[num]                     \n\t"
      "                                   \n\t"
      "ptrue p0.s                         \n\t"
      "ptrue p1.s                         \n\t"
      "ptrue p2.s                         \n\t"
      "ptrue p3.s                         \n\t"
      "ptrue p4.s                         \n\t"
      "ptrue p5.s                         \n\t"
      "ptrue p6.s                         \n\t"
      "ptrue p7.s                         \n\t"
      "                                   \n\t"
      ".LOOP:                              \n\t"
      "fmla z0.s, p0/m, z20.s, z21.s      \n\t"
      "fmla z1.s, p1/m, z20.s, z21.s      \n\t"
      "fmla z2.s, p2/m, z20.s, z21.s      \n\t"
      "fmla z3.s, p3/m, z20.s, z21.s      \n\t"
      "fmla z4.s, p4/m, z20.s, z21.s      \n\t"
      "fmla z5.s, p5/m, z20.s, z21.s      \n\t"
      "fmla z6.s, p6/m, z20.s, z21.s      \n\t"
      "fmla z7.s, p7/m, z20.s, z21.s      \n\t"
      "fmla z8.s, p0/m, z20.s, z21.s      \n\t"
      "fmla z9.s, p1/m, z20.s, z21.s      \n\t"
      "fmla z10.s, p2/m, z20.s, z21.s     \n\t"
      "fmla z11.s, p3/m, z20.s, z21.s     \n\t"
      "fmla z12.s, p4/m, z20.s, z21.s     \n\t"
      "fmla z13.s, p5/m, z20.s, z21.s     \n\t"
      "fmla z14.s, p6/m, z20.s, z21.s     \n\t"
      "fmla z15.s, p7/m, z20.s, z21.s     \n\t"
      "                                   \n\t"
      "fmla z0.s, p0/m, z20.s, z21.s      \n\t"
      "fmla z1.s, p1/m, z20.s, z21.s      \n\t"
      "fmla z2.s, p2/m, z20.s, z21.s      \n\t"
      "fmla z3.s, p3/m, z20.s, z21.s      \n\t"
      "fmla z4.s, p4/m, z20.s, z21.s      \n\t"
      "fmla z5.s, p5/m, z20.s, z21.s      \n\t"
      "fmla z6.s, p6/m, z20.s, z21.s      \n\t"
      "fmla z7.s, p7/m, z20.s, z21.s      \n\t"
      "fmla z8.s, p0/m, z20.s, z21.s      \n\t"
      "fmla z9.s, p1/m, z20.s, z21.s      \n\t"
      "fmla z10.s, p2/m, z20.s, z21.s     \n\t"
      "fmla z11.s, p3/m, z20.s, z21.s     \n\t"
      "fmla z12.s, p4/m, z20.s, z21.s     \n\t"
      "fmla z13.s, p5/m, z20.s, z21.s     \n\t"
      "fmla z14.s, p6/m, z20.s, z21.s     \n\t"
      "fmla z15.s, p7/m, z20.s, z21.s     \n\t"
      "                                   \n\t"
      "fmla z0.s, p0/m, z20.s, z21.s      \n\t"
      "fmla z1.s, p1/m, z20.s, z21.s      \n\t"
      "fmla z2.s, p2/m, z20.s, z21.s      \n\t"
      "fmla z3.s, p3/m, z20.s, z21.s      \n\t"
      "fmla z4.s, p4/m, z20.s, z21.s      \n\t"
      "fmla z5.s, p5/m, z20.s, z21.s      \n\t"
      "fmla z6.s, p6/m, z20.s, z21.s      \n\t"
      "fmla z7.s, p7/m, z20.s, z21.s      \n\t"
      "fmla z8.s, p0/m, z20.s, z21.s      \n\t"
      "fmla z9.s, p1/m, z20.s, z21.s      \n\t"
      "fmla z10.s, p2/m, z20.s, z21.s     \n\t"
      "fmla z11.s, p3/m, z20.s, z21.s     \n\t"
      "fmla z12.s, p4/m, z20.s, z21.s     \n\t"
      "fmla z13.s, p5/m, z20.s, z21.s     \n\t"
      "fmla z14.s, p6/m, z20.s, z21.s     \n\t"
      "fmla z15.s, p7/m, z20.s, z21.s     \n\t"
      "                                   \n\t"
      "fmla z0.s, p0/m, z20.s, z21.s      \n\t"
      "fmla z1.s, p1/m, z20.s, z21.s      \n\t"
      "fmla z2.s, p2/m, z20.s, z21.s      \n\t"
      "fmla z3.s, p3/m, z20.s, z21.s      \n\t"
      "fmla z4.s, p4/m, z20.s, z21.s      \n\t"
      "fmla z5.s, p5/m, z20.s, z21.s      \n\t"
      "fmla z6.s, p6/m, z20.s, z21.s      \n\t"
      "fmla z7.s, p7/m, z20.s, z21.s      \n\t"
      "fmla z8.s, p0/m, z20.s, z21.s      \n\t"
      "fmla z9.s, p1/m, z20.s, z21.s      \n\t"
      "fmla z10.s, p2/m, z20.s, z21.s     \n\t"
      "fmla z11.s, p3/m, z20.s, z21.s     \n\t"
      "fmla z12.s, p4/m, z20.s, z21.s     \n\t"
      "fmla z13.s, p5/m, z20.s, z21.s     \n\t"
      "fmla z14.s, p6/m, z20.s, z21.s     \n\t"
      "fmla z15.s, p7/m, z20.s, z21.s     \n\t"
      "                                   \n\t"
      "fmla z0.s, p0/m, z20.s, z21.s      \n\t"
      "fmla z1.s, p1/m, z20.s, z21.s      \n\t"
      "fmla z2.s, p2/m, z20.s, z21.s      \n\t"
      "fmla z3.s, p3/m, z20.s, z21.s      \n\t"
      "fmla z4.s, p4/m, z20.s, z21.s      \n\t"
      "fmla z5.s, p5/m, z20.s, z21.s      \n\t"
      "fmla z6.s, p6/m, z20.s, z21.s      \n\t"
      "fmla z7.s, p7/m, z20.s, z21.s      \n\t"
      "fmla z8.s, p0/m, z20.s, z21.s      \n\t"
      "fmla z9.s, p1/m, z20.s, z21.s      \n\t"
      "fmla z10.s, p2/m, z20.s, z21.s     \n\t"
      "fmla z11.s, p3/m, z20.s, z21.s     \n\t"
      "fmla z12.s, p4/m, z20.s, z21.s     \n\t"
      "fmla z13.s, p5/m, z20.s, z21.s     \n\t"
      "fmla z14.s, p6/m, z20.s, z21.s     \n\t"
      "fmla z15.s, p7/m, z20.s, z21.s     \n\t"
      "                                   \n\t"
      "sub x5, x5, #1                     \n\t"
      "cmp x5, #0                         \n\t"
      "                                   \n\t"
      "bne .LOOP                           \n\t"
      "                                   \n\t"
      // ".LAST:                             \n\t"
      "                                   \n\t"
      ".END:                              \n\t"
      "                                   \n\t"
      :               // output operands (none)
      :               // input operands
      [num] "m"(num)  // 0
      : "x0", "x1", "x2", "w3", "x4", "x5", "x6", "x10", "w11", "w13", "x20", "x21", "x22", "x23", "x24", "x25", "x26", "x27", "x28", "z0", "z1", "z2", "z3", "z4", "z5", "z6", "z7", "z8", "z9", "z10",
        "z11", "z12", "z13", "z14", "z15", "z16", "z17", "z18", "z19", "z20", "z21", "z22", "z23", "z24", "z25", "z26", "z27", "z28", "z29", "z30", "z31", "p0");
}

int main(int argc, char *argv[]) {
  int iter_time = atoi(argv[1]);
  int sve_len   = svcntw();
  printf("begin %d\n", iter_time);
  tic();
  test_flops(iter_time);
  double time = toc();
  printf("%f\n", 2 * 16 * 5 * iter_time / (time * 10.e9));
  return 0;
}
