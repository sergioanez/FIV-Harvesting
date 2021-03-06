import math
import serial
import time
import csv

serialControl = serial.Serial(port='/dev/cu.usbmodem104945401', timeout=None, baudrate=20000000, xonxoff=False, rtscts=False, dsrdtr=False) 
a = 12160;
f = 4;
t = 0;
res = 0.01;

w= f/(2*3.14159);

startTime = time.time()

while True:
    clockTime = time.time()
    t = clockTime - startTime;

    x = round(a*math.sin(w*t),0) # in radians
    y =  round(a*math.sin(0.5*w*t),0)
    command  = '<X' + str(x) +'Y'+str(y) + '>' + '\n'
    serialControl.write(command.encode())     # write a string #<X25Y40>
    print([time.time(),str(command)])


    #time.sleep(1/1000)

    with open("motor_input.csv","a") as f:
        writer = csv.writer(f,delimiter=",")
        writer.writerow([time.time(),str(command)])
    
serialControl.close()
