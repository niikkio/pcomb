#include <gtest/gtest.h>

#include "testing.h"

#include "pcomb/predicate.h"

using pcomb::AnyChar;
using pcomb::Char;
using pcomb::LatinChar;

class PredicateParserTest : public ::testing::Test {
 protected:
  static auto pA() {
    return Char('A');
  }

  static auto pAny() {
    return AnyChar();
  }

  static auto pLatin() {
    return LatinChar();
  }
};

TEST_F(PredicateParserTest, Empty) {
  TestParserFail("", pA());
}

TEST_F(PredicateParserTest, HeadMatch) {
  TestParserSuccess("AB", pA(), 'A', 1, CheckNotEmpty('B'));
}

TEST_F(PredicateParserTest, HeadNotMatch) {
  TestParserFail("BA", pA());
}

TEST_F(PredicateParserTest, AnyCharEmpty) {
  TestParserFail("", pAny());
}

TEST_F(PredicateParserTest, AnyCharNotEmpty) {
  TestParserSuccess("AB", pAny(), 'A', 1, CheckNotEmpty('B'));
}

TEST_F(PredicateParserTest, LatinCharEmpty) {
  TestParserFail("", pLatin());
}

TEST_F(PredicateParserTest, LatinCharUpperMatch) {
  TestParserSuccess("MN", pLatin(), 'M', 1, CheckNotEmpty('N'));
}

TEST_F(PredicateParserTest, LatinCharLowerMatch) {
  TestParserSuccess("mn", pLatin(), 'm', 1, CheckNotEmpty('n'));
}

TEST_F(PredicateParserTest, LatinCharHeadNotMatch) {
  TestParserFail("ЯБ", pLatin());
}
