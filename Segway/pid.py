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
f.write("WrPid.angle:0.5, 0.0, 0.01, 0.00\n");
f.write("WrPid.position:0, 0.001, 0, 0\n");
#f.write("RdPid.angle\n");
print("%d: %s" % (i, ReadResponse(f)))
