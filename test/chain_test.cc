#include <gtest/gtest.h>

#include <list>
#include <memory>
#include <sstream>

#include "testing.h"

#include "pcomb/chain.h"
#include "pcomb/messages.h"
#include "pcomb/predicate.h"

class ChainParserTest : public ::testing::Test {
 protected:
  using Parsers = std::list<pcomb::ParserPointer<pcomb::CharParserType<char>>>;
};

TEST_F(ChainParserTest, Name1) {
  auto parser = pcomb::Chain(Parsers{});
  TestParserName(parser, "Chain <Dynamic Sequence []>");
}

TEST_F(ChainParserTest, Name2) {
  using pcomb::Char, pcomb::Chain;
  auto parser = Chain(Parsers{Char('A'), Char('B'), Char('C')});
  auto expected_name =
      "Chain <Dynamic Sequence [Char('A'), Char('B'), Char('C')]>";
  TestParserName(parser, expected_name);
}

TEST_F(ChainParserTest, NothingFromEmpty) {
  auto input = "";
  auto parser = pcomb::Chain(Parsers{});
  auto expected = std::list<char>{};
  TestContainerParserSuccess(input, parser, expected, 0, CheckEmpty());
}

TEST_F(ChainParserTest, SomethingFromEmpty) {
  using pcomb::Char, pcomb::Chain;
  auto input = "";
  auto parser = Chain(Parsers{Char('A'), Char('B'), Char('C')});

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::EMPTY_STREAM;
  trace << "\t" << MakeTrace(Char('A'), {0, 0, 0}, message);

  TestContainerParserFail(input, parser, trace.str());
}

TEST_F(ChainParserTest, Match) {
  using pcomb::Char, pcomb::Chain;
  auto input = "ABC";
  auto parser = Chain(Parsers{Char('A'), Char('B'), Char('C')});
  auto expected = std::list<char>{'A', 'B', 'C'};
  TestContainerParserSuccess(input, parser, expected, 3, CheckEmpty());
}

TEST_F(ChainParserTest, NotMatch) {
  using pcomb::Char, pcomb::Chain;
  auto input = "ABD";
  auto parser = Chain(Parsers{Char('A'), Char('B'), Char('C')});

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('D');
  trace << "\t" << MakeTrace(Char('C'), {2, 0, 2}, message);

  TestContainerParserFail(input, parser, trace.str());
}
