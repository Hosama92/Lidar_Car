import serial
import threading
import time
import sys
import math
import numpy as np
import cv2


port = "COM11"
baud = 115200
ser = serial.Serial(port, baud, bytesize=serial.EIGHTBITS,
                                           parity=serial.PARITY_NONE,
                                           stopbits=serial.STOPBITS_ONE,
                                           timeout=1)
def main():
    thread = threading.Thread(target=readthread,args=(ser,))
    thread.start()

def readthread(ser):
    i = 0 
    j = 0
    tmp=list(range(10))
    while True:
        if ser.readable():
            tmp[j] = ser.read().hex()
            j+=1
            
            if(j == 9):
                 print(tmp[0:3])
                 print("end")
            #     res = tmp[i]
            #     #print(len(tmp))
            #     res += tmp[i+1] << 8
                
            #     print("%02X",res,end=' ')
            #     i+=2
            #     if i == 10:
            #         print('\n\r')
            #         i=0
                 j=0
            
                    



#ser.close()

main()