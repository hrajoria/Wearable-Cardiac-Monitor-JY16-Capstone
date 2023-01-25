import serial, time
import matplotlib.pyplot as plt
ser = serial.Serial()

ser.baudrate = 9600
ser.port = 'COM7'
ser.open()
ir = []
red = []
check = []
bpm = []
starttime = time.time()

while time.time() - starttime < 120:
    line = ser.readline().decode('utf-8')
    line = line.replace('\r\n','')
    line = line.split(',')
    print(f"{time.time() - starttime}",end='\r')
    try:
        ir.append(float(line[0].replace('IR=', '')))
        red.append(float(line[1].replace(' Red=', '')))
        check.append(int(line[2].replace(' Found=', '')))
        bpm.append(float(line[3].replace(' BPM=', '')))
    except:
        continue

fig, axs = plt.subplots(2)
axs[0].set_title("IR")
axs[0].plot(ir, color='b')
axs[1].set_title("Red")
axs[1].plot(red, color='r')
fig.show()

fig, axs = plt.subplots(2)
axs[0].set_title("IR")
axs[0].plot(ir, color='b')
axs[1].set_title("Beat Found")
axs[1].plot(check)
fig.show()

fig, axs = plt.subplots(2)
axs[0].set_title("Beat detected")
axs[0].plot(check)
axs[1].set_title("BPM")
axs[1].plot(bpm)
fig.show()
input()