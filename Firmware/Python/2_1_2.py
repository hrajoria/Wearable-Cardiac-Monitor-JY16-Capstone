import serial, time, os, sys
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

ser = serial.Serial('COM7')
bpmreading = []
s9reading = []

for i in range(5):
    input(f"Hit enter to start trial {i}")
    avgbpm = []
    try:
        while True:
            line = ser.readline().decode('utf-8')
            line = line.replace('\r\n','')
            line = line.split(',')
            try:
                avgbpm.append(float(line[2].replace(' Avg BPM=', '')))
            except:
                continue
    except KeyboardInterrupt:
        pass
    s9reading.append(float(input("Reading from S9: ")))
    bpmreading.append(sum(avgbpm)/len(avgbpm))

errors = []
for i in range(len(bpmreading)):
    errors.append(abs(bpmreading[i]-s9reading[i])/(abs(bpmreading[i]+s9reading[i])/2))

df = pd.DataFrame({'PPG measurement':bpmreading, 'External PPG Measurment':s9reading, 'Error': errors})
path = os.path.join(os.path.join(os.path.dirname(os.path.dirname(sys.argv[0])),'Data Collection'), '2_1_2_test.csv')
df.to_csv(path)