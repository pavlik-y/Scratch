import curses
import pygments.token

class Renderer:
  @classmethod
  def Render(cls, win, tokens, offset_y, offset_x, height, width, cursors):
    tokens = cls.applyCursors(tokens, iter(cursors))

    show_line_numbers = (width > 20)
    if show_line_numbers:
      width -= 5

    tokens = cls.cropToWindow(tokens, offset_y, offset_x, height, width)
    if show_line_numbers:
      tokens = cls.addLineNumbers(tokens, offset_y, height)

    y = 0
    win.move(y, 0)
    for (t, v, c) in tokens:
      if v == "\n":
        y += 1
        try:
          win.move(y, 0)
        except curses.error:
          pass
        continue
      attr = 0
      if c:
        attr = curses.A_STANDOUT
      try:
        win.addstr(v, attr)
      except curses.error:
        pass

  @staticmethod
  def applyCursors(tokens, cursor_iter):
    y = 0
    x = 0
    c = next(cursor_iter, None)
    for (t, v) in tokens:
      if v == "\n":
        if c != None and c[0] == y:
          yield (t, " ", True) # cursor
          c = next(cursor_iter, None)
        yield (t, v, False)
        y += 1
        x = 0
        continue
      while v != None:
        if c != None and c[0] == y and x + len(v) > c[1]:
          if c[1] > x:
            yield (t, v[:c[1] - x], False)
          yield (t, v[c[1] - x], True) # cursor
          v = v[c[1] - x + 1:]
          x = c[1] + 1
          c = next(cursor_iter, None)
        else:
          yield (t, v, False)
          x += len(v)
          v = None
    if c != None and c[0] == y and c[1] == x:
      yield (pygments.token.Token.Text, " ", True)

  @staticmethod
  def cropToWindow(tokens, offset_y, offset_x, height, width):
    y = 0
    x = 0
    for (t, v, c) in tokens:
      if v == "\n":
        if offset_y <= y < offset_y + height:
          yield (t, v, c)
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
        yield (t, v[:offset_x + width - x], c)
      else:
        yield (t, v, c)
      x += len(v)

  @staticmethod
  def addLineNumbers(tokens, offset_y, height):
    y = offset_y
    yield (pygments.token.Token.Text, "{0:>4d} ".format(y), False)
    for (t, v, c) in tokens:
      yield (t, v, c)
      if v == "\n":
        y += 1
        if y - offset_y < height:
          yield (pygments.token.Token.Text, "{0:>4d} ".format(y), False)
