import serial, time
import matplotlib.pyplot as plt
ser = serial.Serial()

ser.baudrate = 9600
ser.port = 'COM7'
ser.open()
bpm = []
avgbpm = []
for i in range(10000):
    line = ser.readline().decode('utf-8')
    line = line.replace('\r\n','')
    line = line.split(',')
    print(f"{i}\r")
    try:
        bpm.append(float(line[1].replace(' BPM=', '')))
        avgbpm.append(float(line[2].replace(' Avg BPM=', '')))
    except:
        continue

plt.plot(bpm, color='b')
plt.plot(avgbpm, color='r')
plt.show()
input()