#include <gtest/gtest.h>

#include "common.h"

#include "pcomb/predicate.h"

using pcomb::AnyChar;
using pcomb::Char;

class PredicateParserTest : public ::testing::Test { };

TEST_F(PredicateParserTest, Empty) {
  TestParserFail("", Char('A'));
}

TEST_F(PredicateParserTest, HeadMatch) {
  TestParserSuccess("AB", Char('A'), 'A', 1, CheckNotEmpty('B'));
}

TEST_F(PredicateParserTest, HeadNotMatch) {
  TestParserFail("BA", Char('A'));
}

TEST_F(PredicateParserTest, AnyCharEmpty) {
  TestParserFail("", AnyChar());
}

TEST_F(PredicateParserTest, AnyCharNotEmpty) {
  TestParserSuccess("AB", AnyChar(), 'A', 1, CheckNotEmpty('B'));
}
