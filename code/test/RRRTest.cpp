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
  RRR rrr(v1, 4, 5);
  EXPECT_EQ(0, rrr.rank1(2));
  EXPECT_EQ(1, rrr.rank1(3));
  EXPECT_EQ(1, rrr.rank1(4));
  EXPECT_EQ(2, rrr.rank1(5));
}

TEST (RRRTest, Rank1EmptyConstructor) { 
  RRR rrr(v2);
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

TEST (RRRTest, RankDeepTest){
  RRR rrr(v2, 5, 3);
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

TEST (RRRTest, Select){
  RRR rrr(v2, 5, 3);

  // TODO Change to select1 when implemented
  EXPECT_EQ(4, rrr.select1(2));
  EXPECT_EQ(8, rrr.select1(4));
  EXPECT_EQ(15, rrr.select1(8));

  EXPECT_EQ(1, rrr.select0(2));
  EXPECT_EQ(5, rrr.select0(4));
  EXPECT_EQ(28, rrr.select0(18));
}

TEST (RRRTest, Access) {
  RRR rrr(v2, 5, 3);
	
  EXPECT_EQ(0, rrr.access(0));
  EXPECT_EQ(0, rrr.access(5));
  EXPECT_EQ(1, rrr.access(13));
  EXPECT_EQ(0, rrr.access(14));
  EXPECT_EQ(1, rrr.access(15));
  EXPECT_EQ(0, rrr.access(44));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
