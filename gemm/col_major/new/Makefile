ifndef GCC_PATH
GCC_PATH=/usr/local/gcc-11.1.0
endif

SVE_FLAGS = -msve-vector-bits=256

GCC=${GCC_PATH}/bin/gcc
CFLAGS=  -Wall -march=armv8.2-a+sve -O2 -I./  
LDFLAGS=
# LDFLAGS+= -fopenmp
# CFLAGS+= -fopenmp -DMULTI_THREADS=1


COMPILE.c=$(GCC) $(CFLAGS) -c

.o:.cc
.o:.c
.o:.S

.PHONY: clean cleanlog all test_block

all : test_block
test_block : main_test.o gemm.o  util.o sgemm_sve_2vx8.o sgemm_sve_16x8.o sgemm_naive.o
	gcc $(CFLAGS) main_test.o gemm.o  util.o sgemm_sve_2vx8.o sgemm_sve_16x8.o sgemm_naive.o -o test_block
main_test.o : main_test.c
main.o : main.c
gemm.o : gemm.c
sgemm_sve_2vx8.o : sgemm_sve_2vx8.c
sgemm_sve_16x8.o : sgemm_sve_16x8.c
util.o : util.c
sgemm_naive.o : sgemm_naive.c

clean:
	rm -f *.o main qemu* *.o
cleanlog:
	rm -rf *.log




# main : main.o gemm.o  util.o sgemm_sve_2vx8.o sgemm_sve_16x8.o sgemm_naive.o
