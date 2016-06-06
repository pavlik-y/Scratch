import unittest

import data_type_controllers as dtcs
import environment as env


class DataTypeControllerTest(unittest.TestCase):
  def test_InitialState(self):
    dtc = dtcs.DataTypeController(dtcs.ModelType.DEVICE_INFO)
    self.assertEqual(dtc.state, dtc.NOT_RUNNING)


class TestDataTypeController(dtcs.NonBlockingDataTypeController):
  def __init__(self, task_runner, sync_client):
    super(TestDataTypeController, self).__init__(dtcs.ModelType.DEVICE_INFO, task_runner, sync_client)
    self._model_task_runner = task_runner

  def RunOnModelThread(self, task):
    self._model_task_runner.PostTask(task)

class MockSyncClient(object):
  def GetModelTypeServiceForType(self, model_type):
    pass

class NonBlockingdataTypeControllerTest(unittest.TestCase):
  def test_StateTransitions(self):
    message_loop = env.MessageLoop()
    dtc = TestDataTypeController(message_loop, MockSyncClient())
    model_load_callback = lambda type, error: self.ModelLoadCallback(type, error)
    dtc.LoadModels(model_load_callback)
    self.assertEquals(dtc.state, dtc.MODEL_STARTING)
    message_loop.RunUntilIdle()

if __name__ == '__main__':
  unittest.main()