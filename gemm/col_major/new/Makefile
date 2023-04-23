ifndef GCC_PATH
GCC_PATH=/usr/local/gcc-11.1.0
endif

SVE_FLAGS = -msve-vector-bits=256
GCC=${GCC_PATH}/bin/gcc

OMP=
# OMP+= -fopenmp -DMULTI_THREADS

CFLAGS= -g -Wall -march=armv8.2-a+sve -O2 -I./   $(OMP)
CXXFLAGS= -Wall -march=armv8.2-a+sve -O2 -I./  

COMPILE.c=$(GCC) $(CFLAGS) -c

.o:.cc
.o:.c
.o:.S

all: main.out
main.out : main.o gemm.o  util.o sgemm_sve_2vx8.o sgemm_sve_16x8.o sgemm_naive.o
	$(GCC) $(CFLAGS) main.o gemm.o  util.o sgemm_sve_2vx8.o sgemm_sve_16x8.o sgemm_naive.o -o main.out  $(OMP)
main.o : main.c
gemm.o : gemm.c
sgemm_sve_2vx8.o : sgemm_sve_2vx8.c
sgemm_sve_16x8.o : sgemm_sve_16x8.c
util.o : util.c

sgemm_naive.o : sgemm_naive.c
	$(GCC) -c sgemm_naive.c -o sgemm_naive.o  $(OMP)

.PHONY: clean
clean:
	rm -f *.o main qemu* *.o
