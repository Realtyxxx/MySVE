ifndef GCC_PATH
GCC_PATH=/usr/local/gcc-11.1.0
endif

SVE_FLAGS = -msve-vector-bits=256
GCC=${GCC_PATH}/bin/gcc
CFLAGS= -Wall -march=armv8.2-a+sve -O0 -I./  -fopenmp
CXXFLAGS= -Wall -march=armv8.2-a+sve -O0 -I./  -fopenmp 

LDFLAGS=-fopenmp
#  -fopenmp
COMPILE.c=$(GCC) $(CFLAGS) -c

.o:.cc
.o:.c
.o:.S

all: main
main : main.o gemm.o  util.o sgemm_sve_2vx8.o sgemm_sve_16x8.o sgemm_naive.o

main.o : main.c
gemm.o : gemm.c
sgemm_sve_2vx8.o : sgemm_sve_2vx8.c
sgemm_sve_16x8.o : sgemm_sve_16x8.c
util.o : util.c

sgemm_naive.o : sgemm_naive.c
	$(GCC) -c sgemm_naive.c -o sgemm_naive.o -fopenmp

.PHONY: clean
clean:
	rm -f *.o main qemu* *.o
