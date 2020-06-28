COLOR_COUNT = 6

def calcScore(code, guess):
  assert(len(code) == len(guess))
  exact_match = 0
  for c,g in zip(code, guess):
    if c == g:
      exact_match += 1
  counts_c = [0] * COLOR_COUNT
  counts_g = [0] * COLOR_COUNT
  match_count = 0
  for c in code:
    counts_c[c] += 1
  for g in guess:
    counts_g[g] += 1
  for c, g in zip(counts_c, counts_g):
    match_count += min(c,g)
  match_count -= exact_match
  return (exact_match, match_count)

def isGuessConsistent(guesses, new_guess):
  for g, s in guesses:
    if calcScore(new_guess, g) != s:
      return False
  return True

def main():
  code = [0, 1, 0, 2]
  guesses = [
    [4,1,3,5],
    [1,2,3,4],
    [5,3,2,0],
  ]
  new_guess = [0,1,0,2]
  guesses = [(g, calcScore(code, g)) for g in guesses]
  # print(guesses)
  result = isGuessConsistent(guesses, new_guess)
  print(result)
  # score = calcScore(code, guess)
  # print(score)

if __name__ == "__main__":
  main()