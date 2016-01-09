import collections
import Environment as env
import kvs
import Replication as repl

class KVSService:
    def __init__(self, memberId, replicaSet):
        self.kvsState = kvs.KVSState()
        self.replicationEngine = repl.ReplicationEngine(memberId, replicaSet, self)
        self.requestQueue = collections.deque()
        self.keepRunning = False

    def GetNewRequest(self):
        try:
            return self.requestQueue.popleft()
        except IndexError:
            return None
    
    def ExecuteRequest(self, req):
        resp = self.kvsState.HandleRequest(req)
        env.Network().SendMessage(req.src, resp) 
        
    def Start(self):
        self.keepRunning = True
        self.replicationEngine.Start()
        env.StartThread(self._MainLoop(), 0)
        self.clientEndpoint = env.GenId()
        env.RegisterName("KVS:primary", self.clientEndpoint)

    def Stop(self):
        self.replicationEngine.Stop()
        env.UnregisterName("KVS:primary")
        self.keepRunning = False

    def _MainLoop(self):
        while self.keepRunning:
            requests = env.Network().ReceiveMessages(self.clientEndpoint)
            for req in requests:
                self.requestQueue.append(req)
            self.replicationEngine.NewRequestAvailable()
            yield 1

#==================

def _Test_Client(time):
    env.StartServices()
    reqId = env.GenId()
    req = kvs.SetValueRequest('abra', 'cadabra', reqId)
    chubbySocketId = env.ResolveName("KVS:primary")
    env.Network().SendMessage(chubbySocketId, req);
    while not env.Network().HasMessages(reqId):
        yield 1
    msgs = env.Network().ReceiveMessages(reqId)
    print(msgs)
    reqId = env.GenId()
    req = kvs.GetValueRequest('abra', reqId)
    env.Network().SendMessage(chubbySocketId, req)
    while not env.Network().HasMessages(reqId):
        yield 1
    msgs = env.Network().ReceiveMessages(reqId)
    print(msgs)
    yield time
    env.StopServices()
    
def _Test_KVSService():
    env.SetNetwork(env.ReliableNetwork())
    replicaSet = ["kvs0"]
    for memberId in replicaSet:
        kvs = KVSService(memberId, replicaSet)
        env.AddService(kvs)
    env.StartThread(_Test_Client(10), 1)
    env.Run()
    
if __name__ == "__main__":
    _Test_KVSService()
