import math
import time
import numpy as np
import csv
import serial
import os

##serialControl = serial.Serial(port='/dev/cu.usbmodem104945401', timeout=None, baudrate=20000000, xonxoff=False, rtscts=False, dsrdtr=False) 

tMat = np.empty((0,3), float)

xMat =np.empty((0,3), float)

yMat =np.empty((0,3), float)

vMat =np.empty((0,3), float)


a = 1500 #100*12172.8; # steps = mm * 64/0.1
f = 1;
t = 0;
t1 = time.time()+0.0001;
t2 = 0;
y1 = 1.5;
y2 = 1;
delta_y = 1;
delta_t = 0;
res = 0.01;
w= f*2*3.14159265;
#v= round(math.cos(w*t),0)
v = 200;
startTime = time.time()
counter = 0;
while True:
    clockTime = time.time()
    t = clockTime - startTime;
    t2 = t1;
    t1 = clockTime;
    delta_t = t1-t2;
    v_test = 10/(abs(math.cos(w*t)))+250
    v = 100000*delta_t/(delta_y+1)
    x = round(a*math.sin(w*t),0) # in radians
    y =  round(a*math.sin(w*t),0)
    y2 = y1;
    y1 = y;
    delta_y = abs(y2 - y1);
    command  = '<X' + str(x) +'Y'+str(y) + 'V'+ str(v) +'>' + '\n'
   ## serialControl.write(command.encode())     # write a string #<X25Y40V400>

    tMat = np.append(tMat, t)
    xMat = np.append(xMat, x)
    yMat = np.append(yMat, y)
    vMat = np.append(vMat, v)
    #print(command)
    counter = counter +1;
    print([t,x,y,v])
    time.sleep(0.00100)
    if t >= 4.00001 :
        break
     
i = 0
while os.path.exists(f"foo_{i}.csv"):
    i += 1
np.savetxt(f"foo_{i}.csv", np.transpose([tMat, xMat, yMat, vMat]), delimiter=",")
##serialControl.close()

