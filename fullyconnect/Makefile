ifndef GCC_PATH
GCC_PATH=/usr/local/gcc-11.1.0
endif

SVE_FLAGS = -msve-vector-bits=256
GCC=${GCC_PATH}/bin/gcc

OMP=
OMP+= -fopenmp -DMULTI_THREADS

CFLAGS= -g -Wall -march=armv8.2-a+sve -O2 -I./   $(OMP)
CXXFLAGS= -Wall -march=armv8.2-a+sve -O2 -I./  

COMPILE.c=$(GCC) $(CFLAGS) -c

.o:.cc
.o:.c
.o:.S


all: main.out
main.out : main.o gemv.o  
	$(GCC) $(CFLAGS) main.o gemv.o    -o main.out  $(OMP)
main.o : main.c
gemv.o : gemv.c

run:
	make clean && make -j

.PHONY: all clean run
clean:
	rm -f *.o main qemu* *.o
