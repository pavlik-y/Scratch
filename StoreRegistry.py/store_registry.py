from collections import deque

class BackendRegistry:
  def __init__(self):
    self.runners = {}
    self.backends = {}
  def GetRunner(self, path, runner):
    return self.runners.setdefault(path, runner)
  def GetBackend(self, path):
    return self.backends.get(path)
  def SetBackend(self, path, backend):
    assert(path not in self.backends)
    self.backends[path] = backend

backend_registry = BackendRegistry()


def GetOrCreateBackend(path, callback_runner, callback):
  print("GetOrCreateBackend")
  success = True
  backend = backend_registry.GetBackend(path)
  if backend == None:
    backend = Backend(path)
    success = backend.Init()
    if success:
      backend_registry.SetBackend(path, backend)
    else:
      backend = None
  def reply():
    print("Reply2")
    callback(success, backend, path)
  callback_runner.Post(reply)

def RegisterRunnerAndPostCreateBackend(path, backend_runner, callback,
      ui_runner):
  print("RegisterRunnerAndPostCreateBackend")
  backend_runner = backend_registry.GetRunner(path, backend_runner)
  def reply(success, backend, path):
    print "Reply1", success, path
    pass
  def task():
    print("Task1")
    GetOrCreateBackend(path, ui_runner, reply)
  backend_runner.Post(task)

class StoreImpl:
  pass

class Backend:
  def __init__(self, path):
    pass
  def Init(self):
    return False

class Runner:
  def __init__(self):
    self.pending_tasks = deque()
  def Post(self, task):
    self.pending_tasks.append(task)
  def RunTask(self):
    if len(self.pending_tasks) == 0:
      return False
    task = self.pending_tasks.popleft()
    task()
    return True

global_runner = Runner()


store = None

def StoreInitDone(created_store):
  print "Store init done"
  store = created_store

def CreateStore(create_callback):
  RegisterRunnerAndPostCreateBackend("default_path", global_runner,
      create_callback, global_runner)


def main():
  CreateStore(StoreInitDone)
  print("After CreateStore")
  while global_runner.RunTask():
    pass
  # store.Delete()


if __name__ == '__main__':
  main()