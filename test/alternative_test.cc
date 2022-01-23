#include <gtest/gtest.h>

#include <list>
#include <variant>
#include <sstream>

#include "testing.h"

#include "pcomb/alternative.h"
#include "pcomb/many.h"
#include "pcomb/messages.h"
#include "pcomb/predicate.h"
#include "pcomb/skipped.h"

class AlternativeParserTest : public ::testing::Test { };

TEST_F(AlternativeParserTest, Name1) {
  auto parser = pcomb::Any(pcomb::Char('A'));
  TestParserName(parser, "Any <Alternative [Char('A')]>");
}

TEST_F(AlternativeParserTest, Name2) {
  using pcomb::Any, pcomb::Char;
  auto parser = Any(Char('A'), Char('B'), Char('C'));
  auto expected_name = "Any <Alternative [Char('A'), Char('B'), Char('C')]>";
  TestParserName(parser, expected_name);
}

TEST_F(AlternativeParserTest, Name3) {
  using pcomb::Any, pcomb::Char, pcomb::Some;
  auto parser = Any(Char('A'), Some(Char('B')));
  auto expected_name = "Any <Alternative [Char('A'), Many [1..] [Char('B')]]>";
  TestParserName(parser, expected_name);
}

TEST_F(AlternativeParserTest, Name4) {
  using pcomb::Many, pcomb::Any, pcomb::Some, pcomb::Char;
  auto parser = Many(Any(Some(Char('A')), Some(Char('B'))));
  auto expected_name = "Many <Many [0..] [Alternative ["
                       "Many [1..] [Char('A')], Many [1..] [Char('B')]]]>";
  TestParserName(parser, expected_name);
}

TEST_F(AlternativeParserTest, Name5) {
  using pcomb::Any, pcomb::Skip, pcomb::Some, pcomb::Char;
  auto parser = Any(Skip(Some(Char('A'))), Skip(Some(Char('B'))));
  auto expected_name = "Any <Alternative [Skipped [Many [1..] [Char('A')]], "
                                         "Skipped [Many [1..] [Char('B')]]]>";
  TestParserName(parser, expected_name);
}

TEST_F(AlternativeParserTest, SingleEmpty) {
  auto input = "";
  auto parser = pcomb::Any(pcomb::Char('A'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::EMPTY_STREAM;
  trace << "\t" << MakeTrace(pcomb::Char('A'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(AlternativeParserTest, SingleMatch) {
  auto input = "A";
  auto parser = pcomb::Any(pcomb::Char('A'));
  TestParserSuccess(input, parser, 'A', 1, CheckEmpty());
}

TEST_F(AlternativeParserTest, SingleNotMatch) {
  auto input = "B";
  auto parser = pcomb::Any(pcomb::Char('A'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('B');
  trace << "\t" << MakeTrace(pcomb::Char('A'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(AlternativeParserTest, NotMatch) {
  using pcomb::Any, pcomb::Char;
  auto input = "D";
  auto parser = Any(Char('A'), Char('B'), Char('C'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('D');
  trace << "\t" << MakeTrace(Char('A'), {0, 0, 0}, message);
  trace << "\t" << MakeTrace(Char('B'), {0, 0, 0}, message);
  trace << "\t" << MakeTrace(Char('C'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(AlternativeParserTest, Take1) {
  using pcomb::Any, pcomb::Char;
  auto input = "ABC";
  auto parser = Any(Char('A'), Char('B'), Char('C'));
  TestParserSuccess(input, parser, 'A', 1, CheckNotEmpty('B'));
}

TEST_F(AlternativeParserTest, Take2) {
  using pcomb::Any, pcomb::Char;
  auto input = "BCA";
  auto parser = Any(Char('A'), Char('B'), Char('C'));
  TestParserSuccess(input, parser, 'B', 1, CheckNotEmpty('C'));
}

TEST_F(AlternativeParserTest, Take3) {
  using pcomb::Any, pcomb::Char;
  auto input = "CAB";
  auto parser = Any(Char('A'), Char('B'), Char('C'));
  TestParserSuccess(input, parser, 'C', 1, CheckNotEmpty('A'));
}

TEST_F(AlternativeParserTest, TakeOneOrMany1) {
  using pcomb::Any, pcomb::Char, pcomb::Some;
  auto input = "ABBB";
  auto parser = Any(Char('A'), Some(Char('B')));
  auto expected = std::variant<char, std::list<char>> {
      std::in_place_index<0>, 'A'};
  TestParserSuccess(input, parser, expected, 1, CheckNotEmpty('B'));
}

TEST_F(AlternativeParserTest, TakeOneOrMany2) {
  using pcomb::Any, pcomb::Char, pcomb::Some;
  auto input = "BBBA";
  auto parser = Any(Char('A'), Some(Char('B')));
  auto expected = std::variant<char, std::list<char>> {
      std::in_place_index<1>, std::list<char>(3, 'B')};
  TestParserSuccess(input, parser, expected, 3, CheckNotEmpty('A'));
}

TEST_F(AlternativeParserTest, ManyAlternatives) {
  using pcomb::Many, pcomb::Any, pcomb::Some, pcomb::Char;
  auto input = "BBAAABAABBB";
  auto parser = Many(Any(Some(Char('A')), Some(Char('B'))));
  auto expected = std::list<std::list<char>>{
      {'B', 'B'}, {'A', 'A', 'A'}, {'B'}, {'A', 'A'}, {'B', 'B', 'B'}};
  TestContainerParserSuccess(input, parser, expected, 11, CheckEmpty());
}

TEST_F(AlternativeParserTest, AlternativeSkipped1) {
  using pcomb::Any, pcomb::Skip, pcomb::Some, pcomb::Char;
  auto input = "ABBBBBC";
  auto parser = Any(Skip(Some(Char('A'))), Skip(Some(Char('B'))));
  auto expected = decltype(parser)::element_type::ValueType();
  TestParserSuccess(input, parser, expected, 1, CheckNotEmpty('B'));
}

TEST_F(AlternativeParserTest, AlternativeSkipped2) {
  using pcomb::Any, pcomb::Skip, pcomb::Some, pcomb::Char;
  auto input = "BBBBBAC";
  auto parser = Any(Skip(Some(Char('A'))), Skip(Some(Char('B'))));
  auto expected = decltype(parser)::element_type::ValueType();
  TestParserSuccess(input, parser, expected, 5, CheckNotEmpty('A'));
}
