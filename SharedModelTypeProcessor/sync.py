# Engine/Datatype interaction diagram:
# http://www.plantuml.com/plantuml/uml/XPBF2eCm3CRlFeN7UfWFiCDGLBRJ1ewTHsc356hiR3lmxQVMlunkfwR9BryQlYRQqDROMWOvDTHq2iB3ARnto0gi1C8np1f4O8P4mPeTc9bV2E24kKWq60Ln7EyqiwLs3A-MLv0rTIq6tRMorxrGv18UYbC3cbXtHbxH2NHTdBWN-BHJAIU-10RYFJRub5SuLKgAl-9dC6wv8KNGBrpWouA53Sp6yDaFexzdtD1rbQ1LWnhcpdQwVnkmDJKNUl9gvvdFuHKVTqy0UVzbFW00

import logging

log = logging.getLogger(__name__)

class SyncEngine:
  def __init__(self, message_queue, server):
    self.mq = message_queue
    self.server = server
    self.datatype_handlers = {}
  # SyncEngine interface for datatypes

  # Connects datatype to sync engine. Returns connection object.
  def ConnectDatatype(self, datatype_handler):
    log.debug("SE:Connect")
    datatype_connection = self.mq.CreateObjectProxy(
        DatatypeConnection(self, datatype_handler))
    self.datatype_handlers[datatype_connection] = datatype_handler
    datatype_handler.DatatypeConnected(datatype_connection)
  def DisconnectDatatype(self, datatype_connection):
    log.debug("SE:Disconnect")
    datatype_handler = self.datatype_handlers[datatype_connection]
    assert(datatype_handler)
    del(self.datatype_handlers[datatype_connection])
    datatype_handler.DatatypeDisconnected()
  def Nudge(self):
    log.debug("SE:Nudge")
    self.RunSyncCycle()
  def RunSyncCycle(self):
    self.DownloadAndApplyUpdates()
    self.CommitLocalChanges()
  def DownloadAndApplyUpdates(self):
    log.debug("SE:Download")
    self.server.GetUpdates()
  def CommitLocalChanges(self):
    log.debug("SE:Commit")
    self.server.Commit()

class DatatypeConnection:
  def __init__(self, sync_engine, datatype_handler):
    self.sync_engine = sync_engine
    self.handler = datatype_handler

class Datatype:
  def __init__(self):
    self.connection = None
  def DatatypeConnected(self, sync_connection):
    log.debug("DT:DatatypeConnected")
    self.connection = sync_connection
  def DatatypeDisconnected(self):
    log.debug("DT:DatatypeDisconnected")
    self.connection = None

class Server:
  def GetUpdates(self):
    log.debug("SS:GetUpdates")
  def Commit(self):
    log.debug("SS:Commit")