import serial, time, os, sys
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from collections import defaultdict

def addToList(ser, dictPoints):
    line = ser.readline().decode('utf-8').replace('\r\n', '').split(',')
    dictPoints['acc_x'].append(float(line[0].replace('Acceleration_x:','')))
    dictPoints['acc_y'].append(float(line[1].replace('Acceleration_y:','')))
    dictPoints['acc_z'].append(float(line[2].replace('Acceleration_z:','')))
    dictPoints['mag'].append(float(line[3].replace('Magnitude:','')))
    dictPoints['gyro_x'].append(float(line[4].replace('Gyro_x:','')))
    dictPoints['gyro_y'].append(float(line[5].replace('Gyro_y:','')))
    dictPoints['gyro_z'].append(float(line[6].replace('Gyro_z:','')))
    dictPoints['gyro_mag'].append(float(line[7].replace('Gyro_mag:','')))

def variance(x, y):
    if x == 0 and y == 0: return 0
    return (abs(x) - abs(y))*100

def calcVariance(varPoints, dictPoints):
    oldX, oldY, oldZ, oldRotX, oldRotY, oldRotZ = None,None,None,None,None,None
    for i in range(len(dictPoints['acc_x'])):
        if i != 0:
            varPoints['acc_x var'].append(variance(dictPoints['acc_x'][i], oldX))
            varPoints['acc_y var'].append(variance(dictPoints['acc_y'][i], oldY))
            varPoints['acc_z var'].append(variance(dictPoints['acc_z'][i], oldZ))
            varPoints['gyro_x var'].append(variance(dictPoints['gyro_x'][i], oldRotX))
            varPoints['gyro_y var'].append(variance(dictPoints['gyro_y'][i], oldRotY))
            varPoints['gyro_z var'].append(variance(dictPoints['gyro_z'][i], oldRotZ))
        oldX = dictPoints['acc_x'][i]
        oldY = dictPoints['acc_y'][i]
        oldZ = dictPoints['acc_z'][i]
        oldRotX = dictPoints['gyro_x'][i]
        oldRotY = dictPoints['gyro_y'][i]
        oldRotZ = dictPoints['gyro_z'][i]

def calcError(dictPoints, errPoints):
    g = 9.81
    for i in range(len(dictPoints['mag'])):
        errPoints['mag err'].append(variance(dictPoints['mag'][i], g))
        errPoints['gyro_mag err'].append(variance(dictPoints['gyro_mag'][i], 0))

ser = serial.Serial('COM7')
avgbpm = []
times = []
starttime = time.time()
dictPoints = defaultdict(list)
varPoints = defaultdict(list)
errPoints = defaultdict(list)

while time.time() - starttime < 10:
    print(f"{time.time() - starttime}", end='\r')
    addToList(ser, dictPoints)

calcVariance(varPoints, dictPoints)
calcError(dictPoints, errPoints)

fig, axs = plt.subplots(2)
axs[0].set_title("Acceleration and rotation variances")
axs[0].set(ylabel="Percent Variance")
lines = []
for item in varPoints.values():
    lines.append(axs[0].plot(item))
axs[0].legend(lines, varPoints.keys())

axs[1].set_title("Magnitude of Acceleration and Rotational Acceleration Error")
axs[1].set(ylabel="Percent Error")
for item in errPoints.values():
    axs[1].plot(item)
fig.show()

df1 = pd.DataFrame(varPoints)
df2 = pd.DataFrame(errPoints)
result = pd.concat([df1, df2], axis=1, join='inner')
path = os.path.join(os.path.join(os.path.dirname(os.path.dirname(sys.argv[0])),'Data Collection'), '2_2_1_test.csv')
result.to_csv(path)
