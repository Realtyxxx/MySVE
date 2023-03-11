#!/bin/bash

# 定义循环范围和跨度
m_start=512
m_end=768
m_step=32

n_start=32
n_end=256
n_step=32

k_start=256
k_end=512
k_step=16

current_time=$(date +"%Y%m%d-%H%M%S")
runlog="block_${current_time}.log"

echo "${runlog}"

# exit
# 循环三个变量
for ((m = m_start; m <= m_end; m += m_step)); do
	for ((n = n_start; n <= n_end; n += n_step)); do
		for ((k = k_start; k <= k_end; k += k_step)); do
			# 在最内层循环中执行程序
			# echo "Running program with m=$m n=$n k=$k mr=$mr nr=$nr"
			echo -n "Running program with m=$m n=$n k=$k " >>${runlog}
			# 在这里添加你要执行的程序
			./test_block 512 256 256 1 0 $m $n $k 0 >>"${runlog}"
		done
	done
done

# for ((mr = mr_start; mr <= mr_end; mr += mr_step)); do
# 	for ((nr = nr_start; nr <= nr_end; nr += nr_step)); do
# 		# 在最内层循环中执行程序
# 		echo -n "Running program with m=$m n=$n k=$k mr=$mr nr=$nr" >>${runlog}
# 		./test_block 512 512 512 1 0 256 256 256 $mr $nr 0 >>"${runlog}"
# 		# 在这里添加你要执行的程序
# 	done
# done
