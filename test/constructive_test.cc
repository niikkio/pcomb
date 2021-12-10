#include <gtest/gtest.h>

#include <tuple>

#include "testing.h"

#include "pcomb/constructive.h"
#include "pcomb/parser.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"

using pcomb::Char;
using pcomb::Construct;
using pcomb::PointerToConstruct;
using pcomb::Seq;

class ConstructiveParserTest : public ::testing::Test {
 protected:
  static auto pPairAB() {
    return Construct<Pair>(Seq(Char('A'), Char('B')));
  }

  static auto ppPairAB() {
    return PointerToConstruct<Pair>(Seq(Char('A'), Char('B')));
  }

  static auto expectedPairAB() {
    return Pair{'A', 'B'};
  }

 private:
  using Pair = std::pair<char, char>;
};

TEST_F(ConstructiveParserTest, Empty) {
  TestParserFail("", pPairAB());
}

TEST_F(ConstructiveParserTest, Match) {
  TestParserSuccess("AB", pPairAB(), expectedPairAB(), 2, CheckEmpty());
}

TEST_F(ConstructiveParserTest, NotMatch) {
  TestParserFail("BA", pPairAB());
}

TEST_F(ConstructiveParserTest, PointerMatch) {
  TestParserSuccess("AB", *(ppPairAB()), expectedPairAB(), 2, CheckEmpty());
}

