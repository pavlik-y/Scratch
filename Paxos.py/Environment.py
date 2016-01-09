import collections
import heapq

# Time and scheduling
_currentTime = 0

def Time():
    return _currentTime

_tasks = []

def _ScheduleTask(task, time):
    heapq.heappush(_tasks, (time, task))
    
def _GetNextTask():
    try:
        return heapq.heappop(_tasks)
    except IndexError:
        return None

def StartThread(gen, delay):
    _ScheduleTask(gen, Time() + delay)

def Run():
    global _currentTime
    while(True):
        r = _GetNextTask()
        if r == None:
            break
        _currentTime, task = r
        r = next(task, None)
        if r != None:
            _ScheduleTask(task, _currentTime + r)

# unique ID
_lastId = 0

def GenId():
    global _lastId
    _lastId += 1
    return _lastId
    
# name resolution
_nameTable = {}
def RegisterName(name, value):
    global _nameTable
    _nameTable[name] = value

def ResolveName(name):
    try:
        return _nameTable[name]
    except KeyError:
        return None

def UnregisterName(name):
    global _nameTable
    try:
        del _nameTable[name]
    except KeyError:
        pass

# Services
_services = []
def AddService(s):
    global _services
    _services.append(s)

def StartServices():
    global _services
    for s in _services:
        s.Start()

def StopServices():
    global _services
    for s in _services:
        s.Stop()

# Network reference

_network = None

def Network():
    return _network

def SetNetwork(network):
    global _network
    _network = network

class _NetworkMessageEntry:
    def __init__(self, msg, time):
        self.msg = msg
        self.time = time

# Reliable network implementation
class ReliableNetwork:
    def __init__(self):
        self.dst = {}
        pass
    def SendMessage(self, dst, msg):
        queue = self._GetDstQueue(dst)
        queue.append(_NetworkMessageEntry(msg, Time() + 1))
    def HasMessages(self, dst):
        queue = self._GetDstQueue(dst)
        return len(queue) > 0 and queue[0].time <= Time()
    def ReceiveMessages(self, dst):
        queue = self._GetDstQueue(dst)
        ret = []
        currentTime = Time()
        while len(queue) > 0 and queue[0].time <= currentTime:
            ret.append(queue.popleft().msg)
        if len(queue) == 0:
            del self.dst[dst]
        return ret
    def _GetDstQueue(self, dst):
        if dst not in self.dst:
            self.dst[dst] = collections.deque()
        return self.dst[dst]

#================    

def _Network_Test():
    n = ReliableNetwork()
    n.SendMessage('a', 'Hello')
    n.SendMessage("b", "H")
    n.SendMessage("a", "b")
    assert(not n.HasMessages("a"))
    r = n.ReceiveMessages("a")
    assert(len(r) == 0)
    yield 1
    assert(n.HasMessages("a"))
    r = n.ReceiveMessages("a")
    assert(len(r) == 2)
    r = n.ReceiveMessages("a")
    assert(len(r) == 0)
    r = n.ReceiveMessages("b")
    assert(len(r) == 1)

def _Task_Test():
    _ScheduleTask("cadabra", 10)
    _ScheduleTask("abra", 5)
    results = []
    while(True):
        r = _GetNextTask()
        if r == None:
            break
        time, task = r
        results.append(task)
    assert(results == ["abra", "cadabra"])

def _Test_Thread():
    assert(Time() == 0)
    yield 1
    assert(Time() == 1)
    yield 2
    assert(Time() == 3)
    yield 3
    assert(Time() == 6)
    
def _Test_Scheduler():
    StartThread(_Test_Thread(), 0)
    Run()

def _Test_NameResolution():
    UnregisterName("abra")
    assert(ResolveName("abra") == None)
    RegisterName("abra", "cadabra")
    assert(ResolveName("abra") == "cadabra")
    UnregisterName("abra")
    assert(ResolveName("abra") == None)

class _Test_ServiceClass:
    def __init__(self):
        self.started = False
        self.stopped = False
    def Start(self):
        self.started = True
    def Stop(self):
        self.stopped = True
        
def _Test_Services():
    s = _Test_ServiceClass()
    AddService(s)
    assert(not s.started and not s.stopped)
    StartServices()
    assert(s.started and not s.stopped)
    StopServices()
    assert(s.started and s.stopped)

if __name__ == "__main__":
    _Task_Test()
    _Test_Scheduler()
    _Test_NameResolution()
    StartThread(_Network_Test(), 0)
    Run()
    _Test_Services()
    