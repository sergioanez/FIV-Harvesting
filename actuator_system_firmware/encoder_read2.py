import math
import time
import numpy as np
import csv
import serial
serialEncoder = serial.Serial(port='/dev/cu.usbmodem103309801', timeout=None, baudrate=20000000, xonxoff=False, rtscts=False, dsrdtr=False) 
tMat = np.empty((0,3), float)
dataMat =np.empty((0,3), str)
t = 0
counter = 0;
startTime = time.time()

while True:
    clockTime = time.time()

    t = clockTime - startTime;


    encoderDataRaw = serialEncoder.readline()
    encoderData = encoderDataRaw.decode('ascii').split(",")
    
    print(str(time.time()) +","+ str(encoderData))
    tMat = np.append(tMat, t)
    dataMat = np.append(dataMat, str(encoderData))
    
    counter = counter +1;
    #time.sleep(1/100000000)
   # if counter >= 10000 :
     #   break
    #
np.savetxt("fin.csv", np.transpose([tMat, dataMat]), delimiter=",", fmt='"%s"')
serialEncoder.close()
