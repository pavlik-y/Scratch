import logging
from message_queue import MessageQueue
from sync import *
from sync_server import *
import unittest

log = logging.getLogger()
logging.basicConfig(level=logging.DEBUG)

class MockDatatype(Datatype):
  def __init__(self, message_queue, model_type):
    self.mq = message_queue
    self.model_type = model_type
  def ConnectToSyncEngine(self, sync_engine):
    log.debug("MDT:Connect")
    sync_engine.ConnectDatatype(self.mq.CreateObjectProxy(self),
                                self.model_type,
                                0)
  def DisconnectFromSyncEngine(self, sync_engine):
    assert(self.connection)
    log.debug("MDT:Disconnect")
    sync_engine.DisconnectDatatype(self.connection, self.model_type)

class Test_SyncEngine_Server(unittest.TestCase):
  def setUp(self):
    self.mq = MessageQueue()
  def RunMQ(self):
    self.mq.Run()
  def PopulateServer(self):
    self.server = Server()
    request = Struct(entities=[Struct(id="c:1", type="bm"),
                               Struct(id="c:2", type="bm"),
                               Struct(id="c:3", type="af")])
    self.server.Commit(request)

  def Connect(self):
    self.PopulateServer()
    self.sync_engine = SyncEngine(self.mq, self.server)
    self.sync_engine_proxy = self.mq.CreateObjectProxy(self.sync_engine)
    self.datatype = MockDatatype(self.mq, "bm")
    self.datatype.ConnectToSyncEngine(self.sync_engine_proxy)
    self.RunMQ()

  def test_ConnectDisconnect(self):
    mq = MessageQueue()
    sync_engine = SyncEngine(mq, Server())
    sync_engine_proxy = mq.CreateObjectProxy(sync_engine)
    datatype = MockDatatype(mq, "bm")
    datatype.ConnectToSyncEngine(sync_engine_proxy)
    log.debug("T:After connect")
    mq.Run()
    log.debug("T:Run")
    datatype.DisconnectFromSyncEngine(sync_engine_proxy)
    log.debug("T:After disconnect")
    mq.Run()
  def test_SimpleSyncCycle(self):
    self.Connect()
    self.sync_engine_proxy.Nudge()
    self.RunMQ()

class TestServer(unittest.TestCase):
  def test_Commit(self):
    server = Server()
    request = Struct(entities=[])
    request.entities.append(Struct(id="c:1", type="bm"))
    response = server.Commit(request)
    self.assertEqual(response.entities[0].version, 1)
    response = server.Commit(request)
    self.assertEqual(response.entities[0].version, 2)
  def PopulateServer(self, server):
    request = Struct(entities=[Struct(id="c:1", type="bm"),
                               Struct(id="c:2", type="bm"),
                               Struct(id="c:3", type="af")])
    server.Commit(request)
  def test_GetUpdates(self):
    server = Server()
    self.PopulateServer(server)
    request = Struct(progress_markers={"bm":0})
    response = server.GetUpdates(request)
    request = Struct(progress_markers={"bm":1})
    response = server.GetUpdates(request)
    request = Struct(progress_markers={"bm":3, "af":0})
    response = server.GetUpdates(request)

  def test_AsyncGU(self):
    server = Server()
    mq = MessageQueue()
    server_proxy = mq.CreateObjectProxy(server)
    request_tracker = Struct(request=Struct(progress_markers={"bm":0}),
                             response=None)
    def GU():
      request_tracker.response = server.GetUpdates(request_tracker.request, None)
    mq.Post(GU)
    mq.RunOne()
    log.debug(request_tracker.response)



class TestStruct(unittest.TestCase):
  def test_Copy(self):
    a = Struct(x=1)
    b = a.copy()
    b.x = 2
    self.assertEqual(1, a.x)
    self.assertEqual(2, b.x)

if __name__ == '__main__':
  unittest.main()