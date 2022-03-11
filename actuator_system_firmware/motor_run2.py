import math
import serial
import time
import csv
i = 0;

serialControl = serial.Serial(port='/dev/cu.usbmodem104945401', timeout=None, baudrate=20000000, xonxoff=False, rtscts=False, dsrdtr=False) 


while True:
    x = round(100*math.sin(i*0.3),5) # in radians
    y = round(100*math.sin(i*0.3),5) # in radians
    command  = '<X' + str(x) +'Y'+str(y) + '>' + '\n'
    serialControl.write(command.encode())     # write a string #<X25Y40>
    print([time.time(),str(command)])
    i += 1; 
    time.sleep(0.1)

    with open("motor_input.csv","a") as f:
        writer = csv.writer(f,delimiter=",")
        writer.writerow([time.time(),str(command)])
    
serialControl.close()
