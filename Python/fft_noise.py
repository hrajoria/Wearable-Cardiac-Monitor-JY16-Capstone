import numpy as np
from numpy.fft import fft, ifft, rfft, rfftfreq
import os, sys
import matplotlib.pyplot as plt

path = os.path.join(os.path.dirname(sys.argv[0]),"PPG_30s_test.txt")
ir = []
red = []
with open(path, "r") as f:
    lines = f.readlines()
    for line in lines:
        ir.append(float(line.split(',')[0]))
        red.append(float(line.split(',')[1]))

sr = 3200

ts = 30.0/len(ir)
t = np.linspace(0,30,len(ir))

yf   = rfft(ir)
xf   = rfftfreq(len(t),t[1]-t[0])
plt.plot(xf,np.abs(yf))
plt.show(block=True)