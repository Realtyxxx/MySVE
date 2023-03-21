#!/bin/bash

# 定义循环范围和跨度
m_start=128
m_end=12800
m_step=128

n_start=8
n_end=256
n_step=8

k_start=10
k_end=600
k_step=20

current_time=$(date +"%Y%m%d-%H%M%S")
runlog="block_${current_time}.log"

echo "${runlog}"

# exit
# 循环三个变量
for ((m = m_start; m <= m_end; m += m_step)); do
	for ((n = n_start; n <= n_end; n += n_step)); do
		for ((k = k_start; k <= k_end; k += k_step)); do
			# 在最内层循环中执行程序
			echo -n "Running program with m=$m n=$n k=$k" >>${runlog}
			./test_block.out 256 256 256 1 0 $m $n $k 0 >>"${runlog}"
			# 在这里添加你要执行的程序
		done
	done
done
