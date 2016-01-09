import serial

class ArdController:
  def __init__(self, port):
    self.ser = serial.Serial(port, 9600)
    self.ser.readline()
  def ReadControls(self):
    cmd = "rdst:\n"
    self.ser.write(cmd)
    resp = self.ser.readline()
    assert(resp[:5] == 'btst:')
    args = resp[5:].split(',')
    return (int(args[0]), int(args[1]))
  def SetLeds(self, value):
    cmd = "wled:%s\n" % (value)
    self.ser.write(cmd)
    self.ser.readline()

if __name__ == "__main__":
  ard = ArdController("COM3")
  print(ard.ReadControls())
  ard.SetLeds(21)
  