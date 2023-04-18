#!/bin/bash

# 自定义三个循环的范围和步长
start_i=64
end_i=256
step_i=64

start_j=1024
end_j=4096
step_j=512

start_k=64
end_k=256
step_k=64

# 使用自定义的循环范围和步长生成乘法表矩阵
for ((i = start_i; i <= end_i; i += step_i)); do
	for ((j = start_j; j <= end_j; j += step_j)); do
		for ((k = start_k; k <= end_k; k += step_k)); do
			# 计算乘积并打印
			./my.out 4096 4096 4096 1 0 $i $j $k 0
		done
		# 在每行结束时打印换行符
		echo
	done
	# 在每个矩阵之间打印两个换行符
	echo
done
