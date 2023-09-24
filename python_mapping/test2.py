import serial
import threading
import time
import sys
import math
import numpy as np
import cv2


port = "COM13"
baud = 115200
ser = serial.Serial(port, baud, bytesize=serial.EIGHTBITS,
                                           parity=serial.PARITY_NONE,
                                           stopbits=serial.STOPBITS_ONE,
                                           timeout=None)
def main():
    thread = threading.Thread(target=readthread,args=(ser,))
    thread.start()

def readthread(ser):
    while True:
        if ser.readable():
            raw_data = ser.read()
            
        i = 0 
        data = []
        while i<80:
            temp = raw_data[i]
            temp += raw_data[i+1]<<8
            data.append(temp)
            i += 2
        #     #raw_data = res.hex()
        #     # if res == scan_code:
        #     #ph_temp = ph_temp_b.hex()
        #     # ctls_temp = raw_data[2]
        #     # ctls_temp += raw_data[3] << 8
        print(data[1:80])
        #print(data.hex())
#ser.close()

main()