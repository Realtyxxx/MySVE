#include <arm_sve.h>
#include <stdio.h>

int main() {
  printf("%ld bits", svcntb() * 8);
}
