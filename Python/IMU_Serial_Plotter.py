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
    "Leave Alone", "Move in x-direction", "Move in y-direction", "Move in z-direction", 
    "Rotate in x-axis", "Rotate in y-axis", "Rotate in z-axis"
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
line, = axs[0].plot(dictPoints['acc_x'])
line.set_label("Acc X")
line, = axs[0].plot(dictPoints['acc_y'])
line.set_label("Acc Y")
line, = axs[0].plot(dictPoints['acc_z'])
line.set_label("Acc Z")
line, = axs[0].plot(dictPoints['mag'])
line.set_label("Acc Mag")
axs[0].axvline(x=100)
axs[0].axvline(x=200)
axs[0].axvline(x=300)
axs[0].legend()

axs[1].set_title("Rotational measurements")
axs[1].set(ylabel="deg/s")
line, = axs[1].plot(dictPoints['gyro_x'])
line.set_label("Rot X")
line, = axs[1].plot(dictPoints['gyro_y'])
line.set_label("Rot Y")
line, = axs[1].plot(dictPoints['gyro_z'])
line.set_label("Rot Z")
axs[1].axvline(x=400)
axs[1].axvline(x=500)
axs[1].axvline(x=600)
axs[1].legend()


fig.show()
print()