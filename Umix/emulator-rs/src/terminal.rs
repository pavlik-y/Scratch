pub trait Terminal {
  fn read(&self) -> u32;
  fn write(&self, value: u32);
}