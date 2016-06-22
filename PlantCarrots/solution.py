def gcd(x, y):
  while y != 0:
    (x, y) = (y, x % y)
  return x

def boundingBox(vertices):
  x1 = min([v[0] for v in vertices])
  x2 = max([v[0] for v in vertices])
  y1 = min([v[1] for v in vertices])
  y2 = max([v[1] for v in vertices])
  return [[x1, y1], [x2, y2]]

def bbDimensions(vertices):
  bounding_box = boundingBox(vertices)
  dims = (bounding_box[1][0] - bounding_box[0][0],
                bounding_box[1][1] - bounding_box[0][1])
  return dims

def calcRectangleArea(vertices):
  (width, height) = bbDimensions(vertices)
  return (height + 1) * (width + 1)

def calcRectanglePerimeter(vertices):
  (width, height) = bbDimensions(vertices)
  return (height + width) * 2

def calcRightTriangleArea(vertices):
  (width, height) = bbDimensions(vertices)
  return calcRectangleArea(vertices) / 2 + gcd(width, height)


def isType1Triangle(vertices):
  bb = boundingBox(vertices)
  x = [bb[0][0], bb[1][0]]
  y = [bb[0][1], bb[1][1]]
  for v in vertices:
    if v[0] not in x and v[1] not in y:
      return False
  return True

def handleType1Triangle(vertices):
  area = 0
  area += calcRectangleArea(vertices)
  area -= calcRightTriangleArea((vertices[0], vertices[1]))
  area -= calcRightTriangleArea((vertices[0], vertices[2]))
  area -= calcRightTriangleArea((vertices[1], vertices[2]))
  area += 3
  return area

def handleType2Triangle(vertices):
  vertices = sorted(vertices, key=lambda x: x[0])
  bb = boundingBox(vertices)
  if vertices[0] != bb[0]:
    print("here")
    print(vertices)
    print(bb)
    vertices = [[v[0],-v[1]] for v in vertices]
    vertices = sorted(vertices, key=lambda x: x[0])
    bb = boundingBox(vertices)
    print(vertices)
    print(bb)
  corners = ((bb[0][0], bb[1][1]), (bb[1][0], bb[0][1]))
  corner = corners[0]
  if (calcRectangleArea((vertices[1], corners[0])) >
     calcRectangleArea((vertices[1], corners[1]))):
      corner = corners[1]
  area = 0
  print(calcRectangleArea(vertices))
  area += calcRectangleArea(vertices)
  print(calcRightTriangleArea((vertices[0], vertices[1])))
  area -= calcRightTriangleArea((vertices[0], vertices[1]))
  print(calcRightTriangleArea((vertices[0], vertices[2])))
  area -= calcRightTriangleArea((vertices[0], vertices[2]))
  print(calcRightTriangleArea((vertices[1], vertices[2])))
  area -= calcRightTriangleArea((vertices[1], vertices[2]))
  area += 3
  print((vertices[1], corner))
  square_dims = bbDimensions((vertices[1], corner))
  area -= square_dims[0] * square_dims[1]
  return area



def answer(vertices):
  if isType1Triangle(vertices):
    print("here")
    return handleType1Triangle(vertices)
  else:
    return handleType2Triangle(vertices)
  pass


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


if __name__ == '__main__':
  main()