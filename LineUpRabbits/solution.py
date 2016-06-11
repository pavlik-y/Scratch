import itertools

def answer(x, y, n):
  count = 0
  permutations_considered = 0
  for lineup in itertools.permutations(range(n)):
    permutations_considered += 1
    if CountRabbitsVisibleFromLeft(lineup) != x:
      continue
    if CountRabbitsVisibleFromRight(lineup) != y:
      continue
    count += 1
  print(permutations_considered)
  return str(count)

def CountRabbitsVisibleFromLeft(lineup):
  tallest = -1
  count = 0
  for rabbit in lineup:
    if rabbit > tallest:
      tallest = rabbit
      count += 1
  return count

def CountRabbitsVisibleFromRight(lineup):
  tallest = -1
  count = 0
  for rabbit in reversed(lineup):
    if rabbit > tallest:
      tallest = rabbit
      count += 1
  return count

def main():
  print(answer(2,2,3))
  print(answer(1,2,6))

if __name__ == '__main__':
  main()