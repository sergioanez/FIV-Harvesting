import math
import serial
import time 
import csv
serialEncoder = serial.Serial(port='/dev/cu.usbmodem103309801', timeout=None, baudrate=20000000, xonxoff=False, rtscts=False, dsrdtr=False) 


while True:


    encoderDataRaw = serialEncoder.readline()
    encoderData = encoderDataRaw.decode('ascii').split(",")
    
    print(str(time.time()) +","+ str(encoderData))
    
    with open("encoder_output.csv","a") as f:
        writer = csv.writer(f,delimiter=",")
        writer.writerow([time.time(),str(encoderData)])
    
serialEncoder.close()
