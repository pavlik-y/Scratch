class CallChecker:
  def __init__(self, failure_exception, args):
    self.failure_exception = failure_exception
    self.args = args
    pass
  def WithArgs(self, *args):
    if self.args != args:
      raise self.failure_exception
  def Any(self):
    pass
  def CaptureArgs(self):
    return self.args

class ReturnValueSetter:
  def __init__(self, f):
    self.f = f
  def Return(self, value):
    self.f.SetReturnValue(value)

class FunctionTracker:
  def __init__(self, failure_exception):
    self.calls = []
    self.failure_exception = failure_exception
    self.return_value = None
  def AppendCall(self, args):
    self.calls.append(args)
    return self.return_value
  def AssertCall(self):
    if len(self.calls) == 0:
      raise self.failure_exception
    return CallChecker(self.failure_exception, self.calls.pop(0))
  def ReturnValueSetter(self):
    return ReturnValueSetter(self)
  def SetReturnValue(self, value):
    self.return_value = value

class Mock:
  def __init__(self, failure_exception = AssertionError):
    self.failure_exception = failure_exception
    self.functions = {}
  def GetFunction(self, name):
    return self.functions.setdefault(name, FunctionTracker(self.failure_exception))
  def __getattr__(self, name):
    def f(*args):
      func = self.GetFunction(name)
      return func.AppendCall(args)
    return f
  def AssertCall(self, name):
    f = self.GetFunction(name)
    return f.AssertCall()
  def WhenCalled(self, name):
    f = self.GetFunction(name)
    return f.ReturnValueSetter()


def main():
  mock = Mock(AssertionError)
  mock.f("qwer", 1243)
  mock.g("qwer", 1243)
  mock.f("sdfg")
  mock.AssertCall("f").WithArgs("qwer", 1243)
  mock.AssertCall("f").Any()
  mock.AssertCall("g").Any()

if __name__ == '__main__':
  main()