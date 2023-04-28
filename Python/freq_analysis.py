import csv, os, sys
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from numpy.fft import fft, ifft
from scipy.signal import find_peaks

def ref_ecg_ppg_test(path):
    df = pd.read_csv(os.path.abspath(r"{}/ref_ECG_PPG_Test/test1_withECG.csv".format(path)))

    plt.subplot(2,1,1)
    plt.plot(df['Time'],df['ECG'],color='tab:red')

    peaks = find_peaks(df['ECG'],prominence=90,distance=20)
    for i, peak in enumerate(peaks[0]):
        plt.plot(df['Time'][peak],df['ECG'][peak],marker='x',color='k')
    plt.title(f"Reference ECG peak count = {len(peaks[0])}")
    plt.gca().get_yaxis().set_visible(False)

    plt.subplot(2,1,2)
    plt.plot(df['Time'],df['PPG2'],color='tab:blue')
    peaks = find_peaks(df['PPG2'],prominence=175)
    for i, peak in enumerate(peaks[0]):
        plt.plot(df['Time'][peak],df['PPG2'][peak],marker='x',color='k')
    plt.show(block=False)
    plt.title(f"PPG peak number = {len(peaks[0])}")
    plt.gca().get_yaxis().set_visible(False)
    plt.xlabel("Time (s)")


    t = df['Time'].to_numpy()
    ppg = df['PPG2'].to_numpy()
    ecg = df['ECG'].to_numpy()

    N = len(t)
    fs = (N-1)/(t[-1]-t[0])

    freqs = np.linspace(0, fs/2, int(N/2))

    fftPPG = fft(np.array(ppg)-np.average(ppg))
    fftECG = fft(np.array(ecg)-np.average(ecg))
    yPPG = 2/N * np.abs(fftPPG)[0:int(N/2)]
    yECG = 2/N * np.abs(fftECG)[0:int(N/2)]
    nyq = fs*2

    plt.figure()
    plt.subplot(2,1,1)
    plt.plot(freqs, yPPG, label="PPG", color='tab:blue')
    plt.title(f"FFT signal analysis (sampling freq = {fs})")
    plt.xlabel("Freq (Hz)")
    plt.ylabel("Magnitude")
    plt.legend()

    plt.subplot(2,1,2)
    plt.plot(freqs,yECG, label='ECG', color='tab:red')
    plt.xlabel("Freq (Hz)")
    plt.ylabel("Magnitude")
    plt.legend()

    def bandpass_filter(freqs, y, wl, wh):
        yfiltered = y.copy()
        fnew = freqs.copy()
        try:
            il = np.where(freqs <= wl)[0][-1]
            ih = np.where(freqs >= wh)[0][0]
            yfiltered[0:il+1] = 0
            yfiltered[ih:] = 0
            ynew = y[il:ih].copy()
            fnew = freqs[il:ih].copy()
            return (fnew, ynew, yfiltered)
        except:
            print("No data to filter")
            return (freqs, ynew, ynew)
    wl, wh = (0.5,8)

    fnew, yPPGnew, yPPGfilter = bandpass_filter(freqs, yPPG, wl, wh)
    fnew, yECGnew, yECGfilter = bandpass_filter(freqs, yECG, wl, wh)
    plt.figure()
    plt.subplot(2,1,1)
    plt.plot(freqs, yPPGfilter, label="PPG", color='tab:blue')
    plt.title(f"Filtered signals thru bandpass({wl},{wh})")
    plt.xlabel("Freq (Hz)")
    plt.ylabel("Magnitude")
    plt.legend()

    plt.subplot(2,1,2)
    plt.plot(freqs,yECGfilter, label='ECG', color='tab:red')
    plt.xlabel("Freq (Hz)")
    plt.ylabel("Magnitude")
    plt.legend()

    plt.figure()
    plt.subplot(2,1,1)
    plt.plot(fnew, yPPGnew, label="PPG", color='tab:blue')
    plt.title(f"Focused signals")
    plt.xlabel("Freq (Hz)")
    plt.ylabel("Magnitude")
    plt.legend()

    plt.subplot(2,1,2)
    plt.plot(fnew,yECGnew, label='ECG', color='tab:red')
    plt.xlabel("Freq (Hz)")
    plt.ylabel("Magnitude")
    plt.legend()

    plt.show(block=False)

    print()

