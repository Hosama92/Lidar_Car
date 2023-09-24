import serial
import threading
import time
import sys
import math
import numpy as np
import cv2
import struct


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
    tmp = []
    while True:
        if ser.readable():
            #tmp = struct.unpack('c',ser.read())
            tmp = ser.read(20)
            print(tmp)
main()