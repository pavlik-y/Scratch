import curses
import sys
import time

import document
import window
import testscr

class Context:
  def __init__(self):


class Editor:
  def __init__(self, scr):
    self.scr = scr

  def Run(self):
    self.context = Context()
    # curses.raw()
    content = ""
    if len(sys.argv) > 1:
      content = open(sys.argv[1], "rt").read();
    doc = document.Document(content)
    win = self.scr.derwin(40, 40, 1, 1)
    win.immedok(False)

    curses.curs_set(0)
    self.scr.refresh()
    self.scr.border()
    parsed_document = document.ParsedDocument(doc)
    self.window = window.Window(doc, parsed_document, win)
    self.context.window = self.window
    self.MainLoop()

  def MainLoop(self):
    # self.window.Render()
    while True:
      self.window.Render()
      ch = self.scr.getch()
      key = curses.keyname(ch)

      if key == 'q':
        break
      elif key == "KEY_DOWN":
        self.window.LineDown()
      elif key == "KEY_UP":
        self.window.LineUp()
      elif key == "KEY_LEFT":
        self.window.CharLeft()
      elif key == "KEY_RIGHT":
        self.window.CharRight()
      elif len(key) == 1:
        self.window.InsertChar(key)

def main(stdscr):
  editor = Editor(stdscr)
  editor.Run()


if __name__ == '__main__':
  curses.wrapper(main)
  # scr = testscr.TestScr()
  # main(scr)