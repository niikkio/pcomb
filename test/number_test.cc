#include <gtest/gtest.h>

#include "testing.h"

#include "pcomb/number.h"

class NumberParserTest : public ::testing::Test { };

TEST_F(NumberParserTest, Name1) {
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

class IntParserTest : public ::testing::TestWithParam<std::string> { };

TEST_P(IntParserTest, Success) {
  auto input = GetParam();
  auto parser = pcomb::Int();
  auto expected = std::stoi(input);
  TestParserSuccess(input, parser, expected, input.size(), CheckEmpty());
}

INSTANTIATE_TEST_CASE_P(
  NumberParserTest,
  IntParserTest,
  ::testing::Values("1", "5", "32", "769", "0", "000", "01",
                    "+123", "+0", "+5", "-123", "-0", "-7"));
