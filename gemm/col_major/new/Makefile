ifndef GCC_PATH
GCC_PATH=/usr/local/gcc-11.1.0
endif

SVE_FLAGS = -msve-vector-bits=256

GCC=${GCC_PATH}/bin/gcc
CFLAGS=  -Wall -march=armv8.2-a+sve -O2 -I./  
# CXXFLAGS= -Wall -march=armv8.2-a+sve -O0 -I./  
LDFLAGS=
# LDFLAGS+= -fopenmp
# CFLAGS+= -fopenmp -DMULTI_THREADS=1


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

.PHONY: clean
clean:
	rm -f *.o main qemu* *.o
