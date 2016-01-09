def g(l):
  a = l.split(',')
  return (float(a[0]), float(a[1]))

f = open("data.txt", "r")
lines = f.readlines()
a = map(lambda x: g(x), lines)
b = reduce(lambda x, y: (x[0] + y[0], x[1] + y[1], x[2] + 1), a, (0.0, 0.0, 0))
print(b[0] / b[2], b[1] / b[2])