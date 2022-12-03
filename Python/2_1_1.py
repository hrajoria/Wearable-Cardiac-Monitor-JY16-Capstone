import serial, time, os, sys
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

ser = serial.Serial('COM7')
avgbpm = []

starttime = time.time()
while time.time() - starttime < 30:
    line = ser.readline().decode('utf-8')
    line = line.replace('\r\n','')
    line = line.split(',')
    print(f"{time.time() - starttime}", end='\r')
    try:
        avgbpm.append(float(line[2].replace(' Avg BPM=', '')))
    except:
        continue

var = []
oldbpm = None
for bpm in avgbpm:
    if not oldbpm:
        oldbpm = bpm
    else:
        var.append(float(abs(oldbpm - bpm) / float(abs(bpm + oldbpm)/2)*100))
        oldbpm = bpm
var.append(var[len(var)-1])

plt.plot(var)
plt.title("PPG Heart Rate Variance Calculator")
plt.xlabel("Data point number")
plt.ylabel("Percent difference")
plt.show()

df = pd.DataFrame({'BPM':bpm, 'Variance':var})
path = os.path.join(os.path.dirname(sys.argv[0]), '2_1_1_test.csv')
df.to_csv(path)