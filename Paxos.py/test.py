class S:
    def __init__(self):
        self.i = 5
    def foo(self):
        print(self.i)

def bar(cb):
    cb()

s = S()
print(isinstance(s, S))
bar(s.foo)

