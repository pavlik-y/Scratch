import unittest
from mock import Mock
from mock import call

from dtm import DataTypeManager
from dtm import DataTypeController
from dtm import ModelTypeSet
from dtm import DownloadManager

class DTMTest(unittest.TestCase):
  def setUp(self):
    self.af_dtc = Mock()
    self.bm_dtc = Mock()
    self.controllers = {
        "bm": self.bm_dtc,
        "af": self.af_dtc}
    self.pss = Mock()

  def test_EnableOneType(self):
    # self.af_dtc.state.return_value = "NotRunning"
    # self.bm_dtc.state.return_value = "Running"
    # dtm = DataTypeManager(self.controllers, self.pss)
    # dtm.Configure(set(["af"]), "reconfiguration")
    # self.pss.OnConfigureStart.assert_called_once_with()
    # configuration_done_callback = self.af_dtc.Configure.call_args[0][1]
    # configuration_done_callback("af")
    # configuration_done_callback = self.bm_dtc.Configure.call_args[0][1]
    # configuration_done_callback("bm")
    # self.pss.OnConfigureDone.assert_called_once_with()
    pass
class SlonDTM:
  def Configure(self, types_to_enable):
    self.dm = DownloadManager(self, self.priority_sets, types_to_enable)
    ready_types = self.dm.GetReadyTypes()
    self.StartAssociation(ready_types)
  def StartDownload(self, types_to_download):
    pass
  def OnDownloadComplete(self):
    pass
  def DownloadComplete(self, requested_types, succeeded_types):
    self.dm.DownloadComplete(requested_types, succeeded_types)
    ready_types = self.dm.GetReadyTypes()
    self.StartAssociation(ready_types)

class DTCTest(unittest.TestCase):
  def test_EnableType_HappyCase(self):
    dtc = DataTypeController()
    # dtc.Configure(True, None)

class ModelTypeSetTest(unittest.TestCase):
  def test_Slon(self):
    mts1 = ModelTypeSet()
    mts2 = ModelTypeSet(["bm"])
    self.assertTrue(mts2.HasAll(mts1))
    self.assertTrue(mts2.HasAll(['bm']))


class DownloadManagerTest(unittest.TestCase):
  def setUp(self):
    self.priority_sets = []
    self.priority_sets.append(set(["ng", "ex"]))
    self.priority_sets.append(set(["di", "af"]))
    self.priority_sets.append(set(["bm"]))

  def test_ReadyTypes(self):
    dm = DownloadManager(None, self.priority_sets, set(["di", "bm"]))
    ready_types = dm.GetReadyTypes()
    print(ready_types)
    dm.SetLocalDataPresent("di", True)
    ready_types = dm.GetReadyTypes()
    print(ready_types)
  def test_ReadyToDownload(self):
    dm = DownloadManager(None, self.priority_sets, set(["di", "af", "bm"]))
    ready_types = dm.GetReadyTypes()
    self.assertTrue(dm.ReadyToDownload(0))
    self.assertFalse(dm.ReadyToDownload(1))
    dm.SetLocalDataPresent("di", True)
    self.assertFalse(dm.ReadyToDownload(1))
    dm.SetLocalDataPresent("af", False)
    self.assertTrue(dm.ReadyToDownload(1))
    self.assertTrue(dm.GetTypesToDownload(1) == set(["af"]))





if __name__ == '__main__':
  unittest.main()