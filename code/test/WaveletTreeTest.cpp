#include "../include/gtest/gtest.h"
#include "../include/WaveletTree.h"

using namespace std;

string str = "Bioinformatika je divan kolegij";
WaveletTree waveletTree(str);

TEST (WaveletTreeTest, Rank) {
	EXPECT_EQ(0, waveletTree.rank(5, 'd'));
	EXPECT_EQ(1, waveletTree.rank(0, 'B'));
	EXPECT_EQ(0, waveletTree.rank(0, 'z'));
	EXPECT_EQ(1, waveletTree.rank(1, 'i'));
	EXPECT_EQ(1, waveletTree.rank(2, 'i'));
	EXPECT_EQ(2, waveletTree.rank(3, 'i'));
	EXPECT_EQ(4, waveletTree.rank(19, 'i'));
	EXPECT_EQ(0, waveletTree.rank(9, 't'));
	EXPECT_EQ(1, waveletTree.rank(10,'t'));
}

TEST (WaveletTreeTest, Select){
	EXPECT_EQ(0, waveletTree.select(0, 'B'));
	EXPECT_EQ(1, waveletTree.select(0, 'i'));
	EXPECT_EQ(10, waveletTree.select(0, 't'));
	EXPECT_EQ(3, waveletTree.select(1, 'i'));
	EXPECT_EQ(11, waveletTree.select(2, 'i'));
	EXPECT_EQ(2, waveletTree.select(0, 'o'));
	EXPECT_EQ(6, waveletTree.select(1, 'o'));
	EXPECT_EQ(25, waveletTree.select(2, 'o'));
	EXPECT_EQ(-1, waveletTree.select(0, 'z'));
	EXPECT_EQ(-1, waveletTree.select(6, 'z'));
}

TEST (WaveletTreeTest, Access) {
	EXPECT_EQ('B', waveletTree.access(0));
	EXPECT_EQ('i', waveletTree.access(1));
	EXPECT_EQ('o', waveletTree.access(2));
	EXPECT_EQ('i', waveletTree.access(3));
	EXPECT_EQ('t', waveletTree.access(10));
	EXPECT_EQ('j', waveletTree.access(30));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
