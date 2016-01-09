#import logging
import Environment as env

class Ballot:
    def __init__(self, memberId, ballotNumber):
        self.memberId = memberId
        self.ballotNumber = ballotNumber
    def IsValid(self):
        if self.memberId == None:
            assert(self.ballotNumber == None)
            return False
        return True
    def __repr__(self):
        return "Ballot('%s', %s)" % (self.memberId, self.ballotNumber)
    def __cmp__(self, other):
        if not self.IsValid():
            return -1 if other.IsValid() else 0
        # self is valid
        if not other.IsValid():
            return 1
        # both valid
        if self.ballotNumber != other.ballotNumber:
            return self.ballotNumber - other.ballotNumber
        if self.memberId != other.memberId:
            return -1 if self.memberId < other.memberId else 1
        return 0
Ballot.Min = Ballot(None, None)

class PrepareRequest:
    def __init__(self, decree, ballot):
        self.decree = decree
        self.ballot = ballot
    def __repr__(self):
        return "PrepareRequest(%s, %s)" % (self.decree, self.ballot)

class PrepareResponse:
    def __init__(self, decree, ballot, memberId, accepted, lastAcceptedBallot, lastAcceptedVoteData):
        self.decree = decree
        self.ballot = ballot
        self.memberId = memberId
        self.accepted = accepted
        self.lastAcceptedBallot = lastAcceptedBallot
        self.lastAcceptedVoteData = lastAcceptedVoteData
    def __repr__(self):
        return "PrepareResponse(%s, %s, '%s', %s, %s, %s)" % \
            (self.decree, self.ballot, self.memberId, self.accepted, self.lastAcceptedBallot, self.lastAcceptedVoteData)
        
class VoteRequest:
    def __init__(self, decree, ballot, voteData):
        self.decree = decree
        self.ballot = ballot
        self.voteData = voteData
    def __repr__(self):
        return "VoteRequest(%s, %s, '%s')" % (self.decree, self.ballot, self.voteData)

class VoteResponse:
    def __init__(self, decree, ballot, memberId, accepted, voteData):
        self.decree = decree
        self.ballot = ballot
        self.memberId = memberId
        self.accepted = accepted
        self.voteData = voteData
    def __repr__(self):
        return "VoteResponse(%s, %s, '%s', %s, '%s')" % \
            (self.decree, self.ballot, self.memberId, self.accepted, self.voteData)

class VotePassedRequest:
    def __init__(self, decree, ballot, voteData):
        self.decree = decree
        self.ballot = ballot
        self.voteData = voteData
    def __repr__(self):
        return "VotePassedRequest(%s, %s, '%s')" % (self.decree, self.ballot, self.voteData)
        
#===============================

class ProposerInstance:
    ST_Init = "Init"
    ST_Preparing = "Preparing"
    ST_ReadyToVote = "ReadyToVote"
    ST_Voting = "Voting"
    #ST_Done = "Done"
    
    def __init__(self, decree, acceptors):
        self.decree = decree
        self.responseMap = {}
        for memberId in acceptors:
            self.responseMap[memberId] = None
        self.state = ProposerInstance.ST_Init
        
    def Prepare(self, ballot):
        assert(self.state == ProposerInstance.ST_Init)
        self.state = ProposerInstance.ST_Preparing
        self.ballot = ballot
        return PrepareRequest(self.decree, self.ballot)

    def HandlePrepareResponse(self, prepareResponse):
        # Dispatcher should ensure messages for correct decree get passed to proposer
        assert(self.decree == prepareResponse.decree)
        # proposer might advance state (got quorum, messages out of order)
        if self.state != ProposerInstance.ST_Preparing:
            return
        # proposer might be reinitialized for higher ballot
        if self.ballot != prepareResponse.ballot:
            return
        # replica set is decided and fixed for decree, it is a function of previous decisions
        assert(prepareResponse.memberId in self.responseMap)
        assert(prepareResponse.accepted)
        # only first response counts
        if  self.responseMap[prepareResponse.memberId] == None:
            self.responseMap[prepareResponse.memberId] = prepareResponse
        # if we've got quorum then advance state ro "ready to vote"
        if self._CountResponses():
            self.state = ProposerInstance.ST_ReadyToVote
    def ReadyToVote(self):
        return self.state == ProposerInstance.ST_ReadyToVote

    def _CountResponses(self):
        accepted = 0
        for memberId in self.responseMap:
            response = self.responseMap[memberId]
            if response != None:
                accepted += 1
        return accepted >= (len(self.responseMap) / 2 + 1)
        
    def GetLastAcceptedVote(self):
        assert(self._CountResponses())
        highestBallot = Ballot.Min
        voteData = None
        for memberId in self.responseMap:
            response = self.responseMap[memberId]
            if response == None:
                continue
            if highestBallot < response.lastAcceptedBallot:
                highestBallot = response.lastAcceptedBallot
                voteData = response.lastAcceptedVoteData
        return (highestBallot != Ballot.Min, voteData)
        
    def Vote(self, voteData):
        assert(self.state == ProposerInstance.ST_ReadyToVote)
        assert(self._CountResponses())
        self.state = ProposerInstance.ST_Voting
        return VoteRequest(self.decree, self.ballot, voteData)

