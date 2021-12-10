#include <gtest/gtest.h>

#include "testing.h"

#include "pcomb/exact.h"
#include "pcomb/predicate.h"

using pcomb::Char;
using pcomb::Exact;

class ExactParserTest : public ::testing::Test {
 protected:
  static auto pA() {
    return Exact(Char('A'));
  }
};

TEST_F(ExactParserTest, Empty) {
  TestParserFail("", pA());
}

TEST_F(ExactParserTest, HeadNotMatch) {
  TestParserFail("B", pA());
}

TEST_F(ExactParserTest, HeadMatchStreamEnded) {
  TestParserSuccess("A", pA(), 'A', 1, CheckEmpty());
}

TEST_F(ExactParserTest, HeadMatchStreamNotEnded) {
  TestParserFail("AB", pA());
}

