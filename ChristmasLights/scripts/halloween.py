import time
import random

import neopixel as np
import display

def RandomWalk(width, height, index):
  x = random.randrange(width)
  y = random.randrange(height)
  while True:
    x += random.randint(-1, 1)
    y += random.randint(-1, 1)
    if x == -1:
      x = 1
    elif x == width:
      x = width -1
    if y == -1:
      y = 1
    elif y == height:
      y = height -1
    yield (x, y, index)

def Spiders(d):
  walkers = []
  for _ in xrange(7):
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
    for _ in xrange(18):
      d.FillColorRGB(low, 0, 0)
      yield

def Strobe(d):
  duration = 12
  for i in xrange(duration):
    if i % 2 == 1:
      d.FillColorRGB(255, 255, 255)
    else:
      d.FillColorRGB(0, 0, 0)
    yield

def Bugs(d):
  duration = 100
  walkers = []
  for _ in xrange(7):
    walkers.append(RandomWalk(d.width, d.height))

  for _ in xrange(duration):
    for w in walkers:
      (x, y) = w.next()
      d.SetPixelColorRGB(x, y, 0, 0, 40)
    yield

def Pulse(d):
  low = 5
  high = 30
  duration = 50
  while True:
    for i in xrange(duration):
      v = low + ((high - low) * i) / duration
      d.FillColorRGB(v, 0, 0)
      yield
    for i in xrange(duration):
      v = low + ((high - low) * (duration - i)) / duration
      d.FillColorRGB(v, 0, 0)
      yield

def PulseWithEffects(d):
  background = Heartbeat(d)
  interval = 5.0

  nextEffectTime = time.clock() + interval + random.random() * interval
  effect = None
  for _ in background:
    if not effect and time.clock() > nextEffectTime:
      nextEffectTime = time.clock() + interval + random.random() * interval
      if random.random() < 0.35:
        for _ in Strobe(d):
          d.Show()
          yield
      else:
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

def main():
  strip = display.CreateDefaultStrip()
  strip.begin()

  d = display.Display(strip, 5)
  spiders = Spiders(d)
  pulse = PulseWithEffects(d)
  while True:
    if time.localtime().tm_hour < 18:
      time.sleep(60)
      continue
    try:
      for frame in pulse:
        if time.localtime().tm_hour < 18:
          break
        time.sleep(0.02)
    except KeyboardInterrupt:
      break
    finally:
      d.FillColorRGB(0, 0, 0)
      d.Show()

if __name__ == '__main__':
  main()