import copy

class CubeState:
  rotation_sequences = (
    (
      ((0,0), (0,2), (0,8), (0,6)),
      ((0,1), (0,5), (0,7), (0,3)),
      ((2,0), (3,0), (4,2), (1,2)),
      ((2,3), (3,3), (4,1), (1,1)),
      ((2,6), (3,6), (4,0), (1,0)),
    ),
    (
      ((1,0), (1,2), (1,8), (1,6)),
      ((1,1), (1,5), (1,7), (1,3)),
      ((0,0), (4,0), (5,2), (2,2)),
      ((0,3), (4,3), (5,1), (2,1)),
      ((0,6), (4,6), (5,0), (2,0)),
    ),
    (
      ((2,0), (2,2), (2,8), (2,6)),
      ((2,1), (2,5), (2,7), (2,3)),
      ((1,0), (5,0), (3,2), (0,2)),
      ((1,3), (5,3), (3,1), (0,1)),
      ((1,6), (5,6), (3,0), (0,0)),
    ),
    (
      ((3,0), (3,2), (3,8), (3,6)),
      ((3,1), (3,5), (3,7), (3,3)),
      ((2,6), (5,6), (4,8), (0,8)),
      ((2,7), (5,7), (4,5), (0,5)),
      ((2,8), (5,8), (4,2), (0,2)),
    ),
    (
      ((4,0), (4,2), (4,8), (4,6)),
      ((4,1), (4,5), (4,7), (4,3)),
      ((0,6), (3,6), (5,8), (1,8)),
      ((0,7), (3,7), (5,5), (1,5)),
      ((0,8), (3,8), (5,2), (1,2)),
    ),
    (
      ((5,0), (5,2), (5,8), (5,6)),
      ((5,1), (5,5), (5,7), (5,3)),
      ((2,8), (1,6), (4,6), (3,8)),
      ((2,5), (1,7), (4,7), (3,5)),
      ((2,2), (1,8), (4,8), (3,2)),
    ),
  )
  def __init__(self, other=None):
    if other == None:
      self.facets = [[str(i)] * 9 for i in range(6)]
      self.RecalcHash()
    else:
      self.facets = copy.deepcopy(other.facets)
      self.hash = other.hash

  def Rotate(self, plane):
    sequence = self.rotation_sequences[plane]
    for group in sequence:
      self.RotateGroup(group)
    self.RecalcHash()

  def RotateGroup(self, group):
    last_tile = group[-1]
    last = self.facets[last_tile[0]][last_tile[1]]
    for tile in group:
      last, self.facets[tile[0]][tile[1]] = self.facets[tile[0]][tile[1]], last

  def __eq__(self, other):
    if other == None:
      return False
    for fi in range(6):
      for ti in range(9):
        if self.facets[fi][ti] != other.facets[fi][ti]:
          return False
    return True

  def __hash__(self):
    return self.hash

  def RecalcHash(self):
    self.hash = hash("".join(map(lambda x: "".join(x), self.facets)))

  def Print(self):
    for facet in self.facets:
      facet_str = ""
      for tile in facet:
        facet_str += tile
      print(facet_str)

def SolveCube(initial_cube):
  final_cube = CubeState()
  visited_states = {}
  state_queue = []
  state_queue.append(initial_cube)
  visited_states[initial_cube] = (None, 0, 0)
  while len(state_queue) > 0:
    state_ref = state_queue.pop(0)
    state = CubeState(state_ref)
    for plane in range(6):
      for steps in range(1, 4):
        state.Rotate(plane)
        if state in visited_states:
          continue
        next_state = CubeState(state)
        visited_states[next_state] = (state_ref, plane, steps)
        state_queue.append(next_state)
        if next_state == final_cube:
          return visited_states
        if len(visited_states) % 10000 == 0:
          cube = next_state
          steps = 0
          while True:
            v = visited_states[cube]
            if v[0] == None:
              break
            steps += 1
            cube = v[0]
          print("%d:%d:%d" % (len(visited_states), len(state_queue), steps))
      state.Rotate(plane)

def main():
  initial_cube = CubeState()
  cube = CubeState(initial_cube)
  cube.Rotate(0)
  cube.Rotate(1)
  cube.Rotate(2)
  cube.Rotate(5)
  cube.Rotate(2)
  cube.Rotate(1)

  cube.Rotate(4)

  # cube.Rotate(5)
  # cube.Rotate(3)
  # cube.Rotate(3)
  # cube.Rotate(0)
  states = SolveCube(cube)
  print(len(states))
  cube = CubeState()
  while True:
    v = states[cube]
    if v[0] == None:
      break
    print(v[1], v[2])
    cube = v[0]

if __name__ == '__main__':
  main()