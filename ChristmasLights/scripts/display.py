import neopixel as np

LED_COUNT      = 540     # Number of LED pixels.
LED_PIN        = 18      # GPIO pin connected to the pixels (must support PWM!).
LED_FREQ_HZ    = 800000  # LED signal frequency in hertz (usually 800khz)
LED_DMA        = 5       # DMA channel to use for generating signal (try 5)
LED_BRIGHTNESS = 255     # Set to 0 for darkest and 255 for brightest
LED_INVERT     = False   # True to invert the signal (when using NPN transistor level shift)

def CreateDefaultStrip():
  return np.Adafruit_NeoPixel(LED_COUNT, LED_PIN, LED_FREQ_HZ, LED_DMA, LED_INVERT, LED_BRIGHTNESS)

class Display:
  def __init__(self, strip, height):
    self.strip = strip
    self.num_pixels = self.strip.numPixels()
    self.height = height
    self.width = self.strip.numPixels() / self.height

  def SetPixelColor(self, x, y, v):
    self.strip.setPixelColor(x * self.height + y, v)

  def SetPixelColorRGB(self, x, y, r, g, b):
    self.strip.setPixelColorRGB(x * self.height + y, g, r, b)

  def Show(self):
    self.strip.show()

  def FillColor(self, color):
    for i in xrange(self.num_pixels):
      self.strip.setPixelColor(i, color)

  def FillColorRGB(self, r, g, b):
    color = np.Color(g, r, b)
    self.FillColor(color)
