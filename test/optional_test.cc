#include <gtest/gtest.h>

#include <optional>

#include "testing.h"

#include "pcomb/optional.h"
#include "pcomb/predicate.h"

class OptionalParserTest : public ::testing::Test { };

TEST_F(OptionalParserTest, Name1) {
  auto parser = pcomb::Opt(pcomb::Char('A'));
  TestParserName(parser, "Opt <Optional [Predicate]>");
}

TEST_F(OptionalParserTest, Name2) {
  auto parser = pcomb::WithDefault(pcomb::Char('A'), 'B');
  auto expected_name = "WithDefault('B') <Adaptive [Optional [Predicate]]>";
  TestParserName(parser, expected_name);
}

TEST_F(OptionalParserTest, Empty) {
  auto input = "";
  auto parser = pcomb::Opt(pcomb::Char('A'));
  auto expected = std::optional<char>{};
  TestParserSuccess(input, parser, expected, 0, CheckEmpty());
}

TEST_F(OptionalParserTest, HeadMatch) {
  auto input = "AB";
  auto parser = pcomb::Opt(pcomb::Char('A'));
  auto expected = std::optional<char>{'A'};
  TestParserSuccess(input, parser, expected, 1, CheckNotEmpty('B'));
}

TEST_F(OptionalParserTest, HeadNotMatch) {
  auto input = "BA";
  auto parser = pcomb::Opt(pcomb::Char('A'));
  auto expected = std::optional<char>{};
  TestParserSuccess(input, parser, expected, 0, CheckNotEmpty('B'));
}

TEST_F(OptionalParserTest, EmptyWithDefault) {
  auto input = "";
  auto parser = pcomb::WithDefault(pcomb::Char('A'), 'B');
  TestParserSuccess(input, parser, 'B', 0, CheckEmpty());
}

TEST_F(OptionalParserTest, HeadMatchWithDefault) {
  auto input = "AB";
  auto parser = pcomb::WithDefault(pcomb::Char('A'), 'B');
  TestParserSuccess(input, parser, 'A', 1, CheckNotEmpty('B'));
}

TEST_F(OptionalParserTest, HeadNotMatchWithDefault) {
  auto input = "BA";
  auto parser = pcomb::WithDefault(pcomb::Char('A'), 'B');
  TestParserSuccess(input, parser, 'B', 0, CheckNotEmpty('B'));
}
