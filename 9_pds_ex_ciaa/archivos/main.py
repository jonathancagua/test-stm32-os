import simpleaudio as sa
import numpy as np
import matplotlib.pyplot as plt

#--------------------------------
fs = 8000
muestra = np.load("data/chapu_noise.npy")
N = len(muestra)

#-------------------------
filtro_test = np.array(np.load('./data/numpy_fir.npy')[0]).astype(float)
filtered = np.convolve(filtro_test, muestra)
#se repdroducd el audio luego del filtrado
#playObj = sa.play_buffer(np.array(np.real(filtered), np.int16), 1, 2, fs * 1)
#playObj.wait_done()
#-----------------------
N_ = len(filtro_test)
nData_ = np.arange(0, N, 1)
fData_ = nData_ * (fs / N) - (fs / 2)

nData = np.arange(0, N, 1)
fData = nData * (fs / N) - (fs / 2)
#---------------------------------------------------------
fig = plt.figure()
muestra_ax = fig.add_subplot(3, 1, 1)
muestra_ax.set_xlim(0, N / fs)
muestra_ax.set_title("muestra", rotation = 0, fontsize = 10, va ="center")
plt.plot(nData / fs, muestra, 'g-o', linewidth = 2, alpha = 0.75)
plt.grid()
#---------------------------------------------------------
espectro = np.fft.fft(muestra)
espectro_ax = fig.add_subplot(3, 1, 2)
espectro_ax.set_xlim((-fs / 2) - (fs / N), (fs / 2) + (fs / N))
espectro_ax.set_title("FFT(muestra)", rotation = 0, fontsize = 10, va ="center")
plt.plot(fData, np.abs(np.fft.fftshift(espectro) / N ** 2), 'b-', linewidth = 2, alpha = 0.75)
plt.grid()
#-----------------------------
convAxe         = fig.add_subplot(3,1,3)
convolveNData = np.arange(0, len(filtered), 1)
convolveTData = convolveNData/fs
convLn,       = plt.plot(convolveTData, filtered, 'r-', label ="muestra-filtro", linewidth = 6, alpha = 0.5)
#convAxe.legend()
convAxe.grid(True)
convAxe.set_xlim(0,convolveTData[-1])

#se escucha antes del filtro
#playObj = sa.play_buffer(np.array(np.real(np.fft.ifft(espectro)), np.int16), 1, 2, fs * 1)
#playObj.wait_done()

plt.show()


