import serial
import sys
import math
import numpy as np
import cv2
import time
 
class YdlidarX2:
    
    def __init__(self, port):
        self.port = port
        self.data = []
    def Connect(self):
        self.ydlidarX2 = serial.Serial(self.port, 
                            115200, 
                            bytesize=serial.EIGHTBITS, 
                            parity=serial.PARITY_NONE, 
                            stopbits=serial.STOPBITS_ONE, 
                            timeout=1)

        self.scan_code = bytes([0xA5, 0x5A, 0x05, 0x00, 0x40, 0x81])
        self.ydlidarX2.write(self.scan_code)
        

        



        
    def GetSerial(self):
 
        while True:
            #self.ydlidarX2.open()
            raw_data = self.ydlidarX2.read()
            #self.ydlidarX2.close()
 
            ph_temp = raw_data[0]
            ph_temp += raw_data[1] << 8
            ctls_temp = raw_data[2]
            ctls_temp += raw_data[3] << 8
 
 
            if ph_temp == 0x55AA and ctls_temp == 0x2800:
                i = 0
                self.data = []
                while i < 90:
                    temp = raw_data[i]
                    temp += raw_data[i+1] << 8
                    self.data.append(temp)
                    i += 2
                break
            else:
                continue
 
    
    def Calculate(self):
        check_sum = self.data[0] ^ self.data[2]
        check_sum ^= self.data[1]
        check_sum ^= self.data[3]
 
        an_fsa = (self.data[2] >> 1) // 64
        an_lsa = (self.data[3] >> 1) // 64
        
        if an_fsa > an_lsa:
            ang_diff = abs(an_fsa - (360 + an_lsa))
        else:
            ang_diff = abs(an_fsa -an_lsa)
            
            
        
        lsn = self.data[1] >> 8
        
        sampled_data = self.data[5:]
        angle = [an_fsa]
 
        i = 0
 
        for i in range(1, lsn - 1):
            angle.append((ang_diff / (lsn - 1) * i) + an_fsa)
            
        angle.append(an_lsa)
        
        
        distance = []
 
        for dist in range(lsn):
            distance.append(sampled_data[dist] // 4)
            
        ang_correct = []
        
 
        
        for cor in range(lsn):
            if distance[cor] == 0:
                ang_correct = 0
                angle[cor] = 0
            else:
                ang_correct = math.degrees(math.atan(21.8 * (155.3 - distance[cor]) / (155.3 * distance[cor])))
                angle[cor] = abs(angle[cor] - ang_correct)
            
            if angle[cor] >= 360:
                angle[cor] = angle[cor] - 360
            else:
                angle[cor] = angle[cor]
        return angle, distance
 
 
 
ydlidarX2 = YdlidarX2('COM13')
ydlidarX2.Connect()

 

img = np.zeros((512,512,3), np.uint8)
# cv2.imshow('',img)

# k = cv2.waitKey(0) 
    

# if k == 27:

#     cv2.destroyAllWindows()


        
 
        
 
 
 

while True:
 
    ydlidarX2.GetSerial()
 
    angle, distance = ydlidarX2.Calculate()
 
    
 
    
 
 
 
    for idx in range(len(distance)):
 
        if angle[idx] < 90 and angle[idx] > 0:
 
            x = 256
 
            y = int(256 * math.tan(angle[idx]))
 
        elif angle[idx] < 180 and angle[idx] > 90:
 
            x = 256
 
            y = -int(256 * math.tan(angle[idx]))
 
        elif angle[idx] < 270 and angle[idx] > 180:
 
            x = -256
 
            y = int(256 * math.tan(angle[idx]))
 
        elif angle[idx] < 360 and angle[idx] > 270:
 
            x = -256
 
            y = -int(256 * math.tan(angle[idx]))
 
        elif angle[idx] == 0:
 
            x = 0
 
            y = 256
 
        elif angle[idx] == 90:
 
            x = 256
 
            y = 0
 
        elif angle[idx] == 180:
 
            x = 0
 
            y = - 256
 
        elif angle[idx] == 270:
 
            x = -256
 
            y = 0
 
            
 
        
 
        cv2.line(img, (256, 256), (256 + x, 256 + y), (0,0,0), 1)
 
        #cv2.line(img, (256, 256), (512, 0), (0,0,255), 1)
 
 
 
 
 
        x = (distance[idx]) / 15 * math.cos(math.radians(angle[idx]))
 
        y = (distance[idx]) / 15 * math.sin(math.radians(angle[idx]))
 
        
 
        cv2.circle(img,(256 + int(x),256 + int(y)), 1, (255, 0, 0), -1)
 
            
 
    cv2.imshow('',img)
 
    k = cv2.waitKey(0) 
 
        
 
    if k == 27:
 
        break
 
    
 
        
 
        
 
 
 
cv2.destroyAllWindows()