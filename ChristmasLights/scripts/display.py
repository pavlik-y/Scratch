class DummyStrip:
  def begin(self):
    pass

def CreateDefaultStrip():
  return DummyStrip()

class Display:
  def __init__(self, strip, height):
    self.height = height
    self.width = 100

  def SetPixelColor(self, x, y, v):
    pass

  def SetPixelColorRGB(self, x, y, r, g, b):
    pass

  def Show(self):
    pass

  def FillColor(self, color):
    pass

  def FillColorRGB(self, r, g, b):
    pass
