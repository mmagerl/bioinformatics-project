#include <vector>

#include "../include/gtest/gtest.h"
#include "../include/RRR.h"

using namespace std;

TEST (RRRTest, Rank0) {
    
}

TEST (RRRTest, Rank1) { 
	
}

TEST (RRRTest, Select) {
	
}

TEST (RRRTest, Access) {
	
}

int main(int argc, char **argv) {
  vector<bool> v(5, false);
  RRR rrr(v, 2, 5);

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
