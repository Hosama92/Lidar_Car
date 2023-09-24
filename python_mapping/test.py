import serial
import sys
import math
 
class YdlidarX2:
    
    def __init__(self, port):
        self.port = port
        self.data = []
        self.ydlidarX2 = None 
        

    def Connect(self):
        try:
            self.ydlidarX2 = serial.Serial(self.port, 
                             115200, 
                             bytesize=serial.EIGHTBITS, 
                             parity=serial.PARITY_NONE, 
                             stopbits=serial.STOPBITS_ONE, 
                             timeout=None)
    
            self.scan_code = bytes([0xA5, 0x5A, 0x05, 0x00, 0x40, 0x81])
            self.ydlidarX2.write(self.scan_code)
 
            
    
 
        except Exception as e:
           # self.ydlidarX2.close()
            print(e)
            sys.exit()
            
    def Read(self, chunk):
        self.data = bytearray(self.ydlidarX2.read(chunk))
        return self.data
    
    def Open(self):
        self.ydlidarX2.open()
    def Close(self):
        self.ydlidarX2.close()
        
    def GetSerial(self):
 
        while True:
            
            raw_data = bytearray(self.ydlidarX2.read(90))
            
 
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
            an_fsa = 360 - an_fsa
        
        lsn = self.data[1] >> 8
        angle = []
 
        i = 0
 
        for i in range(5, len(self.data)):
            angle.append(((abs(an_fsa - an_lsa) / (lsn - 1)) * i) + an_fsa)
 
        distance = []
 
        for data_idx in range(5, len(self.data)):
            distance.append(self.data[data_idx]//4)
            
        ang_correct = []
 
 
        for cor in range(len(distance)):
            if distance[cor] == 0:
                ang_correct.append(0)
            else:
                ang_correct.append(math.degrees(math.atan(21.8 * (155.3 - distance[cor]) / (155.3 * distance[cor]))))
 
        an_fsa = angle[0] + ang_correct[0]
        an_lsa = angle[len(distance) - 1] + ang_correct[len(distance) - 1]
 
        corr_angle = []
        i = 0
 
        for i in range(lsn):
            corr_angle.append(((abs(an_fsa - an_lsa) / (lsn - 1)) * i) + an_fsa)
            
            
        return corr_angle, distance
 
 
 
ydlidarX2 = YdlidarX2('COM13')
ydlidarX2.Connect()
 
 
while True:
    ydlidarX2.GetSerial()
    angle, distance = ydlidarX2.Calculate()
 
    for idx in range(len(distance)):
        print('Angle : {0:.3f} degrees, Distance : {1} mm'.format(angle[idx], distance[idx]))
    print('cycle')