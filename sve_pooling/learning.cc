#include <alloca.h>
#include <memory.h>
#include <iostream>
using namespace std;

int main() {

  {
    // __asm__ volatile("");
    cout << "---------------------------------" << endl;
    int y, x;
    x = 10;
    y = 20;
    cout << x << "\n"
         << y << endl;
    __asm__ volatile(" mov %[result], %[value]"
                     : [result] "=r"(y) /* Rotation result.*/
                     : [value] "r"(x)   /* Rotated value.*/
                     :                  /* No clobbers */
    );
    cout << x << "\n"
         << y << endl;
  }

  {
    cout << "---------------------------------" << endl;

    int len = 0;
    __asm__ volatile(
        // "cntw x21\n\t"
        // "mov  %x[length], x21\n\t"
        "cntw %x[length]\n"
        : [length] "+r"(len)
        :
        :);
    cout << len << endl;
    cout << 2 * len << endl;
    cout << 3 * len << endl;
  }

  {
    cout << "---------------------------------" << endl;

    __asm__ volatile(
        // "cntw x21\n\t"
        // "mov  %x[length], x21\n\t"
        "cntw %x[length], ALL, MUL #3\n"
        : [length] "+r"(len)
        :
        :);
    cout << len << endl;

    int cnt = 5;
    int sum = 0, value = 3;
  }

  {
    cout << "---------------------------------" << endl;
    __asm__ volatile(
        "3:                                           \n\t"
        "add %x[sum], %x[sum], %x[value]              \n\t"
        "subs %x[cnt], %x[cnt], #0x1                  \n\t"
        "bgt 3b                                       \n\t"
        "3:                                           \n\t"
        "sub %x[sum], %x[sum], #0x1                   \n\t"
        : [sum] "+r"(sum)
        : [value] "r"(value), [cnt] "r"(cnt)
        : "cc", "memory", "x10");
    cout << cnt << " " << sum << endl;
  }

  {
    cout << "---------------------------------" << endl;
    float a[16] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};
    float b[16] = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};
    float c[16];
    memset(c, 0, 16 * 4);
    __asm__ volatile(
        "mov x10, #0                                    \n"
        "mov x12, #3                                    \n"
        "mov x13, #6                                    \n"
        "mov x11, #8                                    \n"
        "whilelt p0.s, x10, x11                         \n"
        "ld1w {z1.s}, p0/z, [%x[a], x12, LSL  #2]       \n"
        "st1w {z1.s}, p0  , [%x[c], x13, LSL  #2]       \n"
        :
        : [a] "r"(a), [c] "r"(c)
        : "x10", "memory", "z1", "p0");
    for (int i = 0; i < 16; ++i) cout << c[i] << " ";
    cout << endl;
  }

  {
    cout << "---------------------------------" << endl;
    int prev = 1;
    __asm__ volatile(
        "mov x10, %x[prev]          \n\t"
        "lsl %x[prev], x10, #3      \n\t"
        : [prev] "+r"(prev)
        :
        :);
    cout << prev << endl;
  }

  {
    int *a = (int *)alloca(64);
    for (int i = 1; i <= 64; ++i) {
      a[i] = i;
    }
    __asm__ volatile(
        "z0"
        :
        : [a] "+r"(a)
        : "memory", "cc");
  }
}
