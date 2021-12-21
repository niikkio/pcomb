#include <gtest/gtest.h>

#include <list>
#include <memory>

#include "testing.h"

#include "pcomb/chain.h"
#include "pcomb/predicate.h"

using pcomb::Chain;
using pcomb::Char;

class ChainParserTest : public ::testing::Test {
 private:
  using Parsers = std::list<std::shared_ptr<pcomb::CharParserType<char>>>;

 protected:
  static auto pNone() {
    return Chain(Parsers{});
  }

  static auto pABC() {
    return Chain(Parsers{Char('A'), Char('B'), Char('C')});
  }

  using Expected = std::list<char>;
};

TEST_F(ChainParserTest, SomethingFromEmpty) {
  TestContainerParserFail("", pABC());
}

TEST_F(ChainParserTest, NothingFromEmpty) {
  TestContainerParserSuccess("", pNone(), Expected{}, 0, CheckEmpty());
}

TEST_F(ChainParserTest, Match) {
  TestContainerParserSuccess("ABC", pABC(),
                             Expected{'A', 'B', 'C'}, 3, CheckEmpty());
}

TEST_F(ChainParserTest, NotMatch) {
  TestContainerParserFail("ABD", pABC());
}
