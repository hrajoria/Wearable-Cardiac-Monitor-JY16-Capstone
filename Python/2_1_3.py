import serial, time, os, sys
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

ser = serial.Serial('COM7')
avgbpm = []
times = []
starttime = time.time()
while time.time() - starttime < 30:
    line = ser.readline().decode('utf-8')
    line = line.replace('\r\n','')
    line = line.split(',')
    try:
        avgbpm.append(float(line[2].replace(' Avg BPM=', '')))
        times.append(time.time() - starttime)
        print(f"{time.time() - starttime}\t{float(line[2].replace(' Avg BPM=', ''))}", end='\r')
    except:
        continue

input("Do something to increase heart rate. Press enter when done. ")
restartTime = time.time() - starttime
try:
    while True:
        line = ser.readline().decode('utf-8')
        line = line.replace('\r\n','')
        line = line.split(',')
        print(f"{time.time() - starttime}", end='\r')
        try:
            avgbpm.append(float(line[2].replace(' Avg BPM=', '')))
            times.append(time.time() - starttime)
            print(f"{time.time() - starttime}\t{float(line[2].replace(' Avg BPM=', ''))}", end='\r')
        except:
            continue
except KeyboardInterrupt:
    pass

plt.plot(times, avgbpm)
plt.axvline(restartTime, color='r', label='Exercise')
plt.title("PPG BPM Measurement before and after exercise")
plt.xlabel("Time(s)")
plt.ylabel("BPM")
plt.show()

df = pd.DataFrame({'Time':times, 'BPM':avgbpm})
path = os.path.join(os.path.join(os.path.dirname(os.path.dirname(sys.argv[0])),'Data Collection'), '2_1_3_test.csv')
df.to_csv(path)