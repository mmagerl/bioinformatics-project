#include <vector>

#include "../include/gtest/gtest.h"
#include "../include/RRR.h"

using namespace std;

vector<bool> v1{false, false, false, true, false, true};
vector<bool> v2{
    false, false, true, false, true,
    false, false, true, true, false,
    true, false, true, true, false,

    true, false, false, false, false,
    false, true, false, false, true,
    false, true, false, false, true,

    true, false, true, true, false,
    false, false, true, false, true,
    true, false, true, true, false
};

TEST (RRRTest, Rank1) { 
  RRR rrr(v1);
  EXPECT_EQ(0, rrr.rank1(2));
  EXPECT_EQ(1, rrr.rank1(3));
  EXPECT_EQ(1, rrr.rank1(4));
  EXPECT_EQ(2, rrr.rank1(5));
}

void testRank(RRR& rrr) {
  // S0, B0
  EXPECT_EQ(0, rrr.rank1(1));
  EXPECT_EQ(1, rrr.rank1(2));
  EXPECT_EQ(2, rrr.rank0(1));
  EXPECT_EQ(2, rrr.rank0(2));

  // S0, B1
  EXPECT_EQ(2, rrr.rank1(6));
  EXPECT_EQ(3, rrr.rank1(7));
  EXPECT_EQ(4, rrr.rank0(5));
  EXPECT_EQ(5, rrr.rank0(6));
  EXPECT_EQ(5, rrr.rank0(7));

  // S1, B0, B1
  EXPECT_EQ(8, rrr.rank1(15));
  EXPECT_EQ(9, rrr.rank1(21));

  EXPECT_EQ(11, rrr.rank0(18));
  EXPECT_EQ(15, rrr.rank0(23));

  // S2, B0, B2
  EXPECT_EQ(13, rrr.rank1(30));
  EXPECT_EQ(15, rrr.rank1(34));
  EXPECT_EQ(20, rrr.rank1(44));
  EXPECT_EQ(18, rrr.rank0(30));
  EXPECT_EQ(20, rrr.rank0(34));
  EXPECT_EQ(25, rrr.rank0(44));
}

TEST (RRRTest, RankTest) {
  RRR rrr1(v2, 5, 3);
  testRank(rrr1);
  RRR rrr2(v2);
  testRank(rrr2);
}

void testSelect(RRR& rrr) {
  EXPECT_EQ(4, rrr.select1(2));
  EXPECT_EQ(8, rrr.select1(4));
  EXPECT_EQ(15, rrr.select1(8));
  EXPECT_EQ(37, rrr.select1(16));

  EXPECT_EQ(1, rrr.select0(2));
  EXPECT_EQ(5, rrr.select0(4));
  EXPECT_EQ(28, rrr.select0(18));
  EXPECT_EQ(44, rrr.select0(25));
}

TEST (RRRTest, Select) {
  RRR rrr1(v2, 5, 3);
  testSelect(rrr1);
  RRR rrr2(v2);
  testSelect(rrr2);
}

void testAccess(RRR& rrr) {
  EXPECT_EQ(0, rrr.access(0));
  EXPECT_EQ(0, rrr.access(5));
  EXPECT_EQ(1, rrr.access(13));
  EXPECT_EQ(0, rrr.access(14));
  EXPECT_EQ(1, rrr.access(15));
  EXPECT_EQ(0, rrr.access(44));
}

TEST (RRRTest, Access) {
  RRR rrr1(v2, 5, 3);
  testAccess(rrr1);
  RRR rrr2(v2);
  testAccess(rrr2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
