import serial, time, os, sys
from collections import defaultdict
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

ser = serial.Serial()

ser.baudrate = 115200
ser.port = 'COM7'
ser.open()

starttime = time.time()

valDict = {p:np.zeros((1,50))[0] for p in ['AccX','AccY','AccZ','GyroX','GyroY','GyroZ','IR','Red']}
t = np.zeros((1,50))[0]
print(valDict.keys())

figPPG, axPPG = plt.subplots(2,1)
axPPG[0].set_title("IR counts")
axPPG[1].set_title("Red counts")
figIMU, axIMU = plt.subplots(2,1)
axIMU[0].set_title("Accelerometer")
axIMU[1].set_title("Gyroscope")

i = len(t) - 1

while True:
    try:
        ser.reset_input_buffer()
        ser.reset_output_buffer()
        line = ser.readline().decode('unicode_escape')
        line = line.replace('\r','')
        line = line.replace('\n','')
        line = line.split(',')
        
        axIMU[0].plot(t,valDict['AccX'],color='tab:blue',label='AccX')
        axIMU[0].plot(t,valDict['AccY'],color='tab:green',label='AccY')
        axIMU[0].plot(t,valDict['AccZ'],color='tab:red',label='AccZ')
        axIMU[1].plot(t,valDict['GyroX'],color='tab:blue',label='GyroX')
        axIMU[1].plot(t,valDict['GyroY'],color='tab:green',label='GyroY')
        axIMU[1].plot(t,valDict['GyroZ'],color='tab:red',label='GyroZ')

        axPPG[0].plot(t,valDict['IR'],color='b',label='IR')
        axPPG[0].set_ylim(valDict['IR'][i]-300,valDict['IR'][i]+300)
        axPPG[1].plot(t,valDict['Red'],color='r',label='Red')
        axPPG[1].set_ylim(valDict['Red'][i]-300,valDict['Red'][i]+300)

        t = np.roll(t,-1)
        t[i] = time.time() - starttime

        for j, p in enumerate(['IR','Red','AccX','AccY','AccZ','GyroX','GyroY','GyroZ']):
            valDict[p] = np.roll(valDict[p],-1)
            valDict[p][i] = float(line[j].replace(f"{p}:",''))

        plt.pause(0.05)
        figPPG.cla()
        figIMU.cla()

    except:
        continue