class AcceptorInstance:
    def __init__(self, memberId, decree, stableStorage):
        self.memberId = memberId
        self.decree = decree
        self.promisedBallot = Ballot.Min
        self.lastAcceptedBallot = Ballot.Min
        self.lastAcceptedVoteData = None
        self.stableStorage = stableStorage
        self.passedVote = None
        
    def HandlePrepare(self, prepareReq):
        assert(self.decree == prepareReq.decree)
        if self.passedVote != None:
            raise "NotImpl"
        if self.promisedBallot <= prepareReq.ballot:
            self.promisedBallot = prepareReq.ballot
            self.stableStorage.Write(prepareReq)
            return PrepareResponse(prepareReq.decree, prepareReq.ballot, self.memberId, 
                True, self.lastAcceptedBallot, self.lastAcceptedVoteData)
        else:
            raise "NotImpl"
        
    def HandleVote(self, voteReq):
        assert(self.decree == voteReq.decree)
        if self.passedVote != None:
            raise "NotImpl"
        if self.promisedBallot <= voteReq.ballot:
            self.promisedBallot = voteReq.ballot
            self.lastAcceptedBallot = voteReq.ballot
            self.lastAcceptedVoteData = voteReq.voteData
            self.stableStorage.Write(voteReq)
            return VoteResponse(voteReq.decree, voteReq.ballot, self.memberId, True, voteReq.voteData)
        else:
            raise "NotImpl"
    
    def HandleVotePassed(self, votePassedReq):
        assert(self.decree == votePassedReq.decree)
        self.stableStorage.Write(votePassedReq)
        self.passedVote = (votePassedReq.ballot, votePassedReq.voteData)
    
    def VotePassed(self):
        return self.passedVote != None
    def GetPassedVote(self):
        assert(self.VotePassed())
        return self.passedVote

class LearnerInstance:
    def __init__(self, decree, acceptors):
        self.decree = decree
        self.votePassed = False
        self.responseMap = {}
        for memberId in acceptors:
            self.responseMap[memberId] = None
    def HandleVoteResponse(self, voteResponse):
        # Dispatcher should ensure messages for correct decree get passed to learner
        assert(self.decree == voteResponse.decree)
        # replica set is decided and fixed for decree, it is a function of previous decisions
        assert(voteResponse.memberId in self.responseMap)
        assert(voteResponse.accepted)
        if self.votePassed:
            return
        # keep highest ballot response
        if  self.responseMap[voteResponse.memberId] == None or \
            self.responseMap[voteResponse.memberId].ballot < voteResponse.ballot:
            self.responseMap[voteResponse.memberId] = voteResponse
        if self._CountResponses():
            self.votePassed = True;
            self.passedVote = self._FindPassedVote()

    def _CountResponses(self):
        accepted = 0
        for memberId in self.responseMap:
            response = self.responseMap[memberId]
            if response != None:
                accepted += 1
        return accepted >= (len(self.responseMap) / 2 + 1)
        
    def _FindPassedVote(self):
        assert(self._CountResponses())
        highestBallot = Ballot.Min
        voteData = None
        foundOne = False
        for memberId in self.responseMap:
            response = self.responseMap[memberId]
            if response == None:
                continue
            if highestBallot < response.ballot:
                highestBallot = response.ballot
                voteData = response.voteData
                foundOne = True
        assert(foundOne)
        return (highestBallot, voteData)

    def VotePassed(self):
        return self.votePassed
    def GetPassedVote(self):
        assert(self.votePassed)
        return self.passedVote
        

# Optimize VoteResponse
# Optimize VoteResponseBroadcast

