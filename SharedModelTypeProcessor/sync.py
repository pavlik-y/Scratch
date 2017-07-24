# Engine/Datatype interaction diagram:
# http://www.plantuml.com/plantuml/uml/XPBF2eCm3CRlFeN7UfWFiCDGLBRJ1ewTHsc356hiR3lmxQVMlunkfwR9BryQlYRQqDROMWOvDTHq2iB3ARnto0gi1C8np1f4O8P4mPeTc9bV2E24kKWq60Ln7EyqiwLs3A-MLv0rTIq6tRMorxrGv18UYbC3cbXtHbxH2NHTdBWN-BHJAIU-10RYFJRub5SuLKgAl-9dC6wv8KNGBrpWouA53Sp6yDaFexzdtD1rbQ1LWnhcpdQwVnkmDJKNUl9gvvdFuHKVTqy0UVzbFW00

import logging
from sync_server import Struct

log = logging.getLogger(__name__)

class SyncEngine:
  def __init__(self, message_queue, server):
    self.mq = message_queue
    self.server = server
    self.datatype_handlers = {}
    self.progress_markers = {}
  # SyncEngine interface for datatypes

  # Connects datatype to sync engine. Returns connection object.
  def ConnectDatatype(self, datatype_handler, model_type, progress_marker):
    log.debug("SE:Connect")
    datatype_connection = self.mq.CreateObjectProxy(
        DatatypeConnection(self, datatype_handler))
    self.datatype_handlers[datatype_connection] = datatype_handler
    self.progress_markers[model_type] = progress_marker
    datatype_handler.DatatypeConnected(datatype_connection)
  def DisconnectDatatype(self, datatype_connection, model_type):
    log.debug("SE:Disconnect")
    datatype_handler = self.datatype_handlers[datatype_connection]
    assert(datatype_handler)
    del(self.datatype_handlers[datatype_connection])
    del(self.progress_markers[model_type])
    datatype_handler.DatatypeDisconnected()
  def Nudge(self):
    log.debug("SE:Nudge")
    self.RunSyncCycle()
  def RunSyncCycle(self):
    self.DownloadAndApplyUpdates()
    self.CommitLocalChanges()
  def DownloadAndApplyUpdates(self):
    log.debug("SE:Download")
    request = Struct(progress_markers=self.progress_markers)
    response = self.server.GetUpdates(request)
    print(response)
  def CommitLocalChanges(self):
    log.debug("SE:Commit")
    request = Struct(entities=[])
    self.server.Commit(request)

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

class Bridge:
  def ReadMetadata(self, storage_key):
    pass
  def ReadData(self, storage_key):
    pass
  def ReadDataTypeState(self):
    pass
  def GetStorageKey(self, emntity_data):
    pass

class Processor:
  def __init__(self, bridge):
    self.bridge = bridge
    self.metadata_cache = {}
    self.commit_items_cache = {}
    self.client_tag_to_storage_key = {}
    self.model_type_state = None

class ProcessorForUpdates(Processor):
  def HandleUpdates(self, updates):
    # Ensure all data loaded
    self.EnsureAllDataLoaded(updates)




