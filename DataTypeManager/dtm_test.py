import unittest
from dtm import DataTypeManager

class MockDataTypeController:
  pass
class SlonTest(unittest.TestCase):
  def test_Slon(self):
    dtc = MockDataTypeController()
    controllers = {"DeviceInfo" : dtc}
    encryption_handler = None
    configurer = None
    observer = None
    dtm = DataTypeManager(controllers, encryption_handler, configurer, observer)

    types = set(["DeviceInfo"])
    dtm.Configure(types)
# {PAV}    dtc.ExpectLoadModelsCalled()




if __name__ == '__main__':
  unittest.main()