#include <gtest/gtest.h>

#include <sstream>
#include <utility>

#include "testing.h"

#include "pcomb/constructive.h"
#include "pcomb/messages.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"

class ConstructiveParserTest : public ::testing::Test { };

TEST_F(ConstructiveParserTest, Name) {
  using pcomb::Construct, pcomb::Seq, pcomb::Char;
  using P = std::pair<char, char>;
  auto parser = Construct<P>(Seq(Char('A'), Char('B')));
  auto expected_name =
      "Construct <Constructive [Sequence [Char('A'), Char('B')]]>";
  TestParserName(parser, expected_name);
}

TEST_F(ConstructiveParserTest, Empty) {
  auto input = "";
  using pcomb::Construct, pcomb::Seq, pcomb::Char;
  using P = std::pair<char, char>;
  auto parser = Construct<P>(Seq(Char('A'), Char('B')));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  trace << "\t" << MakeTrace(Seq(Char('A'), Char('B')), {0, 0, 0});
  auto message = pcomb::messages::EMPTY_STREAM;
  trace << "\t\t" << MakeTrace(Char('A'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(ConstructiveParserTest, Match) {
  auto input = "AB";
  using pcomb::Construct, pcomb::Seq, pcomb::Char;
  using P = std::pair<char, char>;
  auto parser = Construct<P>(Seq(Char('A'), Char('B')));
  auto expected = P{'A', 'B'};
  TestParserSuccess(input, parser, expected, 2, CheckEmpty());
}

TEST_F(ConstructiveParserTest, NotMatch) {
  auto input = "BA";
  using pcomb::Construct, pcomb::Seq, pcomb::Char;
  using P = std::pair<char, char>;
  auto parser = Construct<P>(Seq(Char('A'), Char('B')));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  trace << "\t" << MakeTrace(Seq(Char('A'), Char('B')), {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('B');
  trace << "\t\t" << MakeTrace(Char('A'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}