def our_ecg_ppg_test(path):
    df_ref = pd.read_csv(os.path.abspath(r"{}/data/our_ECG_PPG_Test/ecg_ppg_2min_rest_ecg.csv".format(path)),
                     names=['Time','PPG1','PPG2','PPG3','Time2','ECG'])
    
    df_test = pd.read_csv(os.path.abspath(r"{}\\Data\\our_ECG_PPG_Test\\PPG_ECG_test_1_rest_2min.csv".format(path)),
                     names=['IR','Red'])
    nmins = 1
    df_test['Time'] = np.linspace(0,nmins*60,len(df_test['IR']))
    
    df_test = df_test.truncate(before=df_test['IR'].idxmax()+50)
    df_ref = df_ref[df_ref['Time'] >= min(df_test['Time'])]
    df_ref = df_ref[df_ref['Time'] <= max(df_test['Time'])]
    df_ref['Time'] -= min(df_test['Time'])
    df_test['Time'] -= min(df_test['Time'])

    plt.subplot(3,1,1)
    peaks = find_peaks(df_ref['PPG1'],prominence=90,distance=20)
    plt.plot(df_ref['Time'],df_ref['PPG1'], color="tab:blue",label=f"Client's reference PPG\nNum peaks: {len(peaks[0])}\nAvg peaks/min: {round(len(peaks[0])/nmins,1)}")
    plt.title(f"Test 2 (1 min test)")
    plt.xlabel("Time (s)")
    plt.ylabel("Raw ADC counts")
    for i, peak in enumerate(peaks[0]):
        plt.plot(df_ref['Time'].values[peak],df_ref['PPG1'].values[peak],marker='x',color='k')
    plt.legend()

    plt.subplot(3,1,2)
    peaks = find_peaks(df_test['IR'],prominence=500,distance=20)
    plt.plot(df_test['Time'],df_test['IR'], color="tab:green",label=f"Our system's PPG\nNum peaks: {len(peaks[0])}\nAvg peaks/min: {round(len(peaks[0])/nmins,1)}")
    #plt.title(f"Our system's PPG output")
    plt.xlabel("Time (s)")
    plt.ylabel("Raw ADC counts")
    for i, peak in enumerate(peaks[0]):
        plt.plot(df_test['Time'].values[peak],df_test['IR'].values[peak],marker='x',color='k')
    plt.legend()

    plt.subplot(3,1,3)
    #plt.title(f"Client's reference ECG output")
    peaks = find_peaks(df_ref['ECG'],prominence=50,distance=15)
    plt.plot(df_ref['Time'],df_ref['ECG'],color="tab:red",label=f"Client's reference ECG\nNum peaks: {len(peaks[0])}\nAvg peaks/min: {round(len(peaks[0])/nmins,1)}")
    plt.xlabel("Time (s)")
    plt.ylabel("Raw ADC counts")
    for i, peak in enumerate(peaks[0]):
        plt.plot(df_ref['Time'].values[peak],df_ref['ECG'].values[peak],marker='x',color='k')
    plt.legend()

    #plt.show(block=True)

    tref = df_ref['Time'].to_numpy()
    ttest = df_test['Time'].to_numpy()
    ppgtest = df_test['IR'].to_numpy()
    ppgref = df_ref['PPG1'].to_numpy()
    ecgref = df_ref['ECG'].to_numpy()
    N_test = len(ttest)
    N_ref = len(tref)

    fsref = (len(tref)-1)/(tref[-1] - tref[0])
    fstest = (len(ttest)-1)/(ttest[-1] - ttest[0])
    freqsref = np.linspace(0, fsref/2, int(len(tref)/2))
    freqstest = np.linspace(0, fstest/2, int(len(ttest)/2))

    fftPPGref = fft(ppgref-np.average(ppgref))
    fftPPGtest = fft(ppgtest-np.average(ppgtest))
    fftECGref = fft(ecgref - np.average(ecgref))

    yPPGref = 2/N_ref * np.abs(fftPPGref)[0:int(N_ref/2)]
    yECGref = 2/N_ref * np.abs(fftECGref)[0:int(N_ref/2)]
    yPPGtest = 2/N_test * np.abs(fftPPGtest)[0:int(N_test/2)]

    plt.figure()
    ax1 = plt.subplot(2,1,1)
    plt.plot(freqsref,yPPGref,label=f"Client's reference PPG", color="tab:blue")
    plt.title(f"FFT signal analysis before filtering (fs={round(fsref,1)})")
    #plt.xlabel("Freq (Hz)")
    plt.ylabel("Magnitude")
    plt.legend()

    plt.subplot(2,1,2,sharex=ax1)
    plt.plot(freqsref,yECGref,label=f"Client's reference ECG", color="tab:red")
    #plt.xlabel("Freq (Hz)")
    plt.ylabel("Magnitude")
    plt.xlabel("Freq (Hz)")
    plt.legend()

    #plt.subplot(3,1,3,sharex=ax1)
    plt.figure()
    plt.title(f"FFT signal analysis before filtering (fs={round(fstest,1)})")
    plt.plot(freqstest,yPPGtest,label=f"Our system's PPG", color="tab:green")
    plt.xlabel("Freq (Hz)")
    plt.ylabel("Magnitude")
    plt.legend()

    wl, wh = (0.5,8)

    def SNR(freqs, y, wl, wh):
        Pnoise, Psignal = 0,0
        Nsignal, Nnoise = 0,0
        for i,f in enumerate(freqs):
            if wl <= f <= wh:
                Psignal += np.abs(y[i])**2
                Nsignal += 1
            elif f > wh:
                Pnoise += np.abs(y[i])**2
                Nnoise += 1
        Psignal /= Nsignal
        Pnoise /= Nnoise

        return (10*np.log10(Psignal), 10*np.log10(Pnoise), 10*np.log10(Psignal/Pnoise))
    
    Psignal, Pnoise, snr = SNR(freqstest,yPPGtest,wl,wh)
    print(f"Psignal: {Psignal}dB\tPnoise: {Pnoise}dB\tSNR:{snr}dB")

    def bandpass_filter(freqs, y, wl, wh):
        yfiltered = y.copy()
        fnew = freqs.copy()
        try:
            il = np.where(freqs <= wl)[0][-1]
            ih = np.where(freqs >= wh)[0][0]
            yfiltered[0:il+1] = 0
            yfiltered[ih:] = 0
            ynew = y[il:ih].copy()
            fnew = freqs[il:ih].copy()
            return (fnew, ynew, yfiltered)
        except:
            print("No data to filter")
            return (freqs, ynew, ynew)
    
    f_ref_new, yPPG_ref_new, yPPG_filter_ref_new = bandpass_filter(freqsref, yPPGref, wl,wh)
    f_ref_new, yECG_ref_new, yECG_filter_ref_new = bandpass_filter(freqsref, yECGref, wl,wh)
    f_test_new, yPPG_test_new, yPPG_filter_test_new = bandpass_filter(freqstest, yPPGtest, wl,wh)

    plt.figure()
    f_test_new -= 0.14
    ind = np.argpartition(yPPG_test_new, -30*nmins)[-30*nmins:]
    plt.subplot(3,1,1)
    plt.plot(f_ref_new, yPPG_ref_new, label=f"client's reference PPG\n(fs = {round(fsref,1)}Hz)", color='tab:blue')
    plt.title(f"FFT signal analysis after filtering thru bandpass({wl}Hz, {wh}Hz)")
    plt.xlabel("Freq (Hz)")
    plt.ylabel("Magnitude")
    plt.legend()
    for i in ind:
        plt.axvline(x=f_test_new[i],alpha=0.2,color='g')

    plt.subplot(3,1,2)
    plt.plot(f_ref_new,yECG_ref_new, label=f"client's reference ECG\n(fs = {round(fsref,1)}Hz)", color='tab:red')
    plt.xlabel("Freq (Hz)")
    plt.ylabel("Magnitude")
    plt.legend()
    for i in ind:
        plt.axvline(x=f_test_new[i],alpha=0.2,color='g')

    plt.subplot(3,1,3)
    plt.plot(f_test_new,yPPG_test_new, label=f"Our system's PPG\n(fs = {round(fstest,1)}Hz)", color='tab:green')
    plt.xlabel("Freq (Hz)")
    plt.ylabel("Magnitude")
    plt.legend()
    for i in ind:
        plt.axvline(x=f_test_new[i],alpha=0.2,color='g')

    plt.show(block=True)

