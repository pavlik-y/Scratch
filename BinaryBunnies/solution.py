C_cache = {}
def C(n, k):
  assert(0 <= k <= n)
  if k == 0 or k == n:
    return 1
  if (n, k) in C_cache:
    return C_cache[(n, k)]
  result = C(n-1, k-1) + C(n-1, k)
  C_cache[(n, k)] = result
  return result

class Node:
  def __init__(self, value):
    self.value = value
    self.less = None
    self.greater = None
    self.N = 0
    self.K = 0

def BuildTree(seq):
  root = None
  for v in seq:
    node = Node(v)
    if root == None:
      root = node
      continue
    parent = root
    while True:
      if node.value < parent.value:
        if parent.less != None:
          parent = parent.less
          continue
        else:
          parent.less = node
          break
      else:
        if parent.greater != None:
          parent = parent.greater
          continue
        else:
          parent.greater = node
          break
  return root

def PrintTree(node):
  if node == None:
    return
  PrintTree(node.greater)
  PrintTree(node.less)

def CalcTreeNumbers(node):
  if node == None:
    return (0, 1)
  less = CalcTreeNumbers(node.less)
  greater = CalcTreeNumbers(node.greater)
  node.N = less[0] + greater[0] + 1
  node.K = less[1] * greater[1] * C(node.N - 1, less[0])
  return (node.N, node.K)

def answer(seq):
  tree = BuildTree(seq)
  # calculate numbers on the tree
  CalcTreeNumbers(tree)
  # PrintTree(tree)
  # return result
  return str(tree.K)

def main():
  seq = [5, 9, 8, 2, 1]
  print answer(seq)
  seq = [1,2,3,4,5,6,7,8,9,10]
  print answer(seq)

if __name__ == '__main__':
  main()