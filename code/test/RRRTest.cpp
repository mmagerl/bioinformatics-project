#include <vector>

#include "../include/gtest/gtest.h"
#include "../include/RRR.h"

using namespace std;

TEST (RRRTest, Rank0) {
}

TEST (RRRTest, Rank1) { 
  vector<bool> v{false, false, false, true, false, true};
  RRR rrr(v, 4, 5);
  EXPECT_EQ(0, rrr.rank1(2));
  EXPECT_EQ(1, rrr.rank1(3));
  EXPECT_EQ(1, rrr.rank1(4));
  EXPECT_EQ(2, rrr.rank1(5));
}

TEST (RRRTest, Select) {
	
}

TEST (RRRTest, Access) {
	
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
