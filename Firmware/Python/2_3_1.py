import time
import serial, os, sys
import pandas as pd
import matplotlib.pyplot as plt

ser = serial.Serial('COM7')
starttime = time.time()
times = []
instructions = []
i = 0
while time.time() - starttime < 1:
    try:
        line = ser.readline().decode('utf-8').replace('\r\n', '').split(',')
        i += 1
        instructions.append(i)
        times.append(time.time() - starttime)
    except:
        continue

plt.plot(times, instructions)
plt.title("Number of serial outputs vs time")
plt.xlabel("Time (s)")
plt.ylabel("Serial outputs")
plt.show()

df = pd.DataFrame({"Times": times, "Serial Outputs": instructions})
path = os.path.join(os.path.join(os.path.dirname(os.path.dirname(sys.argv[0])),'Data Collection'), '2_3_1_test.csv')
df.to_csv(path)