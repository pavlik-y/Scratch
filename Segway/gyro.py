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

f.write("GyroStartCalibration\n");
print("Gyro:%s" % (ReadResponse(f)))
time.sleep(5);
f.write("GyroStopCalibration\n");
print("Gyro:%s" % (ReadResponse(f)))

while True:
#for i in range (0, 64):
  f.write("RdGyro\n");
  print("%d: %s" % (i, ReadResponse(f)))
