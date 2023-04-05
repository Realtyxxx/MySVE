g++ -march=armv8.2-a -I./ -fopenmp impl.cpp main.cpp -o main_sve.out && ./main_sve.out $1 $2 $3 $4 $5
