class A():
  def __init__(self, i):
    self.i = i
  def pr(self, b):
    print(self.i)
    print(b)

def main():
  a1 = A(1)
  a2 = A(2)
  f1 = a2.pr
  f1(4)

if __name__ == '__main__':
  main()