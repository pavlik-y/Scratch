import curses
import sys
import time
import pygments
import pygments.formatters
import pygments.lexers

def TestCurses(scr):
  curses.raw()
  curses.noecho()
  scr.keypad(0)
  try:
    while True:
      ch = scr.getch()
      if curses.keyname(ch) == 'q':
        break

      scr.addstr("[%d]" % ch)
      scr.addstr(curses.keyname(ch))
      scr.addstr("|\n")
      scr.refresh()
  except Exception as e:
    scr.addstr(str(e))
    scr.refresh()

if __name__ == '__main__':
  # curses.wrapper(TestCurses)
  scr = curses.initscr()
  TestCurses(scr)
  curses.endwin()
  # curses.echo()
  # curses.nl()
  # curses.noraw()
  # curses.nocbreak()