class PaxosInstance:
    def __init__(self, memberId, decree, replicaSet, stableStorage, callbacks):
        self.memberId = memberId
        self.decree = decree
        self.replicaSet = replicaSet
        self.callbacks = callbacks
        self.proposer = ProposerInstance(decree, replicaSet)
        self.acceptor = AcceptorInstance(memberId, decree, stableStorage)
        self.learner = LearnerInstance(decree, replicaSet)
        self.keepRunning = False

    def DispatchMessage(self, msg):
        if isinstance(msg, PrepareRequest): 
            resp = self.acceptor.HandlePrepare(msg)
            print(msg.ballot.memberId)
            env.Network().SendMessage(msg.ballot.memberId, resp)
        elif isinstance(msg, PrepareResponse):
            self.proposer.HandlePrepareResponse(msg)
            # check if ready to vote
        elif isinstance(msg, VoteRequest):
            resp = self.acceptor.HandleVote(msg)
            env.Network().SendMessage(msg.ballot.memberId, resp)
        elif isinstance(msg, VoteResponse):
            self.learner.HandleVoteResponse(msg)
            if self.learner.VotePassed():
                passedVote = self.learner.GetPassedVote()
                votePassedReq = VotePassedRequest(self.decree, passedVote[0], passedVote[1])
                for memberId in self.replicaSet:
                    env.Network().SendMessage(memberId, votePassedReq)
        elif isinstance(msg, VotePassedRequest):
            self.acceptor.HandleVotePassed(msg)
    
    def MainLoop(self):
        while self.keepRunning:
            msgs = env.Network().ReceiveMessages(self.memberId)
            for msg in msgs:
                self.DispatchMessage(msg)
            yield 1
    def Start(self):
        self.keepRunning = True
        env.StartThread(self.MainLoop(), 0)
    def Stop(self):
        self.keepRunning = False

    def Prepare(self, ballot):
        return self.proposer.Prepare(ballot)
    def IsReadyToVote(self):
        return self.proposer.ReadyToVote()
    def Vote(self, voteData):
        return self.proposer.Vote(voteData)
    def IsVotePassed(self):
        return self.acceptor.VotePassed()
    def GetPassedVote(self):
        return self.acceptor.GetPassedVote()
        
    def GetLastAcceptedVote(self):
        return self.proposer.GetLastAcceptedVote()


#================
def _Ballot_Test():
    b1 = Ballot.Min
    assert(not b1.IsValid())
    b2 = Ballot("qwe", 5)
    assert(b1 < b2)
    assert(b2 > b1)
    b3 = Ballot("qwe", 5)
    assert(b3 == b2)
    b4 = Ballot("qwe", 6)
    assert(b3 < b4)
    b5 = Ballot("asd", 6)
    assert(b4 > b5)
        
def _ProposerInstance_Test():
    p = ProposerInstance(100, Ballot("p", 5), ('a', 'b', 'c'))
    r = p.Prepare()
    # send r to acceptors
    assert(isinstance(r, PrepareRequest) and r.decree == 100)
    pr = PrepareResponse(100, Ballot("p", 5), "a", True, Ballot('c', 3), "Data")
    res = p.HandlePrepareResponse(pr)
    assert(res == False)
    pr = PrepareResponse(100, Ballot("p", 5), "a", True, Ballot.Min, None)
    res = p.HandlePrepareResponse(pr)
    assert(res == False)
    pr = PrepareResponse(100, Ballot("p", 5), "w", True, Ballot.Min, None)
    res = p.HandlePrepareResponse(pr)
    assert(res == False)
    pr = PrepareResponse(100, Ballot("p", 5), "b", True, Ballot.Min, None)
    res = p.HandlePrepareResponse(pr)
    assert(res == True)
    res = p.GetLastAcceptedVote()
    assert(res[0] == True and res[1] == "Data")
    res = p.Vote(res[1])
    assert(isinstance(res, VoteRequest) and res.decree == 100 and res.voteData == "Data")

def _AcceptorInstance_Test():
    ai = AcceptorInstance("a", 100)
    prepare = PrepareRequest(100, Ballot("p", 5))
    ret = ai.HandlePrepare(prepare)
    assert(ret.lastAcceptedBallot == Ballot.Min)
    vote = VoteRequest(100, Ballot("p", 5), "HelloWorld")
    ret = ai.HandleVote(vote)
    assert(ret == True)
    ret = ai.HandlePrepare(prepare)
    assert(ret.lastAcceptedBallot != Ballot.Min and ret.lastAcceptedVoteData == "HelloWorld")

if __name__ == "__main__":
    _Ballot_Test()
    _AcceptorInstance_Test()
    _ProposerInstance_Test()
    #_Proposer_Test()