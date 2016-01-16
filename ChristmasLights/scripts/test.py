import time
import random

import neopixel as np
import display

def RandomWalk(width, height):
  x = random.randint(0, width)
  y = random.randint(0, height)
  while True:
    x += random.randint(-1, 1)
    y += random.randint(-1, 1)
    x %= width
    y %= height
    yield (x, y)


def Spiders(d):
  walkers = []
  for _ in range(7):
    walkers.append(RandomWalk(d.width, d.height))
  while True:
    d.FillColorRGB(50, 20, 0)
    for w in walkers:
      (x, y) = w.next()
      d.SetPixelColorRGB(x, y, 0, 0, 40)
    d.Show()
    yield
    yield
    yield
    yield
    yield

def Heartbeat(d):
  low = 20
  values = [20, 25, 30, 40, 30, 25, 20]
  while True:
    for v in values:
      d.FillColorRGB(v, 0, 0)
      yield
    for v in values:
      d.FillColorRGB(v, 0, 0)
      yield
    for _ in range(18):
      d.FillColorRGB(low, 0, 0)
      yield

def Strobe(d):
  duration = 12
  for i in range(duration):
    if i % 2 == 1:
      d.FillColorRGB(255, 255, 255)
    else:
      d.FillColorRGB(0, 0, 0)
    yield

def Bugs(d):
  duration = 100
  walkers = []
  for _ in range(7):
    walkers.append(RandomWalk(d.width, d.height))

  for _ in range(duration):
    for w in walkers:
      (x, y) = w.next()
      d.SetPixelColorRGB(x, y, 0, 0, 40)
    yield

def Pulse(d):
  low = 5
  high = 30
  duration = 50
  while True:
    for i in range(duration):
      v = low + ((high - low) * i) / duration
      d.FillColorRGB(v, 0, 0)
      yield
    for i in range(duration):
      v = low + ((high - low) * (duration - i)) / duration
      d.FillColorRGB(v, 0, 0)
      yield

def PulseWithEffects(d):
  background = Pulse(d)
  interval = 5.0

  nextEffectTime = time.clock() + interval + random.random() * interval
  effect = None
  for _ in background:
    if not effect and time.clock() > nextEffectTime:
      nextEffectTime = time.clock() + interval + random.random() * interval
      effect = Bugs(d)
    if effect:
      try:
        effect.next()
      except StopIteration:
        effect = None
      except:
        raise
    d.Show()
    yield

def TestStrand(d):
  v = 10
  while True:
    for _ in range(10):
      d.FillColorRGB(v, 0, 0)
      d.Show()
      yield
    for _ in range(10):
      d.FillColorRGB(0, v, 0)
      d.Show()
      yield
    for _ in range(10):
      d.FillColorRGB(0, 0, v)
      d.Show()
      yield

def main():
  strip = display.CreateDefaultStrip()
  strip.begin()

  d = display.Display(strip, 5)
  spiders = Spiders(d)
  pulse = PulseWithEffects(d)
  test = TestStrand(d)
  while True:
    # if time.localtime().tm_hour < 18:
    #   time.sleep(60)
    #   continue
    try:
      for frame in test:
        # if time.localtime().tm_hour < 18:
        #   break
        time.sleep(0.02)
    except KeyboardInterrupt:
      break
    finally:
      d.FillColorRGB(0, 0, 0)
      d.Show()

if __name__ == '__main__':
  main()