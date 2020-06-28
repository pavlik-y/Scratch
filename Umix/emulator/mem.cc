#include "emulator/mem.h"

void Mem::AllocZeroPage(uint32_t size) {
  if (pages_.empty())
    pages_.resize(1);
  ResetPage(0, size);
}

uint32_t Mem::AllocPage(uint32_t size) {
  uint32_t page;
  if (free_pages_.empty())
  {
    page = pages_.size();
    pages_.resize(page+1);
  } else {
    page = free_pages_.back();
    free_pages_.pop_back();
  }
  ResetPage(page, size);
  return page;
}

void Mem::FreePage(uint32_t page) {
  delete[] pages_[page].data;
  pages_[page].data = nullptr;
  free_pages_.push_back(page);
}

void Mem::CopyPageToZeroPage(uint32_t page) {
  ResetPage(0, pages_[page].size);
  uint32_t* src = pages_[page].data;
  uint32_t* dst = pages_[0].data;
  for (uint32_t offset = 0; offset < pages_[page].size; offset++)
    dst[offset] = src[offset];
}

uint32_t Mem::Read(uint32_t page, uint32_t offset) {
  return pages_[page].data[offset];
}

void Mem::Write(uint32_t page, uint32_t offset, uint32_t value) {
  pages_[page].data[offset] = value;
}

void Mem::ResetPage(uint32_t page, uint32_t size) {
  if (pages_[page].data)
    delete[] pages_[page].data;
  pages_[page].data = new uint32_t[size];
  pages_[page].size = size;
  for (uint32_t offset = 0; offset < size; offset++)
    pages_[page].data[offset] = 0;
}


