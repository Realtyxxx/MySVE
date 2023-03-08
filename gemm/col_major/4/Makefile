ifndef GCC_PATH
GCC_PATH=/usr/local/gcc-11.1.0
endif

GCC=${GCC_PATH}/bin/gcc
CFLAGS=-g -Wall -march=armv8.2-a+sve -O2 -I./
CXXFLAGS=-g -Wall -march=armv8.2-a+sve -O2 -I./
LDFLAGS=

COMPILE.c=$(GCC) $(CFLAGS) -c
.o:.cc
.o:.S

all: main
main : main.o gemm.o sve_4x4.o util.o sve_8x4.o
main.o : main.cc
gemm.o : gemm.cc
sve_4x4.o : sve_4x4.cc
sve_8x4.o : sve_8x4.cc
util.o : util.c


.PHONY: clean
clean:
	rm -f *.o main qemu* *.o
