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

def SumDigit(a, b, ci):
  s = ord(a) + ord(b) + ord(ci) - 144
  r = chr(s%10 + 48)
  co = chr(s/10 + 48)
  return (r, co)

def LongSum(a, b):
  l = max(len(a), len(b))
  a = a.zfill(l)
  b = b.zfill(l)
  c = "0"
  res = ""
  for i in xrange(l):
    (r, c) = SumDigit(a[-1 - i], b[-1 -i], c)
    res += r
  if c != "0":
    res += c
  return res[::-1]



def AddDeps(deps, n):
  if n < 0:
    return
  if n in deps:
    return
  deps.add(n)
  AddDeps(deps, n/2)
  if n % 2 == 1:
    AddDeps(deps, n/2-1)
  else:
    AddDeps(deps, n/2+1)

def PrintDependencies(n):
  deps = set()
  AddDeps(deps, n)
  print(n)
  print(sorted(deps))

def main():
  print(LongSum("923", "456"))

if __name__ == '__main__':
  main()