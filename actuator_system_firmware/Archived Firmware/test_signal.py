import math
import serial
import time 
i = 0;

serialControl = serial.Serial('/dev/cu.usbmodem104945401')  # open serial port
serialReadEncoder = serial.Serial('/dev/tty.usbmodem103309801')  # open serial port
print(serialControl.name)         # check which port was really used

while True:
    x = round(100*math.sin(i*0.1),5) # in radians
    y = round(100*math.sin(i*0.1),5) # in radians
    command  = '<X' + str(x) +'Y'+str(y) + '>' + '\n'
    serialControl.write(command.encode())     # write a string #<X25Y40>
    print(command.encode())
    i += 1; 
    time.sleep(0.1)
    encoderData = serialReadEncoder.read() 
    
serialControl.close()
serialReadEncoder.close()
