class A:
  def __init__(self, i):
    self.i = i
  def f(self, i):
    self.i = i

class B(A):
  def __init__(self, i, j):
    A.__init__(self, i)
    self.j = j
  def g(self, j):
    self.j = j
  def __str__(self):
    return "B(%d,%d)" % (self.i, self.j)

b = B(1,2)
print(b)
b.f(5)
print(b)
b.g(10)
print(b)
