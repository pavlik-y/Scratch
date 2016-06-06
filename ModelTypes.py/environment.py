import collections

class MessageLoop(object):
  def __init__(self):
    self._tasks = collections.deque()

  def PostTask(self, task):
    self._tasks.append(task)
    pass

  def RunUntilIdle(self):
    while len(self._tasks) > 0:
      task = self._tasks.popleft()
      task()
