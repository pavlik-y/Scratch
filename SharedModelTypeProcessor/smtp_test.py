import unittest

from smtp import *

class TestInstantiation(unittest.TestCase):
  def test_instantiation(self):
    processor = Processor()
    self.assertTrue(True)

if __name__ == '__main__':
  unittest.main()