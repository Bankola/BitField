#include "../Set/set.h"
#include <istream>
#include <ostream>
#include <utility>

Set::Set(int maxPower) : _maxPower(maxPower), _bf(maxPower) {}

Set::Set(const BitField& bf) : _maxPower(bf.size()), _bf(bf) {}

Set::Set(const Set& other) : _maxPower(other._maxPower), _bf(other._bf) {}

Set::Set(Set&& other)
    : _maxPower(other._maxPower), _bf(std::move(other._bf)) {
  other._maxPower = 0;
}

Set& Set::operator=(const Set& other) {
  _maxPower = other._maxPower;
  _bf = other._bf;
  return *this;
}

Set& Set::operator=(Set&& other) {
  if (this != &other) {
    _maxPower = other._maxPower;
    _bf = std::move(other._bf);
    other._maxPower = 0;
  }
  return *this;
}

void Set::insert(int elem) { _bf.set(elem); }

void Set::erase(int elem) { _bf.reset(elem); }

bool Set::is_member(int elem) const { return _bf.get(elem); }

Set Set::operator+(int elem) const {
  Set res(*this);
  res.insert(elem);
  return res;
}

Set Set::operator-(int elem) const {
  Set res(*this);
  res.erase(elem);
  return res;
}

Set& Set::operator+=(int elem) {
  insert(elem);
  return *this;
}

Set& Set::operator-=(int elem) {
  erase(elem);
  return *this;
}

Set Set::operator+(const Set& rhs) const { return Set(_bf | rhs._bf); }

Set Set::operator*(const Set& rhs) const { return Set(_bf & rhs._bf); }

Set Set::operator-(const Set& rhs) const {
  int len = _maxPower;
  if (rhs._maxPower > len) len = rhs._maxPower;
  BitField zero(len);
  return Set((_bf | zero) & ~(rhs._bf | zero));
}

Set& Set::operator+=(const Set& rhs) {
  *this = *this + rhs;
  return *this;
}

Set& Set::operator*=(const Set& rhs) {
  *this = *this * rhs;
  return *this;
}

Set& Set::operator-=(const Set& rhs) {
  *this = *this - rhs;
  return *this;
}

Set Set::operator-() const { return Set(~_bf); }

std::ostream& operator<<(std::ostream& os, const Set& s) {
  os << '{';
  bool first = true;
  for (int i = 0; i < s._maxPower; i++) {
    if (s.is_member(i)) {
      if (!first) os << ", ";
      os << i;
      first = false;
    }
  }
  os << '}';
  return os;
}

std::istream& operator>>(std::istream& is, Set& s) {
  char c;
  if (!(is >> c) || c != '{') {
    is.setstate(std::ios::failbit);
    return is;
  }
  Set tmp(s._maxPower);
  while (true) {
    if (!(is >> c)) return is;
    if (c == '}') break;
    if (c == ',') continue;
    is.putback(c);
    int v;
    if (!(is >> v) || v < 0 || v >= tmp._maxPower) {
      is.setstate(std::ios::failbit);
      return is;
    }
    tmp.insert(v);
  }
  s = std::move(tmp);
  return is;
}
