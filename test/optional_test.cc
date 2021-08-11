#include <gtest/gtest.h>

#include "common.h"

#include "pcomb/optional.h"
#include "pcomb/predicate.h"

using pcomb::Char;
using pcomb::Opt;

class OptionalParserTest : public ::testing::Test { };

TEST_F(OptionalParserTest, Empty) {
  auto parser = Opt(Char('A'));
  using Expected = decltype(parser)::ValueType;
  TestParserSuccess("", parser, Expected{}, 0, CheckEmpty());
}

TEST_F(OptionalParserTest, HeadMatch) {
  auto parser = Opt(Char('A'));
  using Expected = decltype(parser)::ValueType;
  TestParserSuccess("AB", parser, Expected{'A'}, 1, CheckNotEmpty('B'));
}

TEST_F(OptionalParserTest, HeadNotMatch) {
  auto parser = Opt(Char('A'));
  using Expected = decltype(parser)::ValueType;
  TestParserSuccess("BA", parser, Expected{}, 0, CheckNotEmpty('B'));
}
