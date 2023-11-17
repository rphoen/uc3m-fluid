#include <gtest/gtest.h>
#include "../sim/progargs.hpp"

TEST(ProgargsTest, ValidInput) {
  int argc = 4;
  const char *argv[] = {"progargs", "10", "small.fld", "out/test.fld"};

  int result = progargs(argc, (char **)argv);

  ASSERT_EQ(result, 0);
}

TEST(ProgargsTest, InvalidNumberOfArguments) {
  int argc = 3;
  const char *argv[] = {"progargs", "10", "small.fld"};

  int result = progargs(argc, (char **)argv);

  ASSERT_EQ(result, -1);
}

TEST(ProgargsTest, InvalidTimeSteps) {
  int argc = 4;
  const char *argv[] = {"progargs", "0", "small.fld", "out/test.fld"};

  int result = progargs(argc, (char **)argv);

  ASSERT_EQ(result, -1);
}

TEST(ProgargsTest, NegativeTimeSteps) {
  int argc = 4;
  const char *argv[] = {"progargs", "-1", "small.fld", "out/test.fld"};

  int result = progargs(argc, (char **)argv);

  ASSERT_EQ(result, -2);
}

TEST(ProgargsTest, CannotOpenInputFile) {
  int argc = 4;
  const char *argv[] = {"progargs", "10", "nonexistent.txt", "out/test.fld"};

  int result = progargs(argc, (char **)argv);

  ASSERT_EQ(result, -3);
}

TEST(ProgargsTest, CannotOpenOutputFile) {
  int argc = 4;
  const char *argv[] = {"progargs", "10", "small.fld", "nonexistent.txt"};

  int result = progargs(argc, (char **)argv);

  ASSERT_EQ(result, -4);
}
