#include <gtest/gtest.h>

#include <tuple>

#include "common.h"

#include "pcomb/constructive.h"
#include "pcomb/parser.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"

using pcomb::Char;
using pcomb::Construct;
using pcomb::ParserPointerType;
using pcomb::Seq;

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

TEST_F(ConstructiveParserTest, PointerMatch) {
  ParserPointerType<Pair> parser =
      pcomb::PointerToConstruct<Pair>(Seq(Char('A'), Char('B')));
  TestParserSuccess("AB", *parser, Pair{'A', 'B'}, 2, CheckEmpty());
}

