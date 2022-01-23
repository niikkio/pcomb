#include <gtest/gtest.h>

#include <sstream>
#include <utility>

#include "testing.h"
#include "transformers.h"

#include "pcomb/adaptive.h"
#include "pcomb/messages.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"

class StrictAdaptiveParserTest : public ::testing::Test { };

TEST_F(StrictAdaptiveParserTest, Name) {
  using pcomb::StrictAdapted, pcomb::Seq, pcomb::Char;
  auto parser = StrictAdapted(Seq(Char('A'), Char('B')), &tuple2pair);
  auto expected_name =
      "StrictAdapted <Strict Adaptive [Sequence [Char('A'), Char('B')]]>";
  TestParserName(parser, expected_name);
}

TEST_F(StrictAdaptiveParserTest, Empty) {
  using pcomb::StrictAdapted, pcomb::Seq, pcomb::Char;
  auto input = "";
  auto parser = StrictAdapted(Seq(Char('A'), Char('B')), &tuple2pair);

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  trace << "\t" << MakeTrace(Seq(Char('A'), Char('B')), {0, 0, 0});
  auto message = pcomb::messages::EMPTY_STREAM;
  trace << "\t\t" << MakeTrace(Char('A'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(StrictAdaptiveParserTest, Match) {
  using pcomb::StrictAdapted, pcomb::Seq, pcomb::Char;
  auto input = "AB";
  auto parser = StrictAdapted(Seq(Char('A'), Char('B')), &tuple2pair);
  auto expected = std::pair<char, char>{'A', 'B'};
  TestParserSuccess(input, parser, expected, 2, CheckEmpty());
}

TEST_F(StrictAdaptiveParserTest, NotMatch1) {
  using pcomb::StrictAdapted, pcomb::Seq, pcomb::Char;
  auto input = "BA";
  auto parser = StrictAdapted(Seq(Char('A'), Char('B')), &tuple2pair);

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  trace << "\t" << MakeTrace(Seq(Char('A'), Char('B')), {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('B');
  trace << "\t\t" << MakeTrace(Char('A'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(StrictAdaptiveParserTest, NotMatch2) {
  using pcomb::StrictAdapted, pcomb::Seq, pcomb::Char;
  auto input = "AA";
  auto parser = StrictAdapted(Seq(Char('A'), Char('B')), &tuple2pair);

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  trace << "\t" << MakeTrace(Seq(Char('A'), Char('B')), {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('A');
  trace << "\t\t" << MakeTrace(Char('B'), {1, 0, 1}, message);

  TestParserFail(input, parser, trace.str());
}
