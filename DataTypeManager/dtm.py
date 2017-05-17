class ModelTypeSet:
  def __init__(self, types=set()):
    self.types = set(types)
  def HasAll(self, types):
    if isinstance(types, ModelTypeSet):
      return types.types <= self.types
    return set(types) <= self.types

class DownloadManager:
  def __init__(self, delegate, priority_sets, types_to_enable):
    self.delegate = delegate
    self.priority_sets = priority_sets
    self.types_to_enable = types_to_enable
    self.priority_index = 0
    self.local_data_present = {}
  def SetLocalDataPresent(self, type, local_data_present):
    assert(type not in self.local_data_present)
    assert(type in self.types_to_enable)
    self.local_data_present[type] = local_data_present
    self.DownloadIfReady()
  def DownloadComplete(self, requested_types, succeeded_types):
    pass
  def ReadyToDownload(self, priority_index):
    types_to_enable = self.types_to_enable & self.priority_sets[priority_index]
    for t in types_to_enable:
      if t not in self.local_data_present:
        return False
    return True
  def GetTypesToDownload(self, priority_index):
    types_to_enable = self.types_to_enable & self.priority_sets[priority_index]
    types_to_download = set()
    for t in types_to_enable:
      assert(t in self.local_data_present)
      if not self.local_data_present[t]:
        types_to_download.add(t)
    return types_to_download

  def DownloadIfReady(self):
    while(True):
      if self.priority_index >= len(self.priority_sets):
        self.delegate.OnDownloadComplete()
        break
      if not self.ReadyToDownload(self.priority_index):
        break
      types_to_download = self.GetTypesToDownload(self.priority_index)
      if len(types_to_download) == 0:
        self.priority_index += 1
        continue
      self.delegate.StartDownload(types_to_download)

  def GetReadyTypes(self):
    priority_index = 0
    all_priority_types = set()
    for priority_set in self.priority_sets:
      all_priority_types |= self.priority_sets[priority_index]
      if priority_index == self.priority_index:
        break
    ready_types = set()
    for t in self.local_data_present:
      if not self.local_data_present[t]:
        continue
      if not t in all_priority_types:
        continue
      ready_types.add(t)
    return ready_types

class DataTypeManager:
  def __init__(self, controllers, delegate):
    self.controllers = controllers
    self.delegate = delegate
    self.state = "Ready"
  def Configure(self, desired_types, reason):
    assert(self.state == "Ready")
    self.state = "Configuring"
    self.delegate.OnConfigureStart()
    self.configuring_types = set()
    for t in self.controllers:
      dtc = self.controllers[t]
      self.configuring_types.add(t)
      should_be_running = t in desired_types
      if should_be_running:
        dtc.Start()
      else:
        dtc.Stop()
  def SingleTypeConfigurationDone(self, t):
    self.configuring_types.remove(t)
    if len(self.configuring_types) == 0:
      self.delegate.OnConfigureDone()

class DataTypeController:
  def Start(self):
    pass

class DirectoryDataTypeController:
  def Start(self):
    pass

class USSDataTypeCOntroller:
  def Start(self):
    pass
