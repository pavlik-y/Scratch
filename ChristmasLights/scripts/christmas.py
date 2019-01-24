import collections
import itertools
import random
import time

import neopixel as np
import display

Pixel = collections.namedtuple('Pixel', ['x', 'y'])

def iShuffledPixels(width, height):
  while True:
    all_pixels = list(itertools.product(range(width), range(height)))
    random.shuffle(all_pixels)
    for p in all_pixels:
      yield Pixel(p[0], p[1])

def FindEmptyListSlot(l):
  for i in xrange(len(l)):
    if l[i] is None:
      return i
  i = len(l)
  l.append(None)
  return i

def Fade(start, end, duration):
  for i in xrange(duration):
    yield start + ((end - start) * i) / duration

def FlakePattern(duration):
  for v in Fade(0, 255, duration / 2):
    yield v
  for _ in xrange(2):
    yield 255
  for v in Fade(255, 0, duration / 2):
    yield v

def Flake(d, x, y, duration):
  for v in FlakePattern(duration):
    d.SetPixelColorRGB(x, y, v, v, v)
    yield True

def Snowflakes(d):
  d.FillColorRGB(5, 5, 25)
  flakes = []
  tick = 0
  pixels = iShuffledPixels(d.width, d.height)
  while True:
    for index, flake in enumerate(flakes):
      if flake is None:
        continue
      if not next(flake[0], False):
        flakes[index] = None
        d.SetPixelColorRGB(flake[1], flake[2], 5, 5, 25)
    if tick % 3 == 0:
      index = FindEmptyListSlot(flakes)
      px = next(pixels)
      flakes[index] = (Flake(d, px.x, px.y, 40), px.x, px.y)
    d.Show()
    yield

def RandomColors(d):
  colors = [(80, 0, 0), (0, 80, 0), (50, 50, 30), (30, 0, 50)]

  pixels = iShuffledPixels(d.width, d.height)
  while True:
    for _ in xrange(3):
      px = next(pixels)
      c = random.choice(colors)
      d.SetPixelColorRGB(px.x, px.y, c[0], c[1], c[2])
    d.Show()
    yield

def Stripes(d):
  colors = [(80, 0, 0), (0, 80, 0), (50, 50, 30), (30, 0, 50)]
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

def ClearDisplay(d):
  d.FillColorRGB(0, 0, 0)
  d.Show()

def ShouldShowEffectNow(now, auto_off):
  if not auto_off:
    return True
  hr = now.tm_hour
  if hr >= 16:
    return True
  if hr >= 5 and hr <= 8:
    return True
  return False

def MainLoop(d):
  auto_off = False
#   effects = [RandomColors]
  effects = [RandomColors, Stripes, Snowflakes]
#   effects = [Icicles]
#   effects = [Snowflakes]

  effect_it = None
  iteration = 0

  while True:
    if iteration % 100 == 0:
      while not ShouldShowEffectNow(time.localtime(), auto_off):
        ClearDisplay(d)
        effect_it = None
        time.sleep(60)
    if effect_it == None:
      effect_it = random.choice(effects)(d)
      effectStartTime = time.time()
      continue
    if time.time() - effectStartTime >= 600:
      ClearDisplay(d)
      effect_it = None
      continue
    iteration += 1
    assert(effect_it)
    next(effect_it)
    time.sleep(0.02)


def main():
  strip = display.CreateDefaultStrip()
  strip.begin()

  d = display.Display(strip, 5)
  try:
    MainLoop(d)
  except KeyboardInterrupt:
    ClearDisplay(d)

if __name__ == '__main__':
  main()
