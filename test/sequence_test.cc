#include <gtest/gtest.h>

#include <sstream>
#include <tuple>

#include "testing.h"

#include "pcomb/messages.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"
#include "pcomb/skipped.h"

class SequenceParserTest : public ::testing::Test { };

TEST_F(SequenceParserTest, Name1) {
  auto parser = pcomb::Seq(pcomb::Char('A'));
  TestParserName(parser, "Seq <Sequence [Predicate]>");
}

TEST_F(SequenceParserTest, Name2) {
  using pcomb::Seq, pcomb::Char;
  auto parser = Seq(Char('A'), Char('B'), Char('C'));
  auto expected_name = "Seq <Sequence [Predicate, Predicate, Predicate]>";
  TestParserName(parser, expected_name);
}

TEST_F(SequenceParserTest, Name3) {
  using pcomb::Seq, pcomb::Skip, pcomb::Char;
  auto parser = Seq(Skip(Char('A')), Skip(Char('B')));
  auto expected_name =
      "Seq <Sequence [Skipped [Predicate], Skipped [Predicate]]>";
  TestParserName(parser, expected_name);
}

TEST_F(SequenceParserTest, Name4) {
  using pcomb::Seq, pcomb::Skip, pcomb::Char;
  auto parser =
      Seq(Skip(Char('(')), Char('A'), Char('B'), Char('C'), Skip(Char(')')));
  auto expected_name = "Seq <Sequence [Skipped [Predicate], Predicate, "
                       "Predicate, Predicate, Skipped [Predicate]]>";
  TestParserName(parser, expected_name);
}

TEST_F(SequenceParserTest, Seq1Match) {
  auto input = "A";
  auto parser = pcomb::Seq(pcomb::Char('A'));
  TestParserSuccess(input, parser, 'A', 1, CheckEmpty());
}

TEST_F(SequenceParserTest, Seq1NotMatch) {
  auto input = "B";
  auto parser = pcomb::Seq(pcomb::Char('A'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('B');
  trace << '\t' << MakeTrace(pcomb::Char('A'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(SequenceParserTest, Seq3Match) {
  using pcomb::Seq, pcomb::Char;
  auto input = "ABC";
  auto parser = Seq(Char('A'), Char('B'), Char('C'));
  auto expected = std::tuple<char, char, char>('A', 'B', 'C');
  TestParserSuccess(input, parser, expected, 3, CheckEmpty());
}

TEST_F(SequenceParserTest, Seq3Empty1) {
  using pcomb::Seq, pcomb::Char;
  auto input = "";
  auto parser = Seq(Char('A'), Char('B'), Char('C'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::EMPTY_STREAM;
  trace << '\t' << MakeTrace(pcomb::Char('A'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(SequenceParserTest, Seq3Empty2) {
  using pcomb::Seq, pcomb::Char;
  auto input = "A";
  auto parser = Seq(Char('A'), Char('B'), Char('C'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::EMPTY_STREAM;
  trace << '\t' << MakeTrace(pcomb::Char('B'), {1, 0, 1}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(SequenceParserTest, Seq3Empty3) {
  using pcomb::Seq, pcomb::Char;
  auto input = "AB";
  auto parser = Seq(Char('A'), Char('B'), Char('C'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::EMPTY_STREAM;
  trace << '\t' << MakeTrace(pcomb::Char('C'), {2, 0, 2}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(SequenceParserTest, Seq3NotMatch1) {
  using pcomb::Seq, pcomb::Char;
  auto input = "D";
  auto parser = Seq(Char('A'), Char('B'), Char('C'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('D');
  trace << '\t' << MakeTrace(pcomb::Char('A'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(SequenceParserTest, Seq3NotMatch2) {
  using pcomb::Seq, pcomb::Char;
  auto input = "AD";
  auto parser = Seq(Char('A'), Char('B'), Char('C'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('D');
  trace << '\t' << MakeTrace(pcomb::Char('B'), {1, 0, 1}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(SequenceParserTest, Seq3NotMatch3) {
  using pcomb::Seq, pcomb::Char;
  auto input = "ABD";
  auto parser = Seq(Char('A'), Char('B'), Char('C'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('D');
  trace << '\t' << MakeTrace(pcomb::Char('C'), {2, 0, 2}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(SequenceParserTest, Skip2) {
  using pcomb::Seq, pcomb::Skip, pcomb::Char;
  auto parser = Seq(Skip(Char('A')), Skip(Char('B')));
  auto input = "ABC";
  auto expected = decltype(parser)::element_type::ValueType();
  TestParserSuccess(input, parser, expected, 2, CheckNotEmpty('C'));
}

TEST_F(SequenceParserTest, Skip2NotEnough) {
  using pcomb::Seq, pcomb::Skip, pcomb::Char;
  auto input = "A";
  auto parser = Seq(Skip(Char('A')), Skip(Char('B')));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  trace << "\t" << MakeTrace(Skip(Char('B')), {1, 0, 1});
  auto message = pcomb::messages::EMPTY_STREAM;
  trace << "\t\t" << MakeTrace(Char('B'), {1, 0, 1}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(SequenceParserTest, Brackets) {
  using pcomb::Seq, pcomb::Skip, pcomb::Char;
  auto input = "(ABC)";
  auto parser =
      Seq(Skip(Char('(')), Char('A'), Char('B'), Char('C'), Skip(Char(')')));
  auto expected = std::tuple<char, char, char>('A', 'B', 'C');
  TestParserSuccess(input, parser, expected, 5, CheckEmpty());
}
