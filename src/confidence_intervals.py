import re
import sys

import numpy as np
import scipy.stats as stat

if len(sys.argv) != 2:
    print("Usage: python confidence_intervals.py <file_name>")
    exit(1)

myf = open(sys.argv[1])

# Read the data
t = myf.readlines()

print("The following shows respective confidence intervals (in kBps)\n")

for i in range(9):
    print(t[21 * i], end='')
    meta = []  # List of time taken for 20 runs
    for j in range(20):
        time = float((re.findall(r'\d+\.\d+', t[21 * i + j + 1]))[0])
        meta.append(5 * 1024 / time)
    # Confidence interval of 90% with degree of freedom 19 (one less than number of runs)
    interval = stat.t.interval(alpha=0.9, df=len(meta) - 1, loc=np.mean(meta), scale=stat.sem(meta))
    print(interval)
    print("")
