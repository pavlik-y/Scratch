class Piece:
  def __init__(self, id, edges):
    self.left = edges[0]
    self.top = edges[1]
    self.right = edges[2]
    self.bottom = edges[3]
    self.edges = edges
    self.id = id
  def __str__(self):
    return "%s:%s" % (self.id, self.edges)


class Raft:
  def __init__(self, dims, leftEdge, topEdge, rightEdge, bottomEdge):
    self.field = {}
    self.dims = dims
    self.InitEdges(leftEdge, topEdge, rightEdge, bottomEdge)
    
  def InitEdges(self, leftEdge, topEdge, rightEdge, bottomEdge):
    for i in range(self.dims[0]):
      piece = Piece('E', '000' + topEdge[i])
      self.PutPiece((i, -1), piece)
      piece = Piece('E', '0' + bottomEdge[i] + '00')
      self.PutPiece((i, self.dims[1]), piece)
    for i in range(self.dims[1]):
      piece = Piece('E', '00' +  leftEdge[i] + '0')
      self.PutPiece((-1, i), piece)
      piece = Piece('E', rightEdge[i] + '000')
      self.PutPiece((self.dims[0], i), piece)

  def CanPutPiece(self, pos, piece):
    if self.IsPositionOccupied(pos):
      return False
    up = (pos[0], pos[1] - 1)
    if self.IsPositionOccupied(up) and self.field[up].bottom != piece.top:
      return False
    right = (pos[0] + 1, pos[1])
    if self.IsPositionOccupied(right) and self.field[right].left != piece.right:
      return False
    down = (pos[0], pos[1] + 1)
    if self.IsPositionOccupied(down) and self.field[down].top != piece.bottom:
      return False
    left = (pos[0] - 1, pos[1])
    if self.IsPositionOccupied(left) and self.field[left].right != piece.left:
      return False
    return True
  def IsPositionOccupied(self, position):
    return position in self.field
  def PutPiece(self, position, piece):
    self.field[position] = piece
  def RemovePiece(self, position):
    del self.field[position]

class Stats:
  def __init__(self):
    self.totalTrials = 0
  def CountPlacement(self):
    self.totalTrials += 1
    if self.totalTrials % 100 == 0:
      print(self.totalTrials)

def GetNextAvailablePosition(raft):
  for y in range(raft.dims[1]):
    for x in range(raft.dims[0]):
      pos = (x, y)
      if not raft.IsPositionOccupied(pos):
        return pos
  return None

def SolveRaft(raft, pieces, usedPieces, stats):
  if len(pieces) == len(usedPieces):
    return True
  pos = GetNextAvailablePosition(raft)
  for piece in pieces:
    if piece in usedPieces:
      continue
    if not raft.CanPutPiece(pos, piece):
      continue
    usedPieces.add(piece)
    raft.PutPiece(pos, piece)
    print("PutPiece: l=%s, pos=%s, piece=%s" % (len(usedPieces), pos, piece))
    stats.CountPlacement()
    if SolveRaft(raft, pieces, usedPieces, stats):
      return True
    if stats.totalTrials >= 1000:
      return False
    raft.RemovePiece(pos)
    usedPieces.discard(piece)
  return False


def TestSolver():
  raft = Raft((3, 4), ('<<<>'), ('vvv'), ('><<<'), ('vvv'))
  #raft = Raft((3, 4), ('>>><'), ('^^^'), ('<>>>'), ('^^^'))
  pieces = (
    Piece('A', '>v>^'),
    Piece('B', '>^>^'),
    Piece('C', '<v<v'),

    Piece('D', '<^<^'),
    Piece('E', '<^>^'),
    Piece('F', '<^>v'),

    Piece('G', '>v<v'),
    Piece('H', '<v>^'),
    Piece('I', '>^>v'),

    Piece('J', '>v>v'),
    Piece('K', '>^<v'),
    Piece('L', '>v<^'),
    )
  usedPieces = set()
  res = SolveRaft(raft, pieces, usedPieces, Stats())
  print(res)
if __name__ == "__main__":
  TestSolver()

