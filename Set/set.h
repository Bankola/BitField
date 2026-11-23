#include <iosfwd>
#include "../BitField/bitfield.h"

class Set {
 private:
  int _maxPower;
  BitField _bf;

 public:
  explicit Set(int maxPower = 0);
  explicit Set(const BitField& bf);
  Set(const Set& other);
  Set(Set&& other);

  Set& operator=(const Set& other);
  Set& operator=(Set&& other);

  operator BitField() const { return _bf; }

  int max_power() const { return _maxPower; }
  void insert(int elem);
  void erase(int elem);
  bool is_member(int elem) const;

  bool operator==(const Set& rhs) const { return _bf == rhs._bf; }
  bool operator!=(const Set& rhs) const { return !(_bf == rhs._bf); }

  Set operator+(int elem) const;
  Set operator-(int elem) const;
  Set& operator+=(int elem);
  Set& operator-=(int elem);
  Set operator+(const Set& rhs) const;
  Set operator*(const Set& rhs) const;
  Set operator-(const Set& rhs) const;
  Set& operator+=(const Set& rhs);
  Set& operator*=(const Set& rhs);
  Set& operator-=(const Set& rhs);
  Set operator-() const;

  friend std::ostream& operator<<(std::ostream& os, const Set& s);
  friend std::istream& operator>>(std::istream& is, Set& s);
};