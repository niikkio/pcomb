#include <gtest/gtest.h>

#include "testing.h"

#include "pcomb/predicate.h"

using pcomb::AnyChar;
using pcomb::Char;
using pcomb::LatinChar;

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

TEST_F(PredicateParserTest, LatinCharEmpty) {
  TestParserFail("", LatinChar());
}


TEST_F(PredicateParserTest, LatinCharUpperMatch) {
  TestParserSuccess("MN", LatinChar(), 'M', 1, CheckNotEmpty('N'));
}

TEST_F(PredicateParserTest, LatinCharLowerMatch) {
  TestParserSuccess("mn", LatinChar(), 'm', 1, CheckNotEmpty('n'));
}

TEST_F(PredicateParserTest, LatinCharHeadNotMatch) {
  TestParserFail("ЯБ", LatinChar());
}
