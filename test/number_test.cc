#include <gtest/gtest.h>

#include "testing.h"

#include "pcomb/number.h"

class NumberParserTest : public ::testing::Test { };

TEST_F(NumberParserTest, DISABLED_Name1) {
  auto parser = pcomb::Int();
  TestParserName(parser, "Int <Number(int)>");
}

/*
TEST_F(NumberParserTest, Name2) {
  auto parser = pcomb::UInt();
  TestParserName(parser, "UInt <Number(uint)>");
}

TEST_F(NumberParserTest, Name3) {
  auto parser = pcomb::Double();
  TestParserName(parser, "Double <Number(double)>");
}
*/

TEST_F(NumberParserTest, SimpleOk) {
  auto input = "7";
  auto parser = pcomb::Int();
  TestParserSuccess(input, parser, 7, 1, CheckEmpty());
}

class IntParserTest : public ::testing::TestWithParam<int> { };

TEST_P(IntParserTest, DISABLED_Success) {
  auto param = GetParam();
  auto input = std::to_string(param);
  auto parser = pcomb::Int();
  auto expected = param;
  TestParserSuccess(input, parser, expected, input.size(), CheckEmpty());
}

INSTANTIATE_TEST_CASE_P(
  NumberParserTest,
  IntParserTest,
  ::testing::Values(1, 22, 3, 41, 52, 6, 7, 8, 9));
