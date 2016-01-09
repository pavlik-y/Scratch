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

# f.write("Echo:hello\n");
# print("Rsp: %s" % (ReadResponse(f)))

# f.write("WrConfig:SF.Lambda,0.003\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("WrConfig:Accel.BiasX,-88\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("WrConfig:Accel.BiasZ,915\n");
# print("Rsp: %s" % (ReadResponse(f)))

# f.write("WrConfig:Gyro.Bias,-103\n");
# print("Rsp: %s" % (ReadResponse(f)))

# f.write("WrConfig:Motor.LeftZero,0.42\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("WrConfig:Motor.LeftOne,0.88\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("WrConfig:Motor.RightZero,0.4\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("WrConfig:Motor.RightOne,1\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("WrConfig:Motor.Enabled,1\n");
# print("Rsp: %s" % (ReadResponse(f)))

# f.write("WrConfig:Bal.UprightAngle,1.25\n");
# print("Rsp: %s" % (ReadResponse(f)))

# f.write("WrConfig:Bal.Angle.KP,0.2\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("WrConfig:Bal.Angle.KI,0\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("WrConfig:Bal.Angle.KD,0.008\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("WrConfig:Bal.Angle.KL,0\n");
# print("Rsp: %s" % (ReadResponse(f)))


# f.write("WrConfig:SF.Lambda,0.001\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("DiagRecord:1\n");
# print("Rsp: %s" % (ReadResponse(f)))
# raw_input("Press enter")
# f.write("DiagRecord:0\n");
# print("Rsp: %s" % (ReadResponse(f)))


# f.write("WrConfig:VelCtrl.KP,-0.1\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("WrConfig:VelCtrl.KI,-0.2\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("WrConfig:VelCtrl.KD,0.0\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("WrConfig:VelCtrl.KL,8\n");
# print("Rsp: %s" % (ReadResponse(f)))

# f.write("WrConfig:ConfigHash,25544\n");
# print("Rsp: %s" % (ReadResponse(f)))

# f.write("WrConfig:Pos.Lambda,0.005\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("WrConfig:Pos.SampleInterval,0.1\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("WrConfig:Motor.TurnOffset,0.2\n");
# print("Rsp: %s" % (ReadResponse(f)))

# f.write("WrConfig:Motor.LeftZero,0.42\n");
# print("Rsp: %s" % (ReadResponse(f)))
# f.write("WrConfig:Motor.RightZero,0.4\n");
# print("Rsp: %s" % (ReadResponse(f)))

f.write("WrConfig:Motor.LeftZero,0.45\n");
print("Rsp: %s" % (ReadResponse(f)))
f.write("WrConfig:Motor.RightZero,0.42\n");
print("Rsp: %s" % (ReadResponse(f)))

# f.write("Calibrate\n");
# print("Rsp: %s" % (ReadResponse(f)))

while True:
  f.write("RdFD\n");
  print("Rsp: %s" % (ReadResponse(f)))
  time.sleep(1)

