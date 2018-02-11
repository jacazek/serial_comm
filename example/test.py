import sys
import serial
with serial.Serial(sys.argv[1], 9600) as ser:
     line = ser.readline()
     while(line):
             print(line)
             line = ser.readline()

