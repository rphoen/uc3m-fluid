#include <gtest/gtest.h>
#include "../sim/progargs.hpp"

TEST(ProgargsTest, ValidInput) {
  int const argc = 4;
  std::array<char *, 4> const argv = {"progargs", "10", "small.fld", "out/test.fld"};

  int const result = progargs(argc, argv);

  ASSERT_EQ(result, 0);
}

TEST(ProgargsTest, InvalidNumberOfArguments) {
  int const argc = 3;
  std::array<char *, 4> const argv = {"progargs", "10", "small.fld"};

  int const result = progargs(argc, argv);

  ASSERT_EQ(result, -1);
}

TEST(ProgargsTest, InvalidTimeSteps) {
  int const argc = 4;
  std::array<char *, 4> const argv = {"progargs", "0", "small.fld", "out/test.fld"};

  int const result = progargs(argc, argv);

  ASSERT_EQ(result, -1);
}

TEST(ProgargsTest, NegativeTimeSteps) {
  int const argc = 4;
  std::array<char *, 4> const argv = {"progargs", "-1", "small.fld", "out/test.fld"};

  int const result = progargs(argc, argv);

  ASSERT_EQ(result, -2);
}

TEST(ProgargsTest, CannotOpenInputFile) {
  int const argc = 4;
  std::array<char *, 4> const argv = {"progargs", "10", "nonexistent.txt", "out/test.fld"};

  int const result = progargs(argc, argv);

  ASSERT_EQ(result, -3);
}

TEST(ProgargsTest, CannotOpenOutputFile) {
  int const argc = 4;
  std::array<char *, 4> const argv = {"progargs", "10", "small.fld", "nonexistent.txt"};

  int const result = progargs(argc, argv);

  ASSERT_EQ(result, -4);
}
