import math
import time
import numpy as np
import csv
import serial
serialControl = serial.Serial(port='/dev/cu.usbmodem104945401', timeout=None, baudrate=20000000, xonxoff=False, rtscts=False, dsrdtr=False) 

tMat = np.empty((0,3), float)

xMat =np.empty((0,3), float)

yMat =np.empty((0,3), float)


a = 32000 #100*12172.8; # steps = mm * 64/0.1
f = 4;
t = 0;
res = 0.01;
w= f*2*3.14159265;
#v= round(math.cos(w*t),0)
v = 0
startTime = time.time()
counter = 0;
while True:
    clockTime = time.time()
    t = clockTime - startTime;
    #v = round(100*w*math.cos(w*t) +10,0)
    x = round(a*math.sin(w*t),0) # in radians
    y =  round(a*math.sin(w*t),0)
    command  = '<X' + str(x) +'Y'+str(y) + 'V'+ str(v) +'>' + '\n'
    serialControl.write(command.encode())     # write a string #<X25Y40V400>

    tMat = np.append(tMat, t)
    xMat = np.append(xMat, x)
    yMat = np.append(yMat, x)

    counter = counter +1;
    print([t,x,y, v])
    time.sleep(0.00100)
    if counter >= 1000000 :
        break
     
    
np.savetxt("foo.csv", np.transpose([tMat, xMat,yMat]), delimiter=",")
serialControl.close()

