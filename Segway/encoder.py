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
while True:
  i += 1
  f.write("RdEncLeft\n");
  print("%d: %s" % (i, ReadResponse(f)))
