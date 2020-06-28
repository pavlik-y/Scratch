def applyCursors(tokens, cursor_iter):
  y = 0
  x = 0
  c = next(cursor_iter, None)
  for (v, t) in tokens:
    if v == "\n":
      if c != None and c[0] == y:
        yield (" ", (t | 64))
        yield (v, t)
        c = next(cursor_iter, None)
      y += 1
      x = 0
      continue
    while v != None:
      if c != None and c[0] == y and x + len(v) > c[1]:
        if c[1] > x:
          yield (v[:c[1] - x], t)
        yield (v[c[1] - x], t | 64)
        v = v[c[1] - x + 1:]
        x = c[1] + 1
        c = next(cursor_iter, None)
      else:
        yield (v, t)
        x += len(v)
        v = None

def cropToWindow(tokens, offset_y, offset_x, height, width):
  y = 0
  x = 0
  for (v, t) in tokens:
    if v == "\n":
      if offset_y <= y < offset_y + height:
        yield (v, t)
      y+= 1
      x = 0
      continue
    if y < offset_y:
      continue
    if offset_y + height <= y:
      break
    # y within bounds
    if x + len(v) <= offset_x or x >= offset_x + width:
      x += len(v)
      continue
    # at least part of the token is visible
    if x < offset_x:
      v = v[offset_x - x:]
      x = offset_x
    if x + len(v) > offset_x + width:
      yield (v[:offset_x + width - x], t)
    else:
      yield (v, t)
    x += len(v)


def main():
  tokens = [("asdf", 1), ("\n", 4), ("qwer", 2), ("zxcv", 3)]
  cursors = [(0, 8), (1, 2), (1, 5)]
  tokens = applyCursors(tokens, iter(cursors))
  tokens = cropToWindow(tokens, 1, 1, 2, 3)
  print(list(tokens))


if __name__ == '__main__':
  main()