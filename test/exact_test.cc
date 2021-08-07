#include <gtest/gtest.h>

#include "common.h"

#include "pcomb/exact.h"
#include "pcomb/predicate.h"

using pcomb::Char;
using pcomb::Exact;

class ExactParserTest : public ::testing::Test { };

TEST_F(ExactParserTest, Empty) {
  TestParserFail("", Exact(Char('A')));
}

TEST_F(ExactParserTest, HeadNotMatch) {
  TestParserFail("B", Exact(Char('A')));
}

TEST_F(ExactParserTest, HeadMatchStreamEnded) {
  TestParserSuccess("A", Exact(Char('A')), 'A', 1, CheckEmpty());
}

TEST_F(ExactParserTest, HeadMatchStreamNotEnded) {
  TestParserFail("AB", Exact(Char('A')));
}

