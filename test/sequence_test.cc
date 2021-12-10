#include <gtest/gtest.h>

#include <list>
#include <tuple>

#include "testing.h"

#include "pcomb/predicate.h"
#include "pcomb/sequence.h"
#include "pcomb/skipped.h"

using pcomb::Char;
using pcomb::Seq;
using pcomb::Skip;

class SequenceParserTest : public ::testing::Test { };

TEST_F(SequenceParserTest, Seq1Match) {
  TestParserSuccess("A", Seq(Char('A')), 'A', 1, CheckEmpty());
}

TEST_F(SequenceParserTest, Seq1NotMatch) {
  TestParserFail("B", Seq(Char('A')));
}

TEST_F(SequenceParserTest, Seq3Match) {
  auto parser = Seq(Char('A'), Char('B'), Char('C'));
  using Expected = std::tuple<char, char, char>;
  TestParserSuccess("ABC", parser, Expected{'A', 'B', 'C'}, 3, CheckEmpty());
}

TEST_F(SequenceParserTest, Seq3NotMatch1) {
  auto parser = Seq(Char('A'), Char('B'), Char('C'));
  TestParserFail("", parser);
}

TEST_F(SequenceParserTest, Seq3NotMatch2) {
  auto parser = Seq(Char('A'), Char('B'), Char('C'));
  TestParserFail("A", parser);
}

TEST_F(SequenceParserTest, Seq3NotMatch3) {
  auto parser = Seq(Char('A'), Char('B'), Char('C'));
  TestParserFail("AB", parser);
}

TEST_F(SequenceParserTest, Seq3NotMatch4) {
  auto parser = Seq(Char('A'), Char('B'), Char('C'));
  TestParserFail("ABB", parser);
}

TEST_F(SequenceParserTest, Skip2) {
  auto parser = Seq(Skip(Char('A')), Skip(Char('B')));
  using Expected = decltype(parser)::ValueType;
  TestParserSuccess("ABC", parser, Expected{}, 2, CheckNotEmpty('C'));
}

TEST_F(SequenceParserTest, Skip2NotEnough) {
  auto parser = Seq(Skip(Char('A')), Skip(Char('B')));
  TestParserFail("A", parser);
}

TEST_F(SequenceParserTest, Brackets) {
  auto parser = Seq(
      Skip(Char('(')), Char('A'), Char('B'), Char('C'), Skip(Char(')')));
  using Expected = std::tuple<char, char, char>;
  TestParserSuccess("(ABC)", parser, Expected{'A', 'B', 'C'}, 5, CheckEmpty());
}
