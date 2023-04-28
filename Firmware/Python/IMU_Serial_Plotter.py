import serial, time
import matplotlib.pyplot as plt
from collections import defaultdict
ser = serial.Serial('COM7')
#ser.open()

def addToList(ser, dictPoints):
    line = ser.readline().decode('utf-8').replace('\r\n', '').split(',')
    dictPoints['acc_x'].append(float(line[0].replace('Acceleration_x:','')))
    dictPoints['acc_y'].append(float(line[1].replace('Acceleration_y:','')))
    dictPoints['acc_z'].append(float(line[2].replace('Acceleration_z:','')))
    dictPoints['mag'].append(float(line[3].replace('Magnitude:','')))
    dictPoints['gyro_x'].append(float(line[4].replace('Gyro_x:','')))
    dictPoints['gyro_y'].append(float(line[5].replace('Gyro_y:','')))
    dictPoints['gyro_z'].append(float(line[6].replace('Gyro_z:','')))

dictPoints = defaultdict(list)
prompt = (
    "Move in x-direction", "Move in y-direction", "Move in z-direction", 
    "Rotate in x-axis", "Rotate in y-axis", "Rotate in z-axis", "Leave alone"
)

for p in prompt:
    print(p)
    for i in range(5,0,-1):
        print(f"Start in: {i}", end='\r')
        time.sleep(0.5)
    print()
    for i in range(100):
        print(f"Point {i} collected",end='\r')
        addToList(ser, dictPoints)


fig, axs = plt.subplots(2)
axs[0].set_title("Acceleration measurements")
axs[0].set(ylabel="m/s^2")
axs[0].plot(dictPoints['acc_x'])
axs[0].plot(dictPoints['acc_y'])
axs[0].plot(dictPoints['acc_z'])
axs[0].plot(dictPoints['mag'])

axs[1].set_title("Rotational measurements")
axs[1].set(ylabel="rad/s^2")
axs[1].plot(dictPoints['gyro_x'])
axs[1].plot(dictPoints['gyro_y'])
axs[1].plot(dictPoints['gyro_z'])

fig.show()
print()