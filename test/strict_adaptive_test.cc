#include <gtest/gtest.h>

#include <tuple>

#include "testing.h"
#include "transformers.h"

#include "pcomb/adaptive.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"

using pcomb::Char;
using pcomb::Seq;
using pcomb::StrictAdapted;

class StrictAdaptiveParserTest : public ::testing::Test {
 protected:
  static auto pPairAB() {
    return StrictAdapted(Seq(Char('A'), Char('B')), &tuple2pair);
  }

  static auto expectedPairAB() {
    return std::pair<char, char>{'A', 'B'};
  }
};

TEST_F(StrictAdaptiveParserTest, Empty) {
  TestParserFail("", pPairAB());
}

TEST_F(StrictAdaptiveParserTest, Match) {
  TestParserSuccess("AB", pPairAB(), expectedPairAB(), 2, CheckEmpty());
}

TEST_F(StrictAdaptiveParserTest, NotMatch) {
  TestParserFail("BA", pPairAB());
}
