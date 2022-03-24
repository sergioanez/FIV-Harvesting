import math
import time
import numpy as np
import csv
import serial
import os

serialControl = serial.Serial(port='/dev/cu.usbmodem104945401', timeout=None, baudrate=20000000, xonxoff=False, rtscts=False, dsrdtr=False) 

tMat = np.empty((0,3), float)

xMat =np.empty((0,3), float)

yMat =np.empty((0,3), float)

vMat =np.empty((0,3), float)


amm = 30 #100*12172.8; # steps = mm * 8/0.1
a = amm*8/0.1
f = 0.5;
t = 0;
t1 = time.time()+0.0001;
t2 = 0;
y1 = 1.5;
y2 = 1;
delta_y = 1;
delta_t = 0;
res = 0.01;
w = f*2*3.14159265;
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
    dx = abs(round(a*w*math.cos(w*t),0)) #+ round(a*0.5*w*0.5*math.sin(w*0.5*t),0))# + round(a*3*w*0.1*math.sin(w*3*t),0)); #delta_y/delta_t;
    
    if dx == 0:
        v=1000
    else:
        v = 1000000/dx  #v = 1000000/dx  920000/dx
        
    if v < 20 :
        v = 20

    if v > 500 :
        v = 500
        
    y = 12800 + round(a*math.sin(w*t),0)# + round(a*0.5*math.sin(w*0.5*t),0) + round(a*0.1*math.sin(w*3*t),0)
    x = 16000 #round(a*math.sin(w*t),0) + round(a*2*math.sin(w*0.5*t),0) + round(a*0.7*math.sin(w*3*t),0)
    y2 = y1;
    y1 = y;
    delta_y = abs(y2 - y1);
    command  = '<X' + str(x) +'Y'+str(y) + 'V'+ str(v) +'>' + '\n'
    serialControl.write(command.encode())     # write a string #<X25Y40V400>

    tMat = np.append(tMat, clockTime)
    xMat = np.append(xMat, x)
    yMat = np.append(yMat, y)
    vMat = np.append(vMat, v)
    #print(command)
    counter = counter +1;
    print([t,x,y,v])
    time.sleep(0.00100)
    if t >= 20 :
        break
     
i = 0
while os.path.exists(f"encoder_{i}.csv"):
    i += 1
i = i-1

v = 200;
command  = '<X' + str(x) +'Y'+str(y) + 'V'+ str(v) +'>' + '\n'
serialControl.write(command.encode())     # write a string #<X25Y40V400>
np.savetxt(f"input_test_{i}.csv", np.transpose([tMat, xMat, yMat, vMat]), delimiter=",")

serialControl.close()

