import serial, time
import matplotlib.pyplot as plt
import numpy as np
ser = serial.Serial()

ser.baudrate = 9600
ser.port = 'COM7'
ser.open()

starttime = time.time()

t = np.zeros((1,50))[0]
y = np.zeros((1,50))[0]
i = len(t)-1

vl, vh = 0,0

while True:
    try:
        ser.reset_input_buffer()
        ser.reset_output_buffer()
        line = ser.readline().decode('unicode_escape')
        line = line.replace('\r','')
        line = line.replace('\n','')
        line = float(line)/1000
        vh = max(vh, line)
        vl = min(vl, line)
        t = np.roll(t,-1)
        y = np.roll(y,-1)
        t[i] = time.time() - starttime
        y[i] = line

        plt.plot(t,y)
        plt.ylim([vl-1,vh+1])
        plt.pause(0.05)
        plt.clf()

    except:
        continue
