#include "bitfield.h"
#include <ostream>
#include <utility>
#include <stdexcept>
#include <istream>
const int BITS = sizeof(unsigned int) * 8;

BitField::BitField(int len) {
  if (len < 0) {
    throw std::invalid_argument("Error");
  }
  _size = len;
  _memSize = (len + BITS - 1) / BITS;
  _mem = new unsigned int[_memSize];
  for (int i = 0; i < _memSize; i++) {
    _mem[i] = 0;
  }
}

BitField::BitField(const BitField& other) {
  _size = other._size;
  _memSize = other._memSize;
  _mem = new unsigned int[_memSize];
  for (int i = 0; i < _memSize; i++) {
    _mem[i] = other._mem[i];
  }
}

BitField::BitField(BitField&& other) {
  _size = other._size;
  _memSize = other._memSize;
  _mem = other._mem;
  other._size = 0;
  other._memSize = 0;
  other._mem = nullptr;
}

BitField::~BitField() { delete[] _mem; }

BitField& BitField::operator=(const BitField& other) {
  if (this == &other) return *this;
  unsigned int* newMem = new unsigned int[other._memSize];
  for (int i = 0; i < other._memSize; i++) {
    newMem[i] = other._mem[i];
  }
  delete[] _mem;
  _mem = newMem;
  _size = other._size;
  _memSize = other._memSize;
  return *this;
}

BitField& BitField::operator=(BitField&& other) {
  if (this == &other) return *this;
  delete[] _mem;
  _size = other._size;
  _memSize = other._memSize;
  _mem = other._mem;
  other._size = 0;
  other._memSize = 0;
  other._mem = nullptr;
  return *this;
}

void BitField::set(int n) {
  if (n < 0 || n >= _size) throw std::out_of_range("Error");
  _mem[n / BITS] |= 1u << (n % BITS);
}

void BitField::reset(int n) {
  if (n < 0 || n >= _size) throw std::out_of_range("Error");
  _mem[n / BITS] &= ~(1u << (n % BITS));
}

bool BitField::get(int n) const {
  if (n < 0 || n >= _size) throw std::out_of_range("Error");
  return (_mem[n / BITS] >> (n % BITS)) & 1u;
}

bool BitField::operator==(const BitField& rhs) const {
  if (_size != rhs._size) return false;
  for (int i = 0; i < _memSize; i++) {
    if (_mem[i] != rhs._mem[i]) return false;
  }
  return true;
}

BitField BitField::operator|(const BitField& rhs) const {
  int len = _size;
  if (rhs._size > len) len = rhs._size;
  BitField res(len);
  for (int i = 0; i < res._memSize; i++) {
    unsigned int a = 0;
    unsigned int b = 0;
    if (i < _memSize) a = _mem[i];
    if (i < rhs._memSize) b = rhs._mem[i];
    res._mem[i] = a | b;
  }
  return res;
}

BitField BitField::operator&(const BitField& rhs) const {
  int len = _size;
  if (rhs._size > len) len = rhs._size;
  BitField res(len);
  for (int i = 0; i < res._memSize; i++) {
    unsigned int a = 0;
    unsigned int b = 0;
    if (i < _memSize) a = _mem[i];
    if (i < rhs._memSize) b = rhs._mem[i];
    res._mem[i] = a & b;
  }
  return res;
}

BitField BitField::operator~() const {
  BitField res(_size);
  for (int i = 0; i < _size; i++) {
    if (!get(i)) res.set(i);
  }
  return res;
}

BitField& BitField::operator|=(const BitField& rhs) {
  *this = *this | rhs;
  return *this;
}

BitField& BitField::operator&=(const BitField& rhs) {
  *this = *this & rhs;
  return *this;
}

std::ostream& operator<<(std::ostream& os, const BitField& bf) {
  for (int i = 0; i < bf._size; i++) {
    if (bf.get(i)) {
      os << '1';
    } else {
      os << '0';
    }
  }
  return os;
}

std::istream& operator>>(std::istream& is, BitField& bf) { 
  BitField tmp(bf._size);
  for (int i = 0; i < bf._size; i++) {
    char c;
    if (!(is >> c) || (c != '0' && c != '1')) {
      is.setstate(std::ios::failbit);
      return is;
    }
    if (c == '1') tmp.set(i);
  }
  bf = tmp;
  return is;
}