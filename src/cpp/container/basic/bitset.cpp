// %title
// 可変長bitset
//
// %overview
// 乱数で雑に性能評価したがあんまり速度差が無い(環境によって若干速い・遅い)
// vector<bool> がバグる環境用
//
// %require
// ```
#include <cstdint>
#include <memory>
using namespace std;
// ```
// %usage
//
// %verified
// resizeが出来ていない
//
// %references

class BitSet64 {
 public:
  using allocator_type = std::allocator<uint64_t>;

 private:
  allocator_type alloc_;
  int nBlock_;
  uint64_t* mem_;

 public:
  inline BitSet64(int _n = 1) : nBlock_((_n + 63) >> 6) {
    mem_ = allocator_traits<allocator_type>::allocate(alloc_, nBlock_);
  }
  inline BitSet64(const BitSet64& another) : nBlock_(another.nBlock_) {
    mem_ = allocator_traits<allocator_type>::allocate(alloc_, nBlock_);
    std::copy(another.mem_, another.mem_ + nBlock_, mem_);
  }
  inline BitSet64(BitSet64&& another)
      : alloc_(another.alloc_), nBlock_(another.nBlock_), mem_(another.mem_) {
    another.mem_ = nullptr;
  }
  ~BitSet64() {
    if (mem_ != nullptr) {
      allocator_traits<allocator_type>::deallocate(alloc_, mem_, nBlock_);
      mem_ = nullptr;
    }
  }
  inline bool operator[](uint64_t index) const { return (mem_[index >> 6] >> (index & 0x3f)) & 1; }
  inline void set(uint64_t index, bool v) {
    if (v)
      mem_[index >> 6] |= uint64_t(1) << (index & 0x3f);
    else
      mem_[index >> 6] &= ~(uint64_t(1) << (index & 0x3f));
  }
  inline void set(uint64_t index) { mem_[index >> 6] |= uint64_t(1) << (index & 0x3f); }
  inline void reset(uint64_t index) { mem_[index >> 6] &= ~(uint64_t(1) << (index & 0x3f)); }
  inline void fill(bool v) {
    std::fill(mem_, mem_ + nBlock_, v ? numeric_limits<uint64_t>::max() : (uint64_t)0);
  }
  inline void resize(int newSize) {
    newSize >>= 6;
    uint64_t* newmem_ = allocator_traits<allocator_type>::allocate(alloc_, nBlock_);
    std::copy(mem_, mem_ + min(newSize, nBlock_), newmem_);
    allocator_traits<allocator_type>::deallocate(alloc_, mem_, nBlock_);
    nBlock_ = newSize;
  }
};
