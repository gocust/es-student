import serial
import time
from popagraph import *

n = 1
port = 'COM9' 
baudrate = 9600 
lines = [0]*n
count = 0
                
def writeNum(Serial,num):
    s = str(num)
    sfin = "0"*(3-len(s))+s
    for el in sfin:
        data = int(el)
        Serial.write([data])
    print(f"Отправлено: {num}")

def read(Serial):
    data = str(Serial.readline().rstrip())
    leng = len(list(data))
    data = data[2:leng-1:]
    return data

def binary_to_ascii(binary_string):
    return ''.join(chr(int(binary_string[i:i+8], 2)) for i in range(0, len(binary_string), 8))

try:
    ser = serial.Serial(port, baudrate, timeout=1)
    print(f"Подключено к {port} со скоростью {baudrate} бод")
except serial.SerialException as e:
    print(f"Ошибка при открытии порта: {e}")
    exit()

time.sleep(1)
fig, ax = grp.subplots(figsize=(6, 4))

with open("2.txt", "w") as f:
    f.close()

while True:
    if ser.in_waiting > 0:
        data2 = read(ser)
        print(f"Получено: {data2}")
        count += 1
        with open("2.txt", "a") as f:
            f.write(str(data2)+"/")
            f.close()
        draw_graph(ax,count)
        print('goida')
    time.sleep(1)

ser.close
