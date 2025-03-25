import serial
import time


#Config port série
ser = serial.Serial('/dev/ttyAMA0',baudrate=115200,timeout=1)
#sent buffer format
sent_buffer = "$TEMP:23.5C;GPS:48.8566N,2.3522E;ACC:14.5;ALT:120m;PRES:1013hPa;DATE:2025-01-18;TIME:14:30:45#"


def data_process(data):
    data = data.replace('\r','').replace('$','').replace('#','').replace('\n','')
    data = data.split(',')
    return data

while True:
    try:
        if ser.in_waiting > 0 :
            data = ser.readline().decode('utf')
            #print(ser.in_waiting)
            #print(data)
            print(data_process(data))
            time.sleep(0.1)
        else :
            print("serial port deconnected")      
    except KeyboardInterrupt:
        print("except")
