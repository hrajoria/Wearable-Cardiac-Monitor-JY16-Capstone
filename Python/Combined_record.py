import serial, time, sys, os
ser = serial.Serial()

ser.baudrate = 9600
ser.port = 'COM7'
ser.open()
ir = []
red = []
check = []
bpm = []
starttime = time.time()
path = os.path.join(os.path.dirname(sys.argv[0]),"combined3.txt")

with open(path, "w") as f:
    line = ser.readline().decode('utf-8')
    line = line.replace('\r\n','')
    try:
        while True:
            print(line)
            f.write(f"{line}\n")
    except:
        print("Damn")