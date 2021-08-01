#include <gtest/gtest.h>

#include "common.h"

#include "pcomb/exact.h"
#include "pcomb/predicate.h"

class ExactParserTest : public ::testing::Test {
 protected:
  using Ch = pcomb::PredicateParser<char>;
  using Ex = pcomb::ExactParser<Ch>;

  Ch pa = Ch([](char c) { return c == 'A'; });
  Ex pe = Ex(pa);
};

TEST_F(ExactParserTest, Empty) {
  TestParserFail("", pe);
}

TEST_F(ExactParserTest, HeadNotMatch) {
  TestParserFail("B", pe);
}

TEST_F(ExactParserTest, HeadMatchStreamEnded) {
  TestParserSuccess("A", pe, 'A', 1, CheckEmpty());
}

TEST_F(ExactParserTest, HeadMatchStreamNotEnded) {
  TestParserFail("AB", pe);
}

