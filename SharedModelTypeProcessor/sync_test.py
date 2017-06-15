import logging
from message_queue import MessageQueue
from sync import *
import unittest

log = logging.getLogger()

class MockDatatype(Datatype):
  def __init__(self, message_queue):
    self.mq = message_queue
  def ConnectToSyncEngine(self, sync_engine):
    log.debug("MDT:Connect")
    sync_engine.ConnectDatatype(self.mq.CreateObjectProxy(self))
  def DisconnectFromSyncEngine(self, sync_engine):
    assert(self.connection)
    log.debug("MDT:Disconnect")
    sync_engine.DisconnectDatatype(self.connection)

class TestSyncEngine(unittest.TestCase):
  def setUp(self):
    self.mq = MessageQueue()
  def RunMQ(self):
    self.mq.Run()
  def Connect(self):
    self.server = Server()
    self.sync_engine = SyncEngine(self.mq, self.server)
    self.sync_engine_proxy = self.mq.CreateObjectProxy(self.sync_engine)
    self.datatype = MockDatatype(self.mq)
    self.datatype.ConnectToSyncEngine(self.sync_engine_proxy)
    self.RunMQ()

  def test_ConnectDisconnect(self):
    mq = MessageQueue()
    sync_engine = SyncEngine(mq, Server())
    sync_engine_proxy = mq.CreateObjectProxy(sync_engine)
    datatype = MockDatatype(mq)
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

if __name__ == '__main__':
  logging.basicConfig(level=logging.DEBUG)
  unittest.main()