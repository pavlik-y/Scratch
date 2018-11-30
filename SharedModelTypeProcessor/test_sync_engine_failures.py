class Worker(object):
  def __init__(self):
    pass
  def ProcessGUResponse(self, updates):
    pass

def TestGUFailures():
  worker = Worker()
  worker.ProcessGUResponse()

def main():
  TestGUFailures()

if __name__ == '__main__':
  main()