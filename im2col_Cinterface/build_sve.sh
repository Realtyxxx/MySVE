gcc -g -march=armv8.2-a+sve -I./ -DMULTI_THREADS  gemm.c util.c sgemm_sve_2vx8.c impl.c main.c -fopenmp -o main_sve.out && ./main_sve.out $1 $2 $3 $4 $5 $6 $7 $8
