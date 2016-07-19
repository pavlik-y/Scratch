# Breeding like rabbits
# =====================

# As usual, the zombie rabbits (zombits) are breeding... like rabbits! But
# instead of following the Fibonacci sequence like all good rabbits do, the
# zombit population changes according to this bizarre formula, where R(n) is the
# number of zombits at time n:

# R(0) = 1
# R(1) = 1
# R(2) = 2
# R(2n) = R(n) + R(n + 1) + n (for n > 1)
# R(2n + 1) = R(n - 1) + R(n) + 1 (for n >= 1)

# X(2n) = R(2n) + R(2n+1) = R(n) + R(n+1) + R(n-1) + R(n) + n + 1
# = X(n-1) + X(n) + n + 1
# (At time 2, we realized the difficulty of a breeding program with only one
# (zombit and so added an additional zombit.)

# Being bored with the day-to-day duties of a henchman, a bunch of Professor
# Boolean's minions passed the time by playing a guessing game: when will the
# zombit population be equal to a certain amount? Then, some clever minion
# objected that this was too easy, and proposed a slightly different game: when
# is the last time that the zombit population will be equal to a certain amount?
# And thus, much fun was had, and much merry was made.

# (Not in this story: Professor Boolean later downsizes his operation, and you
# (can guess what happens to these minions.)

# Write a function answer(str_S) which, given the base-10 string representation
# of an integer S, returns the largest n such that R(n) = S. Return the answer
# as a string in base-10 representation. If there is no such n, return "None". S
# will be a positive integer no greater than 10^25.

# Languages
# =========

# To provide a Python solution, edit solution.py
# To provide a Java solution, edit solution.java

# Test cases
# ==========

# Inputs:
#     (string) str_S = "7"
# Output:
#     (string) "4"

# Inputs:
#     (string) str_S = "100"
# Output:
#     (string) "None"

# Use verify [file] to test your solution and see how it does. When you are
# finished editing your code, use submit [file] to submit your answer. If your
# solution passes the test cases, it will be removed from your home folder.

import heapq

class F:
  def __init__(self):
    self.cache = {}
    self.cache[0] = 1
    self.cache[1] = 1
    self.cache[2] = 2

# R(2n) = R(n) + R(n + 1) + n (for n > 1)
# R(2n + 1) = R(n - 1) + R(n) + 1 (for n >= 1)
  def EnsureValueCached(self, x):
    if x in self.cache:
      return
    toCalculateSet = set([x])
    toCalculateQueue = []
    heapq.heappush(toCalculateQueue, x)
    while len(toCalculateSet) > 0:
      x = toCalculateQueue[0]
      n = x / 2
      if x % 2 == 0:
        (a, b) = (n, n + 1)
      else:
        (a, b) = (n - 1, n)
      if a in self.cache and b in self.cache:
        self.cache[x] = self.Calc(x)
        toCalculateSet.remove(x)
        heapq.heappop(toCalculateQueue)
        continue
      if a not in self.cache and a not in toCalculateSet:
        toCalculateSet.add(a)
        heapq.heappush(toCalculateQueue, a)
      if b not in self.cache and b not in toCalculateSet:
        toCalculateSet.add(b)
        heapq.heappush(toCalculateQueue, b)

  def GetCached(self, x):
    assert(x in self.cache)
    return self.cache[x]

  def Calc(self, x):
    n = x / 2
    if x % 2 == 0:
      result = self.GetCached(n) + self.GetCached(n + 1) + n
    else:
      result = self.GetCached(n - 1) + self.GetCached(n) + 1
    return result

  def run(self, x):
    self.EnsureValueCached(x)
    return self.cache[x]

def odd(x):
  return 2 * x + 1

def even(x):
  return 2 * x

def BinarySearch(v, f):
  i = 0
  while True:
    x = 1 << i
    if f(x) == v:
      return x
    if f(x) > v:
      break
    i += 1
  # {PAV} Handle special case
  i -= 1
  x = 1 << i
  while True:
    i -= 1
    if i < 0:
      return None
    x1 = x + (1 << i)
    if f(x1) == v:
      return x1
    if f(x1) < v:
      x = x1
  return None

def answer(x):
  x = long(x)
  f = F()
  e = lambda x: f.run(even(x))
  o = lambda x: f.run(odd(x))
  eres = BinarySearch(x, e)
  ores = BinarySearch(x, o)
  if eres > ores:
    return str(2*eres)
  if ores != None:
    return str(2*ores+1)
  return "None"

def main():
  print(answer("7"))
  print(answer("100"))

if __name__ == '__main__':
  main()