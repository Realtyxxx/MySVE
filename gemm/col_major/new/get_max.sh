#!/bin/bash

# 假设我们要从文件中抽取第2列的最大值及其所在的行
filename=$1
column=7

# 使用awk命令找到第column列的最大值及其所在的行
awk -v column="$column" 'BEGIN { max = -1000000 } { if ($column > max) { max = $column; line = $0 } } END { print line }' "$filename" | grep "$max"

