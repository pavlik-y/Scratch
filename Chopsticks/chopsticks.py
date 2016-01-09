import copy

class GameState:
  def __init__(self, state, turn):
    self.state = state
    self.turn = turn
    self.next_states = []
    self.id = ""
  def PlayerToStr(self, player):
    return "%d%d" % (player[0], player[1])
  def __str__(self):
    return "%s:%s:%d" % (self.PlayerToStr(self.state[0]), self.PlayerToStr(self.state[1]), self.turn)
  def __repr__(self):
    return str(self)
  def GenerateAttack(self, attack_hand, defend_hand):
    state = copy.deepcopy(self.state)
    state[1 - self.turn][defend_hand] += state[self.turn][attack_hand]
    if state[1 - self.turn][defend_hand] > 4:
      state[1 - self.turn][defend_hand] = 0
    state[1 - self.turn].sort()
    return GameState(state, 1 - self.turn)
  def GenerateSplits(self):
    splits = []
    total = self.state[self.turn][1]
    for i in range(1, (self.state[self.turn][1] / 2) + 1):
      state = copy.deepcopy(self.state)
      state[self.turn][0] = i
      state[self.turn][1] = total - i
      splits.append(GameState(state, 1 - self.turn))
    return splits

  def GeneratePossibleMoves(self):
    possible_moves = []
    if self.state[self.turn][0] + self.state[self.turn][1] == 0:
      return []
    possible_moves.append(self.GenerateAttack(0, 0))
    possible_moves.append(self.GenerateAttack(0, 1))
    possible_moves.append(self.GenerateAttack(1, 0))
    possible_moves.append(self.GenerateAttack(1, 1))
    if self.state[self.turn][0] == 0:
      possible_moves.extend(self.GenerateSplits())
    return possible_moves
  def IsFinal(self):
    if self.state[0][0] + self.state[0][1] == 0:
      return True
    if self.state[1][0] + self.state[1][1] == 0:
      return True
    return False
  def IsLoosing(self):
    for ns in self.next_states:
      if ns.IsFinal():
        return True
    return False


def main():
  initial_state = GameState(([1,1],[1,1]), 0)
  state_queue = []
  state_map = {}
  state_queue.append(initial_state)
  state_map[str(initial_state)] = initial_state
  steps = 0;
  while len(state_queue) > 0:
    steps += 1
    state = state_queue.pop()
    # print(str(state))
    next_states = state.GeneratePossibleMoves()
    for next_state in next_states:
      if str(next_state) in state_map:
        state.next_states.append(state_map[str(next_state)])
        continue
      state.next_states.append(next_state)
      state_map[str(next_state)] = next_state
      state_queue.append(next_state)
  id = 0
  for s in state_map:
    state = state_map[s]
    id += 1
    state.id = ("s%d" % id)
    print('state "%s" as %s' % (s, state.id))
  for s in state_map:
    state = state_map[s]
    for next_state in state.next_states:
      print("%s -> %s" % (state.id, next_state.id))


  # print(steps)
  # loosing_count = 0
  # for s in state_map:
  #   state = state_map[s]
  #   if state.IsLoosing():
  #     print("%s is loosing" % state)
  #     loosing_count += 1
  # print loosing_count
  # for s in state_map:
  #   state = state_map[s]
  #   if state.IsFinal():
  #     continue
  #   bad_state = True
  #   for ns in state.next_states:
  #     if not ns.IsLoosing():
  #       bad_state = False
  #       break
  #   if bad_state:
  #     print("%s is bad_state" % state)

if __name__ == '__main__':
  main()