#include <gtest/gtest.h>

#include <list>
#include <sstream>
#include <tuple>

#include "testing.h"

#include "pcomb/messages.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"

class StrictSequenceParserTest : public ::testing::Test { };

TEST_F(StrictSequenceParserTest, Name1) {
  auto parser = pcomb::StrictSeq(pcomb::Char('A'));
  auto expected_name = "StrictSeq <Strict Sequence [Char('A')]>";
  TestParserName(parser, expected_name);
}

TEST_F(StrictSequenceParserTest, Name2) {
  using pcomb::StrictSeq, pcomb::Char;
  auto parser = StrictSeq(Char('A'), Char('B'), Char('C'));
  auto expected_name =
      "StrictSeq <Strict Sequence [Char('A'), Char('B'), Char('C')]>";
  TestParserName(parser, expected_name);
}

TEST_F(StrictSequenceParserTest, Seq1Match) {
  auto input = "A";
  auto parser = pcomb::StrictSeq(pcomb::Char('A'));
  auto expected = std::tuple<char>{'A'};
  TestParserSuccess(input, parser, expected, 1, CheckEmpty());
}

TEST_F(StrictSequenceParserTest, Seq1NotMatch) {
  auto input = "B";
  auto parser = pcomb::StrictSeq(pcomb::Char('A'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('B');
  trace << "\t" << MakeTrace(pcomb::Char('A'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(StrictSequenceParserTest, Seq3Match) {
  using pcomb::StrictSeq, pcomb::Char;
  auto input = "ABC";
  auto parser = StrictSeq(Char('A'), Char('B'), Char('C'));
  auto expected = std::tuple<char, char, char>('A', 'B', 'C');
  TestParserSuccess(input, parser, expected, 3, CheckEmpty());
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch1) {
  using pcomb::StrictSeq, pcomb::Char;
  auto input = "";
  auto parser = StrictSeq(Char('A'), Char('B'), Char('C'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::EMPTY_STREAM;
  trace << "\t" << MakeTrace(Char('A'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch2) {
  using pcomb::StrictSeq, pcomb::Char;
  auto input = "A";
  auto parser = StrictSeq(Char('A'), Char('B'), Char('C'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::EMPTY_STREAM;
  trace << "\t" << MakeTrace(Char('B'), {1, 0, 1}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch3) {
  using pcomb::StrictSeq, pcomb::Char;
  auto input = "AB";
  auto parser = StrictSeq(Char('A'), Char('B'), Char('C'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::EMPTY_STREAM;
  trace << "\t" << MakeTrace(Char('C'), {2, 0, 2}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch4) {
  using pcomb::StrictSeq, pcomb::Char;
  auto input = "ABB";
  auto parser = StrictSeq(Char('A'), Char('B'), Char('C'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('B');
  trace << "\t" << MakeTrace(Char('C'), {2, 0, 2}, message);

  TestParserFail(input, parser, trace.str());
}
