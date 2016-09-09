def gcd(x, y):
  while y != 0:
    (x, y) = (y, x % y)
  return x

def triangleArea(p1, p2, p3):
  v1 = (p2[0] - p1[0], p2[1] - p1[1])
  v2 = (p3[0] - p1[0], p3[1] - p1[1])
  xProd = float(abs(v1[0]*v2[1] - v1[1]*v2[0]))
  return xProd/2

def boundaryPoints(p1, p2):
  x = abs(p2[0] - p1[0])
  y = abs(p2[1] - p1[1])
  return gcd(x, y)

def answer(vertices):
  area = triangleArea(vertices[0], vertices[1], vertices[2])
  b = 0
  b += boundaryPoints(vertices[0], vertices[1])
  b += boundaryPoints(vertices[0], vertices[2])
  b += boundaryPoints(vertices[1], vertices[2])
  b = b
  i = area - float(b)/2 + 1
  return int(i)


# Inputs:
#     (int) vertices = [[2, 3], [6, 9], [10, 160]]
# Output:
#     (int) 289

# Inputs:
#     (int) vertices = [[91207, 89566], [-88690, -83026], [67100, 47194]]
# Output:
#     (int) 1730960165

def main():
  # 289

  # print(answer([[2, 3], [6, 9], [10, 160]]))
  print(answer([[91207, 89566], [-88690, -83026], [67100, 47194]]))
  # print(answer([[9, 8], [-8, -8], [6, 4]]))
  # print(triangleArea([91207, 89566], [-88690, -83026], [67100, 47194]))
  # print boundaryPoints([-88690, -83026], [67100, 47194])


if __name__ == '__main__':
  main()