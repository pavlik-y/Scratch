from message_queue import MessageQueue
import collections

class TestObject:
  def __init__(self):
    self.cond = False
  def foo(self, a, b):
    print a, b;
    print a, b;
  def AsyncFunction(self):
    print("Begin")
    for it in self.Download():
      yield None
    print("End")
  def Download(self):
    print("Begin download")
    while not self.cond:
      print("Not yet")
      yield None
    print("End download")


def TestMessageQueue():
  mq = MessageQueue()
  to = TestObject()

  proxy = mq.CreateObjectProxy(to)
  proxy.foo(1, "2")
  print "after call"
  mq.Run()

class AFExecutor:
  def __init__(self, f):
    self.f = f
    self.it = None
  def Continue(self):
    if self.it == None:
      self.it = self.f()
    try:
      self.it.next()
    except:
      pass
def Slon(af):
  request_tracker = Struct(request=request, response=None)
  def GetUpdates():
    request_tracker.response = server.GetUpdates(request_tracker.request)
    request_tracker.mq.Post(af.Continue)

  def GetUpdates(response):
    request_tracker.response = response
    af.Continue()
  server.GetUpdates(Updates)
  while request_tracker.response == None:
    yield None
  ProcessResponse(request_tracker.response)

def TestAsyncFunction():
  mq = MessageQueue()
  to = TestObject()

  ex = AFExecutor(lambda:to.AsyncFunction())
  ex.Continue()
  print("Here")
  to.cond = True
  print("Here")
  ex.Continue()


def main():
  TestAsyncFunction()

if __name__ == '__main__':
  main()