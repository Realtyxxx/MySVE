# #!/bin/bash
#
# # 假设我们要从文件中抽取第2列的最大值及其所在的行
# filename=$1
# column=7
#
# # 使用awk命令找到第column列的最大值及其所在的行
# awk -v column="$column" 'BEGIN { max = -1000000 } { if ($column > max) { max = $column; line = $0 } } END { print line }' "$filename" | grep "$max"
#

import csv
import sys

filename = ''
if len(sys.argv) < 2:
    print("Please provide a filename as an argument.")
else:
    filename = sys.argv[1]
    print("The filename you provided is:", filename)

with open(filename, 'r') as csvfile:
    reader = csv.reader(csvfile)
    max_value = float('-inf')
    max_row = None
    for row in reader:
        if row[1]:  # assuming the seventh column is indexed as 6
            value = float(row[1])
            if value > max_value:
                max_value = value
                max_row = row

if max_row:
    print(f"The maximum value is {max_value} and it is found in row: {max_row}")
else:
    print("The CSV file is empty or there is no value in the seventh column.")
