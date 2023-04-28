import serial, time, os, sys
from collections import defaultdict
import pandas as pd

ser = serial.Serial()

ser.baudrate = 115200
ser.port = 'COM7'
ser.open()
imuDict = defaultdict(list)

def Record_Value(valDict):
    line = ser.readline().decode('utf-8')
    line = line.replace('\r','').replace('\n','')
    line = line.split(',')
    try:
        valDict['AccX'].append(float(line[0].replace('AccX:','')))
        valDict['AccY'].append(float(line[1].replace('AccY:','')))
        valDict['AccZ'].append(float(line[2].replace('AccZ:','')))
        valDict['GyroX'].append(float(line[3].replace('GyroX:','')))
        valDict['GyroY'].append(float(line[4].replace('GyroY:','')))
        valDict['GyroZ'].append(float(line[5].replace('GyroZ:','')))
    except:
        return

starttime = time.time()
timelimit = 20
for p in ['Side 1', 'Side 2', 'Side 3']:
    input(p)
    starttime = time.time()
    while time.time() - starttime <= timelimit:
        Record_Value(imuDict)


path = os.path.dirname(sys.argv[0])
path = os.path.abspath(r"{}/Data/IMU/imu_static_{}s.csv".format(path,timelimit))
pd.DataFrame(imuDict).to_csv(path,index=False)
