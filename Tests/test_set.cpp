#include "pch.h"
#include "../Set/set.h"

TEST(SetTest, ConstructorsAndBitFieldConversion) {
  Set empty;
  EXPECT_EQ(empty.max_power(), 0);

  Set s(10);
  EXPECT_EQ(s.max_power(), 10);
  for (int i = 0; i < 10; ++i) {
    EXPECT_FALSE(s.is_member(i));
  }

  BitField bf(6);
  bf.set(1);
  bf.set(4);

  Set from_bf(bf);
  EXPECT_EQ(from_bf.max_power(), 6);
  EXPECT_TRUE(from_bf.is_member(1));
  EXPECT_TRUE(from_bf.is_member(4));

  BitField back_to_bf = from_bf;
  EXPECT_EQ(back_to_bf, bf);
}

TEST(SetTest, NegativeMaxPowerThrows) {
  EXPECT_THROW(Set(-5), std::invalid_argument);
}

TEST(SetTest, InsertEraseAndIsMember) {
  Set s(15);
  s.insert(0);
  s.insert(7);
  s.insert(14);

  EXPECT_TRUE(s.is_member(0));
  EXPECT_TRUE(s.is_member(7));
  EXPECT_TRUE(s.is_member(14));
  EXPECT_FALSE(s.is_member(5));

  s.erase(7);
  EXPECT_FALSE(s.is_member(7));
  EXPECT_THROW(s.insert(-1), std::out_of_range);
  EXPECT_THROW(s.insert(15), std::out_of_range);
  EXPECT_THROW(s.erase(15), std::out_of_range);
  EXPECT_THROW(s.is_member(-1), std::out_of_range);
}

TEST(SetTest, CopyAndMoveSemantics) {
  Set a(8);
  a.insert(2);
  a.insert(5);

  Set b(a);
  EXPECT_EQ(a, b);
  b.erase(2);
  EXPECT_NE(a, b);
  EXPECT_TRUE(a.is_member(2));

  Set c(3);
  c = a;
  EXPECT_EQ(a, c);

  Set d(std::move(a));
  EXPECT_EQ(d.max_power(), 8);
  EXPECT_TRUE(d.is_member(2));
  EXPECT_TRUE(d.is_member(5));
  EXPECT_EQ(a.max_power(), 0);
}

TEST(SetTest, ElementOperatorsPlusAndMinus) {
  Set s(5);
  Set s1 = s + 1 + 3;
  EXPECT_TRUE(s1.is_member(1));
  EXPECT_TRUE(s1.is_member(3));
  EXPECT_FALSE(s.is_member(1));

  s1 += 4;
  EXPECT_TRUE(s1.is_member(4));

  Set s2 = s1 - 3;
  EXPECT_FALSE(s2.is_member(3));
  EXPECT_TRUE(s1.is_member(3));

  s2 -= 1;
  EXPECT_FALSE(s2.is_member(1));
  EXPECT_TRUE(s2.is_member(4));
}

TEST(SetTest, SetOperationsSamePower) {
  Set a(5);
  a += 0;
  a += 1;
  a += 2;

  Set b(5);
  b += 2;
  b += 3;
  b += 4;

  Set uni = a + b;
  for (int i = 0; i < 5; ++i) {
    EXPECT_TRUE(uni.is_member(i));
  }

  Set inter = a * b;
  EXPECT_FALSE(inter.is_member(0));
  EXPECT_FALSE(inter.is_member(1));
  EXPECT_TRUE(inter.is_member(2));
  EXPECT_FALSE(inter.is_member(3));

  Set diff = a - b;
  EXPECT_TRUE(diff.is_member(0));
  EXPECT_TRUE(diff.is_member(1));
  EXPECT_FALSE(diff.is_member(2));
  EXPECT_FALSE(diff.is_member(3));

  Set comp = -a;
  EXPECT_FALSE(comp.is_member(0));
  EXPECT_FALSE(comp.is_member(1));
  EXPECT_FALSE(comp.is_member(2));
  EXPECT_TRUE(comp.is_member(3));
  EXPECT_TRUE(comp.is_member(4));
}

TEST(SetTest, SetOperationsDifferentPower) {
  Set small(3);
  small += 0;
  small += 2;

  Set big(6);
  big += 2;
  big += 5;

  Set uni = small + big;
  EXPECT_EQ(uni.max_power(), 6);
  EXPECT_TRUE(uni.is_member(0));
  EXPECT_TRUE(uni.is_member(2));
  EXPECT_TRUE(uni.is_member(5));

  Set inter = small * big;
  EXPECT_EQ(inter.max_power(), 6);
  EXPECT_TRUE(inter.is_member(2));
  EXPECT_FALSE(inter.is_member(0));
  EXPECT_FALSE(inter.is_member(5));

  Set diff = small - big;
  EXPECT_EQ(diff.max_power(), 6);
  EXPECT_TRUE(diff.is_member(0));
  EXPECT_FALSE(diff.is_member(2));
  EXPECT_FALSE(diff.is_member(5));
}

TEST(SetTest, StreamOutputFormatting) {
  Set empty(5);
  std::ostringstream oss_empty;
  oss_empty << empty;
  EXPECT_EQ(oss_empty.str(), "{}");

  Set s(10);
  s += 1;
  s += 4;
  s += 9;
  std::ostringstream oss;
  oss << s;
  EXPECT_EQ(oss.str(), "{1, 4, 9}");
}

TEST(SetTest, StreamInputValidAndInvalid) {
  Set s(10);
  std::istringstream iss(" { 2 , 5 , 8 } ");
  iss >> s;
  EXPECT_FALSE(iss.fail());
  EXPECT_TRUE(s.is_member(2));
  EXPECT_TRUE(s.is_member(5));
  EXPECT_TRUE(s.is_member(8));
  EXPECT_FALSE(s.is_member(0));

  std::istringstream iss_empty("{}");
  iss_empty >> s;
  EXPECT_FALSE(iss_empty.fail());
  EXPECT_FALSE(s.is_member(2));

  s += 3;
  std::istringstream bad_elem("{3, 12}");
  bad_elem >> s;
  EXPECT_TRUE(bad_elem.fail());
  EXPECT_TRUE(s.is_member(3));

  std::istringstream bad_format("1, 2}");
  bad_format >> s;
  EXPECT_TRUE(bad_format.fail());
  EXPECT_TRUE(s.is_member(3));
}