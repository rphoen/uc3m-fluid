#include <gtest/gtest.h>
#include <unordered_map>
#include <vector>

#include "../sim/hash.cpp"

using namespace hashing;

TEST(VHashTest, EmptyVector) {
  std::vector<int> emptyVector;
  vHash hashFunction;

  auto hashValue = hashFunction(emptyVector);
  EXPECT_EQ(hashValue, 0);
}

TEST(VHashTest, DifferentVectors) {
  std::vector<int> vector1 = {1, 2, 3};
  std::vector<int> vector2 = {3, 2, 1};

  vHash hashFunction;

  auto hashValue1 = hashFunction(vector1);
  auto hashValue2 = hashFunction(vector2);

  EXPECT_NE(hashValue1, hashValue2);
}
