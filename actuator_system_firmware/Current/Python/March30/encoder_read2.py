import math
import time
import numpy as np
import csv
import serial
import re
import os



serialEncoder = serial.Serial(port='/dev/cu.usbmodem103309801', timeout=None, baudrate=20000000, xonxoff=False, rtscts=False, dsrdtr=False) 
tMat = np.empty((0,3), float)
aTMat =np.empty((0,3), float)
xAMat =np.empty((0,3), float)
xBMat =np.empty((0,3), float)
yMat =np.empty((0,3), float)
lasMat =np.empty((0,3), float)
t_offset = 0
dataMat =np.empty((0,3), str)
toggle = True;
t = 0
counter = 0;
startTime = time.time()

while True:
    clockTime = time.time()

    t = clockTime - startTime;


    encoderDataRaw = serialEncoder.readline()
    encoderData = encoderDataRaw.decode('ascii').split(",")
    
    #print(str(time.time()) +","+ str(encoderData))
    tMat = np.append(tMat, clockTime)
    s = str(encoderData)
    s = s.replace('[', '')
    s = s.replace(']', '')
    s = s.replace("'", '')
    s = s.replace("\\n", '')

    sSplit = s.split(', ')

        
    dataMat = np.append(dataMat,  str(encoderData))
    #print(time.time(), float(sSplit[0]),float(sSplit[1]),float(sSplit[2]),float(sSplit[3]))
    aTMat = np.append(aTMat, float(sSplit[0])-t_offset)
    xAMat = np.append(xAMat, float(sSplit[1]))
    xBMat = np.append(xBMat, float(sSplit[2]))
    yMat  = np.append(yMat, float(sSplit[3]))
    lasMat  = np.append(lasMat, float(sSplit[4]))


    counter = counter +1;
    time.sleep(0.0000001)
    if t >= 900 :
        break

    
i = 0
while os.path.exists(f"encoder_{i}.csv"):
    i += 1    
np.savetxt(f"encoder_{i}.csv", np.transpose([tMat, aTMat, xAMat, xBMat, yMat, lasMat]), delimiter=",", fmt='"%s"')
serialEncoder.close()
