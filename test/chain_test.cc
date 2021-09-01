#include <gtest/gtest.h>

#include <list>

#include "common.h"

#include "pcomb/chain.h"
#include "pcomb/predicate.h"

using pcomb::Chain;
using pcomb::Char;

class ChainParserTest : public ::testing::Test {
 protected:
  using Expected = std::list<char>;
  using Parsers = std::list<pcomb::CharParserType<char>>;
};

TEST_F(ChainParserTest, SomethingFromEmpty) {
  TestContainerParserFail("", Chain(Parsers{Char('A'), Char('B')}));
}

TEST_F(ChainParserTest, NothingFromEmpty) {
  TestContainerParserSuccess("", Chain(Parsers{}), Expected{}, 0, CheckEmpty());
}

TEST_F(ChainParserTest, Match) {
  auto parser = Chain(Parsers{Char('A'), Char('B'), Char('C')});
  TestContainerParserSuccess(
      "ABC", parser, Expected{'A', 'B', 'C'}, 3, CheckEmpty());
}

TEST_F(ChainParserTest, NotMatch) {
  auto parser = Chain(Parsers{Char('A'), Char('B'), Char('C')});
  TestContainerParserFail("ABD", parser);
}
