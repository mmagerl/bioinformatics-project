#include "../include/gtest/gtest.h"

int increment(const int x) {
	return x > 0 ? x + 1 : x - 1;
}

TEST (IncrementTest, PositiveNumbers) {
    EXPECT_EQ (6, increment (5));
    EXPECT_LT (increment (9), 11);
	EXPECT_GT (increment (15), 15);
}

TEST (IncrementTest, NegativeNumbers) { 
	ASSERT_EQ(0, increment (-1));
	ASSERT_TRUE(increment(-2) == -1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
