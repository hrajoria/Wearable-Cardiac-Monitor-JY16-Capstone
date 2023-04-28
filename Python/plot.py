import matplotlib.pyplot as plt
import numpy as np
import sys, os

path = os.path.join(os.path.dirname(sys.argv[0]), f"PPG_ECG_test_1_rest_1min.txt")
ir = []
with open(path, 'r') as f:
    lines = f.readlines()
    for line in lines:
        ir.append(float(line.split(',')[0]))

plt.plot(ir)
plt.show(block=True)