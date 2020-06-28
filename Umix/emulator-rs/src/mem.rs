pub struct Mem {
  pages:Vec<Vec<u32>>,
  free_pages:Vec<usize>,
}

impl Mem {
  pub fn new() -> Mem {
    Mem {
      pages:vec![],
      free_pages:vec![]
    }
  }

  pub fn alloc_zero_page(&mut self, size:u32) {
    assert!(self.pages.is_empty());
    let page = self.alloc_page(size);
    assert!(page == 0);
  }

  pub fn alloc_page(&mut self, size:u32) -> u32 {
    if self.free_pages.is_empty() {
      self.pages.push(vec![]);
      self.free_pages.push(self.pages.len() - 1);
    }
    let page = self.free_pages.pop().unwrap();
    self.pages[page].resize(size as usize, 0);
    return page as u32;
  }

  pub fn free_page(&mut self, page:u32) {
    let page = page as usize;
    self.pages[page].clear();
    self.pages[page].shrink_to_fit();
    self.free_pages.push(page);
  }

  pub fn copy_page_to_zero_page(&mut self, page:u32) {
    assert!(!self.pages.is_empty());
    let page = self.pages[page as usize].to_vec();
    self.pages[0] = page;

  }

  pub fn read(&self, page:u32, offset:u32) -> u32 {
    return self.pages[page as usize][offset as usize];
  }

  pub fn write(&mut self, page:u32, offset:u32, value:u32) {
    self.pages[page as usize][offset as usize] = value;
  }
}