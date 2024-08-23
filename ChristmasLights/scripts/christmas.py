import collections
import itertools
import random
import time

import display

Pixel = collections.namedtuple('Pixel', ['x', 'y'])

def iShuffledPixels(width, height):
  while True:
    all_pixels = list(itertools.product(range(width), range(height)))
    random.shuffle(all_pixels)
    for p in all_pixels:
      yield Pixel(p[0], p[1])

def FindEmptyListSlot(l):
  for i, v in enumerate(l):
    if v is None:
      return i
  l.append(None)
  return len(l) - 1

# Snowflakes and helpers

def Fade(start, end, duration):
  for i in range(duration):
    yield start + ((end - start) * i) // duration

def FlakePattern(duration):
  for v in Fade(0, 255, duration // 2):
    yield v
  for _ in range(2):
    yield 255
  for v in Fade(255, 0, duration // 2):
    yield v

def Flake(d, x, y, duration):
  for v in FlakePattern(duration):
    d.SetPixelColorRGB(x, y, v, v, v)
    yield True

def Snowflakes(d):
  d.FillColorRGB(5, 5, 25)
  flakes = []
  pixels = iShuffledPixels(d.width, d.height)
  while True:
    for index, flake in enumerate(flakes):
      if flake is None:
        continue
      if not next(flake[0], False):
        flakes[index] = None
        d.SetPixelColorRGB(flake[1], flake[2], 5, 5, 25)
    for _ in range(2):
      index = FindEmptyListSlot(flakes)
      px = next(pixels)
      flakes[index] = (Flake(d, px.x, px.y, 30), px.x, px.y)
    d.Show()
    yield

# ===

def RandomColors(d):
  colors = [(80, 0, 0), (0, 80, 0), (50, 50, 30), (30, 0, 50)]

  pixels = iShuffledPixels(d.width, d.height)
  while True:
    for _ in range(3):
      px = next(pixels)
      c = random.choice(colors)
      d.SetPixelColorRGB(px.x, px.y, c[0], c[1], c[2])
    d.Show()
    yield

# ===

def Stripes(d):
  colors = [(80, 0, 0), (0, 80, 0), (50, 50, 30), (30, 0, 50)]
  stripeWidth = 3
  while True:
    for i in range(len(colors) * stripeWidth):
      for x in range(d.width):
        for y in range(d.height):
          c = colors[((x + y + i) // stripeWidth) % len(colors)]
          d.SetPixelColorRGB(x, y, c[0], c[1], c[2])
      d.Show()
      yield 0.4

# ===

def SolidColor(d):
  r = 100
  k = 80
  while True:
    for i in range(k):
      d.FillColorRGB(115 * (r - k + i) // r, 35 * i // k, 0)
      d.Show()
      yield 0.02
    yield 1
    for i in range(k):
      d.FillColorRGB(115 * (r - i) // r, 35 * (k - i) // k, 0)
      d.Show()
      yield 0.02

def Heartbeat(d):
  low = 20
  values = [20, 25, 30, 35, 40, 45, 40, 35, 30, 25, 20]
  while True:
    for v in values:
      d.FillColorRGB(v, 0, 0)
      yield
    for v in values:
      d.FillColorRGB(v, 0, 0)
      yield
    for _ in range(25):
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

# Bugs and helper (RandomWalk)
def RandomWalk(width, height):
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
    yield (x, y)

def Bugs(d):
  duration = 300
  walkers = []
  for _ in range(7):
    walkers.append(RandomWalk(d.width, d.height))

  for _ in range(duration):
    for w in walkers:
      (x, y) = next(w)
      d.SetPixelColorRGB(x, y, 0, 0, 40)
    yield True

# ===

def PulseWithEffects(d):
  background = Heartbeat(d)
  interval = 10.0

  nextEffectTime = time.time() + interval + random.random() * interval
  effect = None
  for _ in background:
    if not effect and time.time() > nextEffectTime:
      nextEffectTime = time.time() + interval + random.random() * interval
      if random.random() < 0.0: #0.2
        for _ in Strobe(d):
          d.Show()
          yield
      else:
        effect = Bugs(d)
    if effect and not next(effect, False):
        effect = None
    d.Show()
    yield

# Main loop and helpers
def ShouldShowEffectNow(now):
  hr = now.tm_hour
  if hr >= 16:
    return True
  if hr >= 5 and hr <= 8:
    return True
  return False

def ClearDisplay(d):
  d.FillColorRGB(0, 0, 0)
  d.Show()

def MainLoop(d, effects, auto_off):
  effect_it = None
  iteration = 0

  while True:
    # Turn off the display during the day
    if iteration % 100 == 0 and auto_off \
        and not ShouldShowEffectNow(time.localtime()):
      ClearDisplay(d)
      effect_it = None
      time.sleep(60)
    # Choose next effect
    if effect_it == None:
      effect_it = random.choice(effects)(d)
      effectStartTime = time.time()
      continue
    # Reset current effect after 10 min
    if time.time() - effectStartTime >= 600:
      ClearDisplay(d)
      effect_it = None
      continue
    iteration += 1
    assert(effect_it)
    delay = next(effect_it)
    if delay == None:
      time.sleep(0.02)
    else:
      time.sleep(delay)

def main():
  strip = display.CreateDefaultStrip()
  strip.begin()

  d = display.Display(strip, 5)
  try:
    # auto_off = True
    auto_off = False
    # Christmas effects.
    # effects = [RandomColors, Stripes, Snowflakes]
    # effects = [Snowflakes]

    # Halloween effects.
    # effects = [SolidColor]
    effects = [PulseWithEffects]
    MainLoop(d, effects, auto_off)
  except KeyboardInterrupt:
    ClearDisplay(d)

if __name__ == '__main__':
  main()
