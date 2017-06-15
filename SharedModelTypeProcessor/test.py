from message_queue import MessageQueue

class TestObject:
  def foo(self, a, b):
    print a, b;
    print a, b;

def main():
  mq = MessageQueue()
  to = TestObject()

  proxy = mq.CreateObjectProxy(to)
  proxy.foo(1, "2")
  print "after call"
  mq.Run()

if __name__ == '__main__':
  main()