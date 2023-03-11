CC = g++
flags =   -I../src/nhwc_depthfirst_kernel -I../src/ -I./


.PHONY: all 

all: test

test:  nhwc_depthfirst_pooling_nhwc_max_generic_kernel.o naive_nhwc_depthfirst_generic_max_pooling.o nhwc_depthfirst_pooling_generic.o main.o
	${CC} ${flags} -o $@ $^

naive_nhwc_depthfirst_generic_max_pooling.o : naive_nhwc_depthfirst_generic_max_pooling.cc
	${CC} -O0 -c -o $@ $^

nhwc_depthfirst_pooling_nhwc_max_generic_kernel.o: ../src/nhwc_depthfirst_kernel/nhwc_depthfirst_pooling_nhwc_max_generic_kernel.cc
	${CC} ${flags} -c -o $@ $^ -march=armv8.2-a+sve

# nhwc_depthfirst_pooling_naive_kernel.o: ../src/nhwc_depthfirst_kernel/nhwc_depthfirst_pooling_naive_kernel.cc
# 	${CC} ${flags} -c -o $@ $^

nhwc_depthfirst_pooling_generic.o :../src/nhwc_depthfirst_pooling_generic.cc
	${CC} ${flags} -c -o $@ $^

nhwc_depthfirst_pooling_nhwc_max_generic.o: ../src/nhwc_depthfirst_pooling_generic.cc
	${CC} ${flags} -c -o $@ $^

main.o: main.cc
	${CC} ${flags} -c -o $@ $^


clean:
	rm -rf *.o pt main qemu*
