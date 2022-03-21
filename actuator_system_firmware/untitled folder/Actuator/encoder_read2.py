import math
import time
import numpy as np
import csv
import serial
import re



serialEncoder = serial.Serial(port='/dev/cu.usbmodem103309801', timeout=None, baudrate=20000000, xonxoff=False, rtscts=False, dsrdtr=False) 
tMat = np.empty((0,3), float)
aTMat =np.empty((0,3), float)
xAMat =np.empty((0,3), float)
xBMat =np.empty((0,3), float)
yMat =np.empty((0,3), float)

dataMat =np.empty((0,3), str)

t = 0
counter = 0;
startTime = time.time()

while True:
    clockTime = time.time()

    t = clockTime - startTime;


    encoderDataRaw = serialEncoder.readline()
    encoderData = encoderDataRaw.decode('ascii').split(",")
    
    #print(str(time.time()) +","+ str(encoderData))
    tMat = np.append(tMat, t)
    s = str(encoderData)
    s = s.replace('[', '')
    s = s.replace(']', '')
    s2 = s.split(', ')
    dataMat = np.append(dataMat,  str(encoderData))
    print(str(time.time()) +","+str(encoderData) )
    aTMat = float(s2[0])
    xAMat = float(s2[0])
    xBMat = float(s2[0])
    yMat = float(s2[0])

    counter = counter +1;
    time.sleep(0.00001)
    if t >= 15 :
        break

    
    
np.savetxt("fin.csv", np.transpose([tMat, dataMat]), delimiter=",", fmt='"%s"')
serialEncoder.close()
