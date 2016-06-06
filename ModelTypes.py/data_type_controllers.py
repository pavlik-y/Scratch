class ModelType(object):
  DEVICE_INFO = "DEVICE_INFO"


class DataTypeController(object):
  # State constants
  NOT_RUNNING = "NOT_RUNNING"
  MODEL_STARTING = "MODEL_STARTING"

  def __init__(self, model_type):
    self.model_type = model_type
    self.state = self.NOT_RUNNING


class NonBlockingDataTypeController(DataTypeController):
  def __init__(self, model_type, ui_task_runner, sync_client):
    super(NonBlockingDataTypeController, self).__init__(model_type)
    self._ui_task_runner = ui_task_runner
    self._sync_client = sync_client

  def RunOnUIThread(self, task):
    self._ui_task_runner.PostTask(task)

  def LoadModels(self, model_load_callback):
    assert(self.state == self.NOT_RUNNING)
    self._model_load_callback = model_load_callback
    self.state = self.MODEL_STARTING
    self.RunOnModelThread(self.LoadModelsOnModelThread)

  def LoadModelsOnModelThread(self):
    assert(self.state == self.MODEL_STARTING)
    service = self._sync_client.GetModelTypeServiceForType(self.model_type)

