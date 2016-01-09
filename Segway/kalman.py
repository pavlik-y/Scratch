import serial
import time

def ReadResponse(f):
  for attempt in range(0,3):
    l = f.readline().strip()
    if l.startswith("RSP:"):
      return l[4:].strip()
    print(l)
  return False

i = 0;
f = serial.Serial("/dev/tty.HC-06-DevB")
#f.write("KalmanWrParams:0.0009,0.003,0.04\n");
#f.write("KalmanWrParams:0.01,0.0003,0.004\n");
#print("%d: %s" % (i, ReadResponse(f)))
while True:
#for i in range (0, 64):
  f.write("RdKalman\n");
  print("%d: %s" % (i, ReadResponse(f)))
