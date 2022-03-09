import math
import serial
import time 
i = 0;

ser = serial.Serial('/dev/cu.usbmodem104945401')  # open serial port
print(ser.name)         # check which port was really used

while True:
    x = round(100*math.sin(i*0.1),5) # in radians
    y = round(100*math.sin(i*0.1),5) # in radians
    command  = '<X' + str(x) +'Y'+str(y) + '>' + '\n'
    ser.write(command.encode())     # write a string #<X25Y40>
    print(command.encode())
    i += 1; 
    time.sleep(0.1)
    
ser.close()
