import serial
from io import StringIO
import numpy as np
import matplotlib.pyplot as plt

ser = serial.Serial('/dev/cu.usbmodem1421', baudrate=9600, timeout=5)

data_received = False
while not data_received:
    line = ser.readline().decode('utf-8')
    if line.startswith("log: "):
        print("ARDUINO {}".format(line[5:].rstrip()))
    elif line.startswith("data: "):
        t = np.loadtxt(StringIO(line[6:]))
        t *= 1e-6

        s = np.empty(len(t))
        s[::2] = 1
        s[1::2] = 0

        plt.step(t, s)
        plt.ylim(-2, 2)
        plt.xlim(xmin=-0.5)
        plt.autoscale(False)
        plt.show()
        plt.close()

        data_received = True

ser.close()