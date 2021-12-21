#include <gtest/gtest.h>

#include <optional>

#include "testing.h"

#include "pcomb/optional.h"
#include "pcomb/predicate.h"

using pcomb::Char;
using pcomb::Opt;
using pcomb::ParseWithDefault;

class OptionalParserTest : public ::testing::Test {
 protected:
  static auto pA() {
    return Opt(Char('A'));
  }

  static auto pAB() {
    return ParseWithDefault(Char('A'), 'B');
  }

  using ExpectedA = std::optional<char>;
};

TEST_F(OptionalParserTest, Empty) {
  TestParserSuccess("", pA(), ExpectedA{}, 0, CheckEmpty());
}

TEST_F(OptionalParserTest, HeadMatch) {
  TestParserSuccess("AB", pA(), ExpectedA{'A'}, 1, CheckNotEmpty('B'));
}

TEST_F(OptionalParserTest, HeadNotMatch) {
  TestParserSuccess("BA", pA(), ExpectedA{}, 0, CheckNotEmpty('B'));
}

TEST_F(OptionalParserTest, EmptyWithDefault) {
  TestParserSuccess("", pAB(), 'B', 0, CheckEmpty());
}

TEST_F(OptionalParserTest, HeadMatchWithDefault) {
  TestParserSuccess("AB", pAB(), 'A', 1, CheckNotEmpty('B'));
}

TEST_F(OptionalParserTest, HeadNotMatchWithDefault) {
  TestParserSuccess("BA", pAB(), 'B', 0, CheckNotEmpty('B'));
}
