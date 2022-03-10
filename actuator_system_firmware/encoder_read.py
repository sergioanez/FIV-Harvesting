import math
import serial
import time 
i = 0;

serialEncoder = serial.Serial('/dev/tty.usbmodem103309801')  # open serial port
while True:

    time.sleep(0.1)

    encoderDataRaw = serialEncoder.readline()
    encoderData = encoderDataRaw.decode().split(",")
    
    print(encoderData)
    
    
serialEncoder.close()
