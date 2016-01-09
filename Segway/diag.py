import time
import serial
import sys

def ReadResponse(f):
  for attempt in range(0,3):
    l = f.readline().strip()
    if l.startswith("RSP:"):
      return l[4:].strip()
    print(l)
  return False

f = serial.Serial("/dev/tty.HC-06-DevB")
cmd = sys.argv[1]
if cmd == "on":
  f.write("DiagCollect:1\n");
if cmd == "off":
  f.write("DiagCollect:0\n");
if cmd == "dump":
  f.write("DiagDump\n");
print("Rsp: %s" % (ReadResponse(f)))
