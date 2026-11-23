#include "pch.h"
#include "../BitField/bitfield.h"

TEST(BitFieldTest, DefaultAndZeroLength) {
  BitField bf;
  EXPECT_EQ(bf.size(), 0);
  EXPECT_THROW(bf.get(0), std::out_of_range);
  EXPECT_THROW(bf.set(0), std::out_of_range);
}

TEST(BitFieldTest, NegativeLengthThrows) {
  EXPECT_THROW(BitField(-1), std::invalid_argument);
}

TEST(BitFieldTest, SetResetAndGetAcrossWordBoundary) {
  BitField bf(40);
  EXPECT_EQ(bf.size(), 40);

  for (int i = 0; i < 40; ++i) {
    EXPECT_FALSE(bf.get(i));
  }

  bf.set(0);
  bf.set(31);
  bf.set(32);
  bf.set(39);

  EXPECT_TRUE(bf.get(0));
  EXPECT_TRUE(bf.get(31));
  EXPECT_TRUE(bf.get(32));
  EXPECT_TRUE(bf.get(39));
  EXPECT_FALSE(bf.get(15));

  bf.reset(31);
  EXPECT_FALSE(bf.get(31));
  EXPECT_TRUE(bf.get(32));
}

TEST(BitFieldTest, OutOfRangeThrows) {
  BitField bf(10);
  EXPECT_THROW(bf.set(-1), std::out_of_range);
  EXPECT_THROW(bf.set(10), std::out_of_range);
  EXPECT_THROW(bf.reset(-1), std::out_of_range);
  EXPECT_THROW(bf.reset(10), std::out_of_range);
  EXPECT_THROW(bf.get(-1), std::out_of_range);
  EXPECT_THROW(bf.get(10), std::out_of_range);
}

TEST(BitFieldTest, CopyAndMoveSemantics) {
  BitField a(10);
  a.set(2);
  a.set(7);

  BitField b(a);
  EXPECT_EQ(a, b);
  b.reset(2);
  EXPECT_NE(a, b);
  EXPECT_TRUE(a.get(2));

  BitField c(3);
  c = a;
  EXPECT_EQ(a, c);

  BitField d(std::move(a));
  EXPECT_EQ(d.size(), 10);
  EXPECT_TRUE(d.get(2));
  EXPECT_TRUE(d.get(7));
  EXPECT_EQ(a.size(), 0);
}

TEST(BitFieldTest, BitwiseOperationsSameLength) {
  BitField a(4);
  a.set(0);
  a.set(2);

  BitField b(4);
  b.set(0);
  b.set(1);

  BitField or_res = a | b;
  BitField and_res = a & b;
  BitField not_res = ~a;

  EXPECT_TRUE(or_res.get(0));
  EXPECT_TRUE(or_res.get(1));
  EXPECT_TRUE(or_res.get(2));
  EXPECT_FALSE(or_res.get(3));

  EXPECT_TRUE(and_res.get(0));
  EXPECT_FALSE(and_res.get(1));
  EXPECT_FALSE(and_res.get(2));
  EXPECT_FALSE(and_res.get(3));

  EXPECT_FALSE(not_res.get(0));
  EXPECT_TRUE(not_res.get(1));
  EXPECT_FALSE(not_res.get(2));
  EXPECT_TRUE(not_res.get(3));
}

TEST(BitFieldTest, BitwiseOperationsDifferentLength) {
  BitField short_bf(3);
  short_bf.set(0);
  short_bf.set(2);

  BitField long_bf(5);
  long_bf.set(4);

  BitField res = short_bf | long_bf;
  EXPECT_EQ(res.size(), 5);
  EXPECT_TRUE(res.get(0));
  EXPECT_FALSE(res.get(1));
  EXPECT_TRUE(res.get(2));
  EXPECT_FALSE(res.get(3));
  EXPECT_TRUE(res.get(4));

  short_bf |= long_bf;
  EXPECT_EQ(short_bf, res);
}

TEST(BitFieldTest, StreamOutputAndInput) {
  BitField bf(5);
  bf.set(0);
  bf.set(3);

  std::ostringstream oss;
  oss << bf;
  EXPECT_EQ(oss.str(), "10010");

  BitField parsed(5);
  std::istringstream iss("  0 1 1\n0 1 ");
  iss >> parsed;
  EXPECT_FALSE(iss.fail());
  EXPECT_FALSE(parsed.get(0));
  EXPECT_TRUE(parsed.get(1));
  EXPECT_TRUE(parsed.get(2));
  EXPECT_FALSE(parsed.get(3));
  EXPECT_TRUE(parsed.get(4));
}

TEST(BitFieldTest, StreamInputFailurePreservesState) {
  BitField bf(4);
  bf.set(1);

  std::istringstream bad_iss("1021");
  bad_iss >> bf;

  EXPECT_TRUE(bad_iss.fail());
  EXPECT_FALSE(bf.get(0));
  EXPECT_TRUE(bf.get(1));
  EXPECT_FALSE(bf.get(2));
  EXPECT_FALSE(bf.get(3));
}