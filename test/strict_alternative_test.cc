#include <gtest/gtest.h>

#include <list>
#include <sstream>
#include <variant>

#include "testing.h"

#include "pcomb/alternative.h"
#include "pcomb/many.h"
#include "pcomb/messages.h"
#include "pcomb/predicate.h"

class StrictAlternativeParserTest : public ::testing::Test { };

TEST_F(StrictAlternativeParserTest, Name1) {
  auto parser = pcomb::StrictAny(pcomb::Char('A'));
  TestParserName(parser, "StrictAny <Strict Alternative [Char('A')]>");
}

TEST_F(StrictAlternativeParserTest, Name2) {
  using pcomb::StrictAny, pcomb::Char;
  auto parser = StrictAny(Char('A'), Char('B'), Char('C'));
  auto expected_name =
      "StrictAny <Strict Alternative [Char('A'), Char('B'), Char('C')]>";
  TestParserName(parser, expected_name);
}

TEST_F(StrictAlternativeParserTest, Name3) {
  using pcomb::StrictAny, pcomb::Char, pcomb::Some;
  auto parser = StrictAny(Char('A'), Some(Char('B')));
  auto expected_name =
      "StrictAny <Strict Alternative [Char('A'), Many [1..] [Char('B')]]>";
  TestParserName(parser, expected_name);
}

TEST_F(StrictAlternativeParserTest, Name4) {
  using pcomb::Many, pcomb::StrictAny, pcomb::Some, pcomb::Char;
  auto parser = Many(StrictAny(Some(Char('A')), Some(Char('B'))));
  auto expected_name = "Many <Many [0..] [Strict Alternative ["
                       "Many [1..] [Char('A')], Many [1..] [Char('B')]]]>";
  TestParserName(parser, expected_name);
}

TEST_F(StrictAlternativeParserTest, SingleMatch) {
  auto input = "A";
  auto parser = pcomb::StrictAny(pcomb::Char('A'));
  auto expected = std::variant<char>{std::in_place_index<0>, 'A'};
  TestParserSuccess(input, parser, expected, 1, CheckEmpty());
}

TEST_F(StrictAlternativeParserTest, SingleNotMatch) {
  auto input = "B";
  auto parser = pcomb::StrictAny(pcomb::Char('A'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('B');
  trace << '\t' << MakeTrace(pcomb::Char('A'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(StrictAlternativeParserTest, Take1) {
  using pcomb::StrictAny, pcomb::Char;
  auto input = "ABC";
  auto parser = StrictAny(Char('A'), Char('B'), Char('C'));
  auto expected = std::variant<char, char, char>{std::in_place_index<0>, 'A'};
  TestParserSuccess(input, parser, expected, 1, CheckNotEmpty('B'));
}

TEST_F(StrictAlternativeParserTest, Take2) {
  using pcomb::StrictAny, pcomb::Char;
  auto input = "BCA";
  auto parser = StrictAny(Char('A'), Char('B'), Char('C'));
  auto expected = std::variant<char, char, char>{std::in_place_index<1>, 'B'};
  TestParserSuccess(input, parser, expected, 1, CheckNotEmpty('C'));
}

TEST_F(StrictAlternativeParserTest, Take3) {
  using pcomb::StrictAny, pcomb::Char;
  auto input = "CAB";
  auto parser = StrictAny(Char('A'), Char('B'), Char('C'));
  auto expected = std::variant<char, char, char>{std::in_place_index<2>, 'C'};
  TestParserSuccess(input, parser, expected, 1, CheckNotEmpty('A'));
}

TEST_F(StrictAlternativeParserTest, NotMatch) {
  using pcomb::StrictAny, pcomb::Char;
  auto input = "D";
  auto parser = StrictAny(Char('A'), Char('B'), Char('C'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('D');
  trace << '\t' << MakeTrace(pcomb::Char('A'), {0, 0, 0}, message);
  trace << '\t' << MakeTrace(pcomb::Char('B'), {0, 0, 0}, message);
  trace << '\t' << MakeTrace(pcomb::Char('C'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(StrictAlternativeParserTest, TakeOneOrMany1) {
  using pcomb::StrictAny, pcomb::Char, pcomb::Some;
  auto input = "ABBB";
  auto parser = StrictAny(Char('A'), Some(Char('B')));
  auto expected = std::variant<char, std::list<char>>{
      std::in_place_index<0>, 'A'};
  TestParserSuccess(input, parser, expected, 1, CheckNotEmpty('B'));
}

TEST_F(StrictAlternativeParserTest, TakeOneOrMany2) {
  using pcomb::StrictAny, pcomb::Char, pcomb::Some;
  auto input = "BBBA";
  auto parser = StrictAny(Char('A'), Some(Char('B')));
  auto expected = std::variant<char, std::list<char>>{
      std::in_place_index<1>, std::list<char>(3, 'B')};
  TestParserSuccess(input, parser, expected, 3, CheckNotEmpty('A'));
}

TEST_F(StrictAlternativeParserTest, ManyAlternatives) {
  using pcomb::Many, pcomb::StrictAny, pcomb::Some, pcomb::Char;
  auto input = "BBAAABAABBB";
  auto parser = Many(StrictAny(Some(Char('A')), Some(Char('B'))));
  using L = std::list<char>;
  using V = std::variant<L, L>;

  auto expected = std::list<V>{
    V{std::in_place_index<1>, L{'B', 'B'}},
    V{std::in_place_index<0>, L{'A', 'A', 'A'}},
    V{std::in_place_index<1>, L{'B'}},
    V{std::in_place_index<0>, L{'A', 'A'}},
    V{std::in_place_index<1>, L{'B', 'B', 'B'}}
  };

  TestContainerParserSuccess(input, parser, expected, 11, CheckEmpty());
}
