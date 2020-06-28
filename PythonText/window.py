import render

class Pos:
  def __init__(self, y, x):
    self.y = y
    self.x = x

class Selection:
  def __init__(self, start, end):
    self.start = start
    self.end = end

class Window:
  def __init__(self, document, parsed_document, win):
    self.document = document
    self.parsed_document = parsed_document
    self.win = win
    self.offset_y = 0
    self.offset_x = 0
    self.cursor = Pos(0, 0)
    self.selection = Selection(0, 0)
    self.width = 40 # {PAV} Fix
    self.height = 40 # {PAV} Fix

  def Render(self):
    self.win.erase()
    self.win.border()

    (y, x) = self.document.PosToValidPos(self.cursor.y, self.cursor.x)
    render.Renderer.Render(self.win,
        self.parsed_document.GetTokens(),
        self.offset_y, self.offset_x, self.height, self.width, [(y, x)])
    self.win.refresh()

  # adjust offset to make position visible
  def MakePositionVisible(self, y, x):
    if self.offset_y > y:
      self.offset_y = y
    if self.offset_y <= y - self.height:
      self.offset_y = y - self.height + 1
    if self.offset_x > x:
      self.offset_x = x
    if self.offset_x <= x - self.width:
      self.offset_x = x - self.width + 1

  def PrevCharDocumentPosition(self, line, col):
    if col > 0:
      col -= 1
    else:
      if line > 0:
        line -= 1
        col = self.document.LineLength(line)
    return (line, col)

  def InsertChar(self, ch):
    (y, x) = self.document.PosToValidPos(self.cursor.y, self.cursor.x)
    offset = self.document.PosToOffset(y, x)
    self.document.ReplaceRange(offset, offset, ch)
    (self.cursor.y, self.cursor.x) = self.document.OffsetToPos(offset + 1)

  def LineDown(self):
    self.cursor.y = min(self.cursor.y + 1,
        self.document.LineCount() - 1)
    (line, col) = self.document.PosToValidPos(self.cursor.y, self.cursor.x)
    self.MakePositionVisible(line, col)

  def LineUp(self):
    self.cursor.y = max(self.cursor.y - 1, 0)
    (line, col) = self.document.PosToValidPos(self.cursor.y, self.cursor.x)
    self.MakePositionVisible(line, col)

  def CharRight(self):
    (y, x) = self.document.PosToValidPos(self.cursor.y, self.cursor.x)
    offset = self.document.PosToOffset(y, x)
    if offset == len(self.document.content):
      return
    offset += 1
    (self.cursor.y, self.cursor.x) = self.document.OffsetToPos(offset)
    self.MakePositionVisible(self.cursor.y, self.cursor.x)

  def CharLeft(self):
    (y, x) = self.document.PosToValidPos(self.cursor.y, self.cursor.x)
    offset = self.document.PosToOffset(y, x)
    if offset == 0:
      return
    offset -= 1
    (self.cursor.y, self.cursor.x) = self.document.OffsetToPos(offset)
    self.MakePositionVisible(self.cursor.y, self.cursor.x)
