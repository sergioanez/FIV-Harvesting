import math
import serial
import time
import csv

serialControl = serial.Serial(port='/dev/cu.usbmodem104945401', timeout=None, baudrate=20000000, xonxoff=False, rtscts=False, dsrdtr=False) 
a = 400*16;
w = 1*2*3.14;
t = 0;
res = 0.001;
while True:
    x=0
   #x = round(a*math.sin(w*t),5) # in radians
    y = round(a*math.sin(w*t),5) # in radians
    command  = '<X' + str(x) +'Y'+str(y) + '>' + '\n'
    serialControl.write(command.encode())     # write a string #<X25Y40>
    print([time.time(),str(command)])


    t = t+res;
    time.sleep(1/w)

    with open("motor_input.csv","a") as f:
        writer = csv.writer(f,delimiter=",")
        writer.writerow([time.time(),str(command)])
    
serialControl.close()
