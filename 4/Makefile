ifndef GCC_PATH
GCC_PATH=/usr/local/gcc-11.1.0
endif

GCC=${GCC_PATH}/bin/gcc
CFLAGS=-g -Wall -march=armv8.2-a+sve -O2 -I./
LDFLAGS=

COMPILE.c=$(GCC) $(CFLAGS) -c
.o:.c
.o:.S

all: main
main : main.o gemm.o sve_4x4.o util.o sve_8x4.o
main.o : main.c
gemm.o : gemm.c
sve_4x4.o : sve_4x4.c
sve_8x4.o : sve_8x4.c
util.o : util.c


.PHONY: clean
clean:
	rm -f *.o main qemu*
