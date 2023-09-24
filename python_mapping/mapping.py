import serial
import sys
import math
import numpy as np
import cv2

class YdlidarX2:
    def __init__(self, port):
        self.port = port
        self.ydlidarX2 = None  # ydlidarX2 속성을 초기화합니다.

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

            self.ydlidarX2.close()

        except Exception as e:
            if self.ydlidarX2:
                self.ydlidarX2.close()
            print(e)
            sys.exit()

    def GetSerial(self):
        while True:
            self.ydlidarX2.open()
            raw_data = bytearray(self.ydlidarX2.read(90))
            self.ydlidarX2.close()

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
        # 이전 Calculate 메서드 내용을 그대로 유지
        pass  # 이렇게 임시로 pass 문을 추가해도 됩니다.

# 나머지 코드는 동일합니다.

ydlidarX2 = YdlidarX2('COM13')
ydlidarX2.Connect()

img = np.zeros((512, 512, 3), np.uint8)

while True:
    ydlidarX2.GetSerial()
    angle, distance = ydlidarX2.Calculate()

    for idx in range(len(distance)):
        x = (distance[idx]) / 15 * math.cos(math.radians(angle[idx]))
        y = (distance[idx]) / 15 * math.sin(math.radians(angle[idx]))
        cv2.circle(img, (256 + int(x), 256 + int(y)), 1, (255, 0, 0), -1)
        cv2.imshow('', img)

        k = cv2.waitKey(1) & 0xFF
        if k == 27:
            break

    if k == 27:
        break
    

cv2.destroyAllWindows()
