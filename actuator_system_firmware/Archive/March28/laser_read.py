import math
import time
import numpy as np
import csv
import serial
import re
import os



serialLaser = serial.Serial(port='/dev/cu.usbmodem104949701', timeout=None, baudrate=20000000, xonxoff=False, rtscts=False, dsrdtr=False) 
tMat = np.empty((0,3), float)
yMat =np.empty((0,3), float)


t = 0
counter = 0;
startTime = time.time()

while True:
    clockTime = time.time()

    t = clockTime - startTime;


    laserRaw = serialLaser.readline()
    laserData = laserRaw.decode('ascii').split(",")
    
    #print(str(time.time()) +","+ str(encoderData))
    tMat = np.append(tMat, clockTime)
    
    s = str(laserData)
    s = s.replace('[', '')
    s = s.replace(']', '')
    s = s.replace("\\r", '')
    s = s.replace("\\n", '')
    s = s.replace("'", '')
    sSplit = s.split(', ')
    
    yMat = np.append(yMat, float(s))
    
    #print(time.time(), float(sSplit[0]),float(sSplit[1]),float(sSplit[2]),float(sSplit[3]))

    counter = counter +1;
    time.sleep(0.0000001)
    if t >= 10 :
        break

    
i = 0
while os.path.exists(f"laser_{i}.csv"):
    i += 1    
np.savetxt(f"laser_{i}.csv", np.transpose([tMat,yMat]), delimiter=",", fmt='"%s"')
serialLaser.close()
