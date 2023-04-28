import serial, time
import matplotlib.pyplot as plt
import numpy as np
from numpy.fft import fft, fftfreq, ifft
from scipy.signal import butter,filtfilt
from scipy.signal import find_peaks

ser = serial.Serial()

ser.baudrate = 115200
ser.port = 'COM15'
ser.open()
ir = []
red = []
starttime = time.time()
timelimit = 5

while time.time() - starttime < timelimit:
    line = ser.readline().decode('utf-8')
    line = line.replace('\r','')
    line = line.replace('\n','')
    line = line.split(',')
    line = [l.strip() for l in line]
    #print(f"{time.time() - starttime}",end='\r')
    try:
        ir.append(float(line[0].replace('IR:', '')))
        red.append(float(line[1].replace('Red:', '')))
    except:
        continue


t = np.linspace(0,timelimit,len(ir))
plt.figure()
"""
fig, ax1 = plt.subplots()
ax1.set_xlabel('time (s)')
ax1.set_ylabel('IR counts')
ax1.set_title("Raw IR signal and peak-peak heart rate measurement")
line, = ax1.plot(t,ir, color='b')
line.set_label("IR")
ax1.tick_params(axis='y')
heartrate = np.zeros_like(np.array(ir))
peaks = find_peaks(ir,prominence=1000/2.8)[0]

for i, peak in enumerate(peaks):
    ax1.plot(t[peak],ir[peak],marker='x')
    if i == 1:
        heartrate[0:peak] = (1/(t[peak] - t[peaks[i-1]])) * 60
    elif i+1 == len(peaks):
        heartrate[peaks[i-1]:] = (1/(t[peak] - t[peaks[i-1]])) * 60
    else:
        heartrate[peaks[i-1]:peak] = (1/(t[peak] - t[peaks[i-1]])) * 60

'''
ax2 = ax1.twinx()
ax2.set_ylabel("Heart rate (BPM)")
line, = ax2.plot(t, heartrate, color='r')
line.set_label("BPM")
ax2.tick_params(axis='y')

fig.tight_layout()
'''

ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
ax2.set_ylabel('Red counts')  # we already handled the x-label with ax1
line, = ax2.plot(t, red, color='r')
line.set_label("Red")
ax2.tick_params(axis='y')

fig.tight_layout()  # otherwise the right y-label is slightly clipped
fig.legend()
"""

plt.subplot(2,1,1)
plt.title("Red and IR")
plt.xlabel("Time (s)")
plt.ylabel("IR counts")
plt.plot(t,ir,label="IR",color='b')
plt.legend()

plt.subplot(2,1,2)
plt.xlabel("Time (s)")
plt.ylabel("Red counts")
plt.plot(t, red, label="Red", color='r')
plt.legend()
plt.show(block=False)



N = len(ir)
fs = (N-1)/(t[-1]-t[0])

freqs = np.linspace(0, fs/2, int(N/2))

fftIR = fft(np.array(ir) - np.average(np.array(ir)))
fftRed = fft(np.array(red) - np.average(red))
y = 2/N * np.abs(fftIR)[0:int(N/2)]
yr = 2/N * np.abs(fftRed)[0:int(N/2)]
nyq = fs*2

plt.figure()
plt.subplot(2,1,1)
plt.plot(freqs, y, label="IR", color='tab:blue')
plt.title(f"FFT signal analysis (sampling freq = {fs})")
plt.xlabel("Freq (Hz)")
plt.ylabel("Magnitude")
plt.legend()

plt.subplot(2,1,2)
plt.plot(freqs,yr, label='Red', color='tab:red')
plt.xlabel("Freq (Hz)")
plt.ylabel("Magnitude")
plt.legend()

def butter_lowpass_filter(data, data2, cutoff, freqs):
    '''
    normal_cutoff = cutoff / nyq
    # Get the filter coefficients 
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    y = filtfilt(b, a, data)
    '''
    try:
        i = np.where(freqs >= cutoff)[0][0]
        data[i:] = 0
        data2[i:] = 0
    except:
        print("No data to filter")
    return data, data2

cutoff = 50
y, fftIR = butter_lowpass_filter(y, fftIR, cutoff, freqs)
yr, fftRed = butter_lowpass_filter(yr, fftRed, cutoff, freqs)

plt.figure()
plt.subplot(2,1,1)
plt.plot(freqs, y, label="IR", color='tab:blue')
plt.title(f"FFT signal analysis (low-pass filter wc = {cutoff}Hz)")
plt.xlabel("Freq (Hz)")
plt.ylabel("Magnitude")
plt.legend()

plt.subplot(2,1,2)
plt.plot(freqs, yr, label='Red', color='tab:red')
plt.xlabel("Freq (Hz)")
plt.ylabel("Magnitude")
plt.legend()

ynew = ifft(fftIR) + np.average(ir)
yrnew = ifft(fftRed) + np.average(red)
t = np.linspace(0,timelimit,len(ynew))
plt.figure()
"""
fig, ax1 = plt.subplots()
line, = ax1.plot(t, ynew, color="tab:blue")
line.set_label("IR")
ax1.set_title("Reconstructed noise filtered PPG")
ax1.set_xlabel("Time (s)")
ax1.set_ylabel("IR counts")

heartrate = np.zeros_like(np.array(ir))
peaks = find_peaks(ynew,prominence=1000/9.8)[0]

for i, peak in enumerate(peaks):
    ax1.plot(t[peak],ynew[peak],marker='x')
    if i == 1:
        heartrate[0:peak] = (1/(t[peak] - t[peaks[i-1]])) * 60
    elif i+1 == len(peaks):
        heartrate[peaks[i-1]:] = (1/(t[peak] - t[peaks[i-1]])) * 60
    else:
        heartrate[peaks[i-1]:peak] = (1/(t[peak] - t[peaks[i-1]])) * 60

'''
ax2 = ax1.twinx()
ax2.set_ylabel("Heart rate (BPM)")
line, = ax2.plot(t, heartrate, color='r')
line.set_label("BPM")
ax2.tick_params(axis='y')

fig.tight_layout()
'''
ax2 = ax1.twinx()
ax2.set_ylabel("Red counts")
line, = ax2.plot(t, yrnew, color='tab:red')
line.set_label("Red")
ax2.tick_params(axis='y')

fig.legend()
"""
plt.subplot(2,1,1)
plt.plot(t,ynew,label="IR",color='tab:blue')
plt.xlabel("Time (s)")
plt.ylabel("IR counts")
plt.title("Reconstructed noise filtered PPG")
plt.legend()

plt.subplot(2,1,2)
plt.plot(t,yrnew,label="Red",color='tab:red')
plt.xlabel("Time (s)")
plt.ylabel("Red counts")
plt.legend()
plt.show(block=False)

print()