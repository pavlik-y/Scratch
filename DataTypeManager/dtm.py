class ModelAssociationManager:
  def __init__(self, controllers, delegate):
    self.controllers_ = controllers
    self.delegate_ = delegate

  def Initialize(self, type_map):
    pass

class DataTypeManager:
  def __init__(self, controllers, encryption_handler, configurer, observer):
    self.controllers_ = controllers
    self.encryption_handler_ = encryption_handler
    self.configurer_ = configurer
    self.observer_ = observer
    self.mam_ = ModelAssociationManager(self.controllers_, self)

  def Configure(self, desired_types):
    self.mam_.Initialize(desired_types)
