#include <iosfwd>

class BitField {
 private:
  int _size;
  int _memSize;
  unsigned int* _mem;
 public:
  explicit BitField(int len = 0);
  BitField(const BitField& other);
  BitField(BitField&& other);
  ~BitField();

  BitField& operator=(const BitField& other);
  BitField& operator=(BitField&& other);

  int size() const { return _size; }
  void set(int n);
  void reset(int n);
  bool get(int n) const;

  bool operator==(const BitField& rhs) const;
  bool operator!=(const BitField& rhs) const { return !(*this == rhs); }

  BitField operator|(const BitField& rhs) const;
  BitField operator&(const BitField& rhs) const;
  BitField operator~() const;
  BitField& operator|=(const BitField& rhs);
  BitField& operator&=(const BitField& rhs);

  friend std::ostream& operator<<(std::ostream& os, const BitField& bf);
  friend std::istream& operator>>(std::istream& is, BitField& bf);
};
