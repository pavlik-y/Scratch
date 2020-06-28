#ifndef MEM_H_
#define MEM_H_

#include <cstdint>
#include <vector>

class Mem {
 public:
  void AllocZeroPage(uint32_t size);
  uint32_t AllocPage(uint32_t size);
  void FreePage(uint32_t page);
  void CopyPageToZeroPage(uint32_t page);
  uint32_t Read(uint32_t page, uint32_t offset);
  void Write(uint32_t page, uint32_t offset, uint32_t value);

 private:
  struct PageDescriptor {
    uint32_t* data = nullptr;
    uint32_t size = 0;
  };

  void ResetPage(uint32_t page, uint32_t size);

  std::vector<PageDescriptor> pages_;
  std::vector<uint32_t> free_pages_;
};
#endif  // MEM_H_