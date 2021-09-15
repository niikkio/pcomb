#include <gtest/gtest.h>

#include <tuple>

#include "common.h"

#include "pcomb/constructive.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"

using pcomb::Char;
using pcomb::Seq;
using pcomb::Construct;

class ConstructiveParserTest : public ::testing::Test {
 protected:
  struct Pair {
    char a;
    char b;

    bool operator==(const Pair& other) const {
      return a == other.a && b == other.b;
    }
  };
};

TEST_F(ConstructiveParserTest, Empty) {
  auto parser = Construct<Pair>(Seq(Char('A'), Char('B')));
  TestParserFail("", parser);
}

TEST_F(ConstructiveParserTest, Match) {
  auto parser = Construct<Pair>(Seq(Char('A'), Char('B')));
  TestParserSuccess("AB", parser, Pair{'A', 'B'}, 2, CheckEmpty());
}

TEST_F(ConstructiveParserTest, NotMatch) {
  auto parser = Construct<Pair>(Seq(Char('A'), Char('B')));
  TestParserFail("BA", parser);
}
