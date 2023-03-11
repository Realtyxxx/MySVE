#include <arm_sve.h>
#include <stdio.h>

int main() {
	printf("sve vector has %d bits\n", 8 * svcntb());
}