def imu_test(path):
    timelimit = 10
    path = os.path.abspath(r"{}/Data/IMU/imu_static_{}s.csv".format(path,timelimit))
    df = pd.read_csv(path)
    t = np.linspace(0,timelimit,len(df['AccX']))
    plt.figure()
    plt.subplot(2,1,1)
    plt.title("Acceleration and Gyroscope output")
    plt.plot(t,df['AccX'].values,color='tab:red',label='Acceleration X')
    plt.plot(t,df['AccY'].values,color='tab:green',label='Acceleration Y')
    plt.plot(t,df['AccZ'].values,color='tab:blue',label='Acceleration Z')
    plt.legend()
    plt.ylabel("m/s^2")

    plt.subplot(2,1,2)
    plt.plot(t,df['GyroX'].values,color='tab:red',label='Gyro X')
    plt.plot(t,df['GyroY'].values,color='tab:green',label='Gyro Y')
    plt.plot(t,df['GyroZ'].values,color='tab:blue',label='Gyro Z')
    plt.legend()
    plt.ylabel("deg/s")
    plt.xlabel("Time (s)")

    
    N = len(t)
    fs = (N-1)/(t[-1] - t[0])
    freqs = np.linspace(0, fs/2, int(N/2))

    fftDict = {}
    yDict = {}

    def SNR(freqs, y, wl, wh):
        Pnoise, Psignal = 0,0
        Nsignal, Nnoise = 0,0
        for i,f in enumerate(freqs):
            if wl <= f <= wh:
                Psignal += np.abs(y[i])**2
                Nsignal += 1
            elif f > wh:
                Pnoise += np.abs(y[i])**2
                Nnoise += 1
        Psignal /= Nsignal
        Pnoise /= Nnoise

        return (10*np.log10(Psignal), 10*np.log10(Pnoise), 10*np.log10(Psignal/Pnoise))

    for col in df.columns:
        npArr = df[col].to_numpy()
        fftDict[col] = fft(npArr-np.average(npArr))
        yDict[col] = 2/N * np.abs(fftDict[col])[0:int(N/2)]
        Psignal, Pnoise, snr = SNR(freqs,yDict[col],0,2)
        print(f"{col} snr:{round(snr,3)}")
    accNames = ['Acceleration X', 'Acceleration Y', 'Acceleration Z']
    gyroNames = ['Gyro X', 'Gyro Y', 'Gyro Z']
    color = ['tab:red','tab:green','tab:blue']

    plt.figure()
    plt.subplot(3,1,1)
    plt.title(f"Acceleration frequency analysis (fs={round(fs,1)}Hz)")
    for i, c in enumerate(['AccX', 'AccY', 'AccZ']):
        plt.subplot(3,1,i+1)
        plt.plot(freqs,yDict[c],color=color[i],label=accNames[i])
        plt.ylabel("Magnitude")
        plt.legend()
    plt.xlabel("Freq (Hz)")

    plt.figure()
    plt.subplot(3,1,1)
    plt.title(f"Gyroscope frequency analysis (fs={round(fs,1)}Hz)")
    for i, c in enumerate(['GyroX', 'GyroY', 'GyroZ']):
        plt.subplot(3,1,i+1)
        plt.plot(freqs,yDict[c],color=color[i],label=gyroNames[i])
        plt.ylabel("Magnitude")
        plt.legend()
    plt.xlabel("Freq (Hz)")
    plt.show(block=True)


path = os.path.dirname(sys.argv[0])
imu_test(path)