class ObjectProxy:
  def __init__(self, message_queue, target):
    self.message_queue = message_queue
    self.target = target
  def __getattr__(self, name):
    tf = getattr(self.target, name)
    def f(*args, **kwargs):
      closure = lambda: tf(*args, **kwargs)
      self.message_queue.Enqueue(closure)
    return f

class MessageQueue:
  def __init__(self):
    self.queue = []
  def CreateObjectProxy(self, target):
    return ObjectProxy(self, target)
  def Enqueue(self, closure):
    self.queue.append(closure)
  def Run(self):
    while len(self.queue) > 0:
      self.RunOne()
  def RunOne(self):
    closure = self.queue.pop(0)
    closure()

