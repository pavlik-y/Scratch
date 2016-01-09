import Paxos
import Environment as env

class _Test_StableStorage:
    def __init__(self):
        pass
    def Write(self, req):
        print(req)
        
def _Test_PaxosSingleDecree_TypicalSuccessScenario_SeparateObjects():
    stableStorage = _Test_StableStorage()
    replicaSet = ["a1", "a2", "a3"]
    decree = 100
    proposer = Paxos.ProposerInstance(decree, replicaSet)
    learner = Paxos.LearnerInstance(decree, replicaSet, stableStorage)
    acceptors = []
    for memberId in replicaSet:
        acceptor = Paxos.AcceptorInstance(memberId, decree, stableStorage)
        acceptors.append(acceptor)
    prepareReq = proposer.Prepare(Paxos.Ballot("a1", 5), )
    assert(isinstance(prepareReq, Paxos.PrepareRequest))
    for acceptor in acceptors:
        prepareResp = acceptor.HandlePrepare(prepareReq)
        proposer.HandlePrepareResponse(prepareResp)
    assert(proposer.ReadyToVote())
    lastAccVote = proposer.GetLastAcceptedVote()
    (needRevote, voteData) = lastAccVote
    if not needRevote:
        voteData = "abra"
    voteReq = proposer.Vote(voteData)
    for acceptor in acceptors:
        voteResp = acceptor.HandleVote(voteReq)
        learner.HandleVoteResponse(voteResp)
        print(learner.VotePassed())
    print(learner.GetPassedVote())

class _TestAppObject():
    def ReadyToVote(self, paxosInstance):
        print("ReadyToVote")
        assert(paxosInstance.IsReadyToVote())
        paxosInstance.Vote()
        # send vote
    def GetRequest(self):
        print("GetRequest")
        return 'abra'
    def VotePassed(self, paxosInstance):
        print("VotePassed")
        assert(paxosInstance.IsVotePassed())
        print(paxosInstance.GetPassedVote())
        # broadcast VotePassed
        # call app if needed
        
def _Test_PaxosSingleDecree_TypicalSuccessScenario_PaxosInstance():
    # Initialize
    stableStorage = _Test_StableStorage()
    replicaSet = ["a1", "a2", "a3"]
    decree = 100
    instances = []
    for memberId in replicaSet:
        app = _TestAppObject()
        instance = Paxos.PaxosInstance(memberId, decree, replicaSet, stableStorage, app)
        instance.Start()
        instances.append(instance)
    primary = instances[0]
    
    # Send prepare
    ballot = Paxos.Ballot(primary.memberId, 5)
    prepareReq = primary.Prepare(ballot)
    for memberId in replicaSet:
        env.Network().SendMessage(memberId, prepareReq)

    # Exchange messages
    yield 20

    # Wait for prepare stage to complete
    assert(primary.IsReadyToVote())
    
    # Send Vote
    lastAccVote = primary.GetLastAcceptedVote()
    (needRevote, voteData) = lastAccVote
    if not needRevote:
        voteData = "abra"
    voteReq = primary.Vote(voteData)
    
    for memberId in replicaSet:
        env.Network().SendMessage(memberId, voteReq)

    # Exchange messages
    yield 20
    
    for instance in instances:
        assert(instance.IsVotePassed())
        print(instance.GetPassedVote())
    for instance in instances:
        instance.Stop()


if __name__ == "__main__":
    env.SetNetwork(env.ReliableNetwork())
#    _Test_PaxosSingleDecree_TypicalSuccessScenario_SeparateObjects()
    env.StartThread(_Test_PaxosSingleDecree_TypicalSuccessScenario_PaxosInstance(), 0)
    env.Run()
