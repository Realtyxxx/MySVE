CC = g++
flags =  -march=armv8.2-a+sve -I../src/nhwc_depthfirst_kernel -O0


.PHONY: all pt obj

all: test

test:  nhwc_depthfirst_pooling_nhwc_max_generic_kernel.o nhwc_depthfirst_pooling_naive_kernel.o  main.o
	${CC} ${flags} -o $@ $^

nhwc_depthfirst_pooling_nhwc_max_generic_kernel.o: ../src/nhwc_depthfirst_kernel/nhwc_depthfirst_pooling_nhwc_max_generic_kernel.cc
	${CC} ${flags} -c -o $@ $^

nhwc_depthfirst_pooling_naive_kernel.o: ../src/nhwc_depthfirst_kernel/nhwc_depthfirst_pooling_naive_kernel.cc
	${CC} ${flags} -c -o $@ $^

main.o: main.cc
	${CC} ${flags} -c -o $@ $^

obj:nhwc_depthfirst_pooling_naive_kernel.o

clean:
	rm -rf *.o pt main
