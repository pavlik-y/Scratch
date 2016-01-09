import Environment as env
import Paxos

#class ProposerInstance:
#    def __init__(self, memberId, replicaSet):
#        pass
#    def

class ReplicationEngine:
    def __init__(self, memberId, replicaSet, callbacks):
        self.callbacks = callbacks
        self.memberId = memberId
        self.keepRunning = False
        self.newRequestAvailable = False
        self.replicationEndpoint = env.GenId()
        env.RegisterName(memberId, self.replicationEndpoint)
        self.replicaSet = replicaSet
        self.replicaEndpoints = {}
        for replicaId in replicaSet:
            self.replicaEndpoints[replicaId] = None

    def Start(self):
        for rId in self.replicaEndpoints:
            self.replicaEndpoints[rId] = env.ResolveName(rId)
        ballot = Paxos.Ballot(self.memberId, 5)
        self.proposerInstance = Paxos.ProposerInstance(100, ballot, self.replicaSet)
        self.acceptorInstance = Paxos.AcceptorInstance(self.memberId, 100)
        self.keepRunning = True
        env.StartThread(self._MainLoop(), 0)

    def Stop(self):
        self.keepRunning = False
        
    def _MainLoop(self):
        while self.keepRunning: 
            if self.newRequestAvailable:
                self._StartReplicationForNewRequests()
                self.newRequestAvailable = False
            msgs = env.Network().ReceiveMessages(self.replicationEndpoint)
            for msg in msgs:
                if isinstance(msg, Paxos.PrepareRequest):
                    self._HandlePrepareRequest(msg)
                elif isinstance(msg, Paxos.PrepareResponse):
                    self._HandlePrepareResponse(msg)
                else:
                    raise "UnknownMessage"
            yield 1
            
    def _HandlePrepareRequest(self, msg):
        res = self.acceptorInstance.HandlePrepare(msg)
        env.Network().SendMessage(self.replicaEndpoints[msg.ballot.memberId], res)
        print res
    def _HandlePrepareResponse(self, msg):
        res = self.proposerInstance.HandlePrepareResponse(msg)
        assert(res)
        request = self.callbacks.GetNewRequest()
        self.newRequestAvailable = False
        assert(request != None)
        (userData, context) = request
        res = self.proposerInstance.Vote(userData)
        print res
        self.callbacks.ExecuteRequest(userData, context)

    def _StartReplicationForNewRequests(self):
        prepareMsg = self.proposerInstance.Prepare()
        print(prepareMsg)
        self._SendRequestToAllReplicas(prepareMsg)

    def _SendRequestToAllReplicas(self, req):
        for rId in self.replicaEndpoints:
            env.Network().SendMessage(self.replicaEndpoints[rId], req)

    def NewRequestAvailable(self):
        assert(self.newRequestAvailable == False)
        self.newRequestAvailable = True

class _Test_ReplicationEngine:
    def __init__(self):
        self.replicationEngine = ReplicationEngine("1", ["1"], self)
        self.generator = self.GenerateRequests()
        self.replicatedRequests = []
    def GenerateRequests(self):
        yield "abra"
#        yield "cadabra"
    def Run(self):
        self.replicationEngine.Start()
        yield 1
        self.replicationEngine.NewRequestAvailable()
        assert(self.replicationEngine.newRequestAvailable == True)
        yield 10
        assert(self.replicationEngine.newRequestAvailable == False)
        self.replicationEngine.Stop()
    def GetNewRequest(self):
        req = next(self.generator, None)
        return (req, req)
    def ExecuteRequest(self, req, context):
        self.replicatedRequests.append(context)
        
    
if __name__ == "__main__":
    env.SetNetwork(env.ReliableNetwork())
    test = _Test_ReplicationEngine()
    env.StartThread(test.Run(), 1)
    env.Run()
#    assert(test.replicatedRequests == ["abra", "cadabra"])
    assert(test.replicatedRequests == ["abra"])
