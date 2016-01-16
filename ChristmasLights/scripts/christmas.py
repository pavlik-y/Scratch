import time
import random

import neopixel as np
import display

def Flake(d, duration):
  x = random.randrange(d.width)
  y = random.randrange(d.height)
  for i in xrange(duration / 2):
    v = i * 255 / (duration / 2)
    d.SetPixelColorRGB(x, y, v, v, v)
    yield
  for _ in xrange(2):
    d.SetPixelColorRGB(x, y, 255, 255, 255)
    yield
  for i in xrange(duration / 2):
    v = 255 - (i * 255 / (duration / 2))
    d.SetPixelColorRGB(x, y, v, v, v)
    yield

def Snowflakes(d):
  flakes = []
  timeToNextFlake = 0
  while True:
    d.FillColorRGB(5, 5, 25)
    i = 0
    while i < len(flakes):
      try:
        flakes[i].next()
        i += 1
      except StopIteration:
        del flakes[i]
      except:
        raise
    if timeToNextFlake > 0:
      timeToNextFlake -= 1
    else:
      timeToNextFlake = 2
      flakes.append(Flake(d, 40))
    d.Show()
    yield

def RandomColors(d):
  colors = [(80, 0, 0), (0, 80, 0), (50, 50, 30)]

  while True:
    for _ in xrange(3):
      x = random.randrange(d.width)
      y = random.randrange(d.height)
      c = random.choice(colors)
      d.SetPixelColorRGB(x, y, c[0], c[1], c[2])
    d.Show()
    yield

def Stripes(d):
  colors = [(80, 0, 0), (0, 80, 0), (50, 50, 30)]
  stripeWidth = 3
  while True:
    for i in xrange(len(colors) * stripeWidth):
      for x in xrange(d.width):
        for y in xrange(d.height):
          c = colors[((x + y + i) / stripeWidth) % len(colors)]
          d.SetPixelColorRGB(x, y, c[0], c[1], c[2])
      d.Show()
      for _ in xrange(20):
        yield

def Icicle(d, x):
  seq = [150,130,115,100,80, 65, 50, 40, 30,10]
  while True:
    for offset in xrange(-len(seq), len(seq)):
      for y in xrange(d.height):
        i = offset + y
        if 0 <= i < len(seq):
          v = seq[i]
          d.SetPixelColorRGB(x, y, v, v, v)
      yield
    for _ in xrange(random.randrange(100)):
      yield

def Icicles(d):
  icicles = []
  for x in xrange(d.width):
    icicles.append(Icicle(d, x))
  while True:
    d.FillColorRGB(0, 0, 25)
    for i in icicles:
      i.next()
    d.Show()
    yield

def main():
  auto_off = True
  strip = display.CreateDefaultStrip()
  strip.begin()

  d = display.Display(strip, 5)
  effects = [RandomColors, Stripes]
#  effects = [Icicles]
  while True:
    if time.localtime().tm_hour < 16 and auto_off:
      time.sleep(60)
      continue
    effect = random.choice(effects)(d)
    try:
      effectStartTime = time.time()
      for frame in effect:
        if time.localtime().tm_hour < 16 and auto_off:
          break
        if time.time() - effectStartTime >= 600:
          break
        time.sleep(0.02)
    except KeyboardInterrupt:
      break
    except:
      raise
    finally:
      d.FillColorRGB(0, 0, 0)
      d.Show()

if __name__ == '__main__':
  main()
