def answer(x, y, n):
  pass



def CountRabbitsVisibleFromLeft(lineup):
  tallest = -1
  count = 0
  for rabbit in lineup:
    if rabbit > tallest:
      tallest = rabbit
      count += 1
  return count


def main():
  print(CountRabbitsVisibleFromLeft([3,2,1]))

if __name__ == '__main__':
  main()