class TestScr:
  def __init__(self):
    pass
  def derwin(self, *args):
    return self
  def border(self):
    print("border")
  def getmaxyx(self):
    print("getmaxyx")
    return (40, 40)
  def erase(self):
    print("erase")
  def move(self, y, x):
    print("move:%d,%d" % (y, x))
  def addstr(self, value):
    print("addstr:{%s}" % (value))
  def refresh(self, *args):
    print("refresh")
  def getch(self):
    pass
  def immedok(self, flag):
    pass

