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
f.write("StAccBias:-84,-65\n");
print("StAccBias: %s" % (ReadResponse(f)))
while True:
  i += 1
  f.write("RdAcc\n");
  print("%d: %s" % (i, ReadResponse(f)))
