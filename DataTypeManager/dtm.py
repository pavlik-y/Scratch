class ModelTypeSet:
  def __init__(self, types=set()):
    self.types = set(types)
  def HasAll(self, types):
    if isinstance(types, ModelTypeSet):
      return types.types <= self.types
    return set(types) <= self.types

class DataTypeManager:
  def __init__(self, controllers, delegate):
    self.controllers = controllers
    self.delegate = delegate
    self.state = "Ready"
  def Configure(self, desired_types):
    assert(self.state == "Ready")
    self.state = "Configuring"
    self.delegate.OnConfigureStart()
    types_to_start, types_to_stop = self.GetTypeActions(desired_types)

    for type in types_to_start:
      self.controllers[type].Start(self.InitialSyncStateCallback)
    for type in types_to_stop:
      self.controllers[type].Stop()
  def InitialSyncStateCallback(self, type, initial_sync_done):
    pass

  def GetTypeActions(self, desired_types):
    types_to_start = set()
    types_to_stop = set()
    for type, controller in self.controllers.iteritems():
      if type in desired_types and controller.state() == "NotRunning":
        types_to_start.add(type)
      elif type not in desired_types and controller.state() == "Running":
        types_to_stop.add(type)
      else:
        assert(False, "type is in unexpected state")
    return (types_to_start, types_to_stop)


class DataTypeController:
  def Start(self):
    pass
