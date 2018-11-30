import collections

Player = collections.namedtuple('Player', ['l', 'r'])
State = collections.namedtuple('State', ['p1', 'p2', 'p1_turn'])
Move = collections.namedtuple('Move', ['fr', 'to'])

def IsPlayerValid(pl):
  if pl.l < 0 or pl.r < 0:
    return False
  if pl.l > 4 or pl.r > 4:
    return False
  return True

def CanonicalizePlayer(pl):
  if pl.l > pl.r:
    pl = Player(l=pl.r, r=pl.l)
  return pl

def GenerateSplits(pl):
  result = set()
  for i in xrange(1, pl.l):
    new_pl = Player(l=pl.l-i, r=pl.r+i)
    if not IsPlayerValid(new_pl):
      continue
    new_pl = CanonicalizePlayer(new_pl)
    if new_pl == pl:
      continue
    result.add(new_pl)
  for i in xrange(1, pl.r):
    new_pl = Player(l=pl.l+i, r=pl.r-i)
    if not IsPlayerValid(new_pl):
      continue
    new_pl = CanonicalizePlayer(new_pl)
    if new_pl == pl:
      continue
    result.add(new_pl)
  return result

def TrimPlayer(pl):
  if pl.l > 4:
    assert(pl.r <= 4)
    return Player(0, pl.r)
  if pl.r > 4:
    assert(pl.l <= 4)
    return Player(pl.l, 0)
  return pl

def GenerateAttacks(p1, p2):
  result = []
  if p1.l > 0:
    if p2.l > 0:
      result.append(Player(p2.l + p1.l, p2.r))
    if p2.r > 0:
      result.append(Player(p2.l, p2.r + p1.l))
  if p1.r > 0:
    if p2.l > 0:
      result.append(Player(p2.l + p1.r, p2.r))
    if p2.r > 0:
      result.append(Player(p2.l, p2.r + p1.r))
  return set([CanonicalizePlayer(TrimPlayer(pl)) for pl in result])

def GenerateNextStates(state):
  if state.p1_turn:
    for s in  GenerateSplits(state.p1):
      yield State(s, state.p2, False)
    for a in GenerateAttacks(state.p1, state.p2):
      yield State(state.p1, a, False)
  else:
    for s in  GenerateSplits(state.p2):
      yield State(state.p1, s, True)
    for a in GenerateAttacks(state.p2, state.p1):
      yield State(a, state.p2, True)

def GenerateGraph(initial_state):
  visited_states = set()
  to_explore = [initial_state]
  graph = {}
  while len(to_explore) > 0:
    state = to_explore.pop()
    graph[state] = set()
    for next_state in GenerateNextStates(state):
      graph[state].add(next_state)
      if next_state in visited_states:
        continue
      visited_states.add(next_state)
      to_explore.append(next_state)
  return graph

def IsLostState(st):
  if st.p1_turn and st.p1.l == 0 and st.p1.r == 0:
    return True
  return False

def AnalyzeBadStates(graph):
  bad_states = set()
  for st in graph:
    if IsLostState(st):
      bad_states.add(st)
  while True:
    changed = False
    for st in graph:
      if st in bad_states:
        continue
      if st.p1_turn:
        if graph[st] & bad_states == graph[st]:
          changed = True
          bad_states.add(st)
      else:
        if graph[st] & bad_states:
          changed = True
          bad_states.add(st)
    if not changed:
      break
  return bad_states

def FindBadMoves(graph, bad_states):
  for st1 in graph:
    if st1 in bad_states:
      continue
    for st2 in graph[st1] & bad_states:
      yield Move(st1, st2)

def main():
  st = State(Player(1,1), Player(1,1), True)
  graph = GenerateGraph(st)
  print(len(graph))
  bad_states = AnalyzeBadStates(graph)
  print(len(bad_states))
  bad_moves = list(FindBadMoves(graph, bad_states))
  print(len(bad_moves))
  for move in bad_moves:
    print("%s -> %s" % (move.fr, move.to))

if __name__ == "__main__":
  main()
