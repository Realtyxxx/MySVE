g++ -march=armv8.2-a+sve -g test_bnorm.cc bnorm_nhwc.cpp bnorm_nchw.cpp naive_bnorm.cpp -o test_bnorm.out
./test_bnorm.out $1 $2
