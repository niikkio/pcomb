#include <gtest/gtest.h>

#include <list>

#include "testing.h"

#include "pcomb/predicate.h"
#include "pcomb/until.h"

class UntilParserTest : public ::testing::Test { };

TEST_F(UntilParserTest, Name) {
  auto parser = pcomb::Until(pcomb::Char('A'));
  auto expected_name = "Until <Until [Char('A')]>";
  TestParserName(parser, expected_name);
}

TEST_F(UntilParserTest, Empty) {
  auto input = "";
  auto parser = pcomb::Until(pcomb::Char('A'));
  auto expected = std::list<char>{};
  TestContainerParserSuccess(input, parser, expected, 0, CheckEmpty());
}

TEST_F(UntilParserTest, EmptyMatch) {
  auto input = "A";
  auto parser = pcomb::Until(pcomb::Char('A'));
  auto expected = std::list<char>{};
  TestContainerParserSuccess(input, parser, expected, 0, CheckNotEmpty('A'));
}

TEST_F(UntilParserTest, Match) {
  auto input = "BCA";
  auto parser = pcomb::Until(pcomb::Char('A'));
  auto expected = std::list<char>{'B', 'C'};
  TestContainerParserSuccess(input, parser, expected, 2, CheckNotEmpty('A'));
}
