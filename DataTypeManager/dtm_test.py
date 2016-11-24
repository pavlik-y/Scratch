import unittest
from mock import Mock
from mock import call

from dtm import DataTypeManager

class DTMTest(unittest.TestCase):
  def setUp(self):
    self.af_dtc = Mock()
    self.bm_dtc = Mock()
    self.controllers = {
        "bm": self.bm_dtc,
        "af": self.af_dtc}
    self.pss = Mock()

  def test_EnableOneType(self):
    self.af_dtc.state.return_value = "NotRunning"
    self.bm_dtc.state.return_value = "Running"
    dtm = DataTypeManager(self.controllers, self.pss)
    dtm.Configure(set(["af"]))
    self.pss.OnConfigureStart.assert_called_once_with()
    initial_sync_state_callback = self.af_dtc.Start.call_args[0][0]
    self.bm_dtc.Stop.assert_called_once_with()
    initial_sync_state_callback("bm", False)


if __name__ == '__main__':
  unittest.main()