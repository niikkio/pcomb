#include <gtest/gtest.h>

#include <list>
#include <variant>

#include "common.h"

#include "pcomb/alternative.h"
#include "pcomb/many.h"
#include "pcomb/predicate.h"
#include "pcomb/skipped.h"

using pcomb::Any;
using pcomb::Char;
using pcomb::Many;
using pcomb::Skip;
using pcomb::Some;

class AlternativeParserTest : public ::testing::Test { };

TEST_F(AlternativeParserTest, SingleMatch) {
  TestParserSuccess("A", Any(Char('A')), 'A', 1, CheckEmpty());
}

TEST_F(AlternativeParserTest, SingleNotMatch) {
  TestParserFail("B", Any(Char('A')));
}

TEST_F(AlternativeParserTest, Take1) {
  auto parser = Any(Char('A'), Char('B'), Char('C'));
  TestParserSuccess("ABC", parser, 'A', 1, CheckNotEmpty('B'));
}

TEST_F(AlternativeParserTest, Take2) {
  auto parser = Any(Char('A'), Char('B'), Char('C'));
  TestParserSuccess("BCA", parser, 'B', 1, CheckNotEmpty('C'));
}

TEST_F(AlternativeParserTest, Take3) {
  auto parser = Any(Char('A'), Char('B'), Char('C'));
  TestParserSuccess("CAB", parser, 'C', 1, CheckNotEmpty('A'));
}

TEST_F(AlternativeParserTest, TakeOneOrMany1) {
  using Expected = std::variant<char, std::list<char> >;
  auto parser = Any(Char('A'), Some(Char('B')));
  TestParserSuccess("ABBB", parser,
                    Expected{std::in_place_index<0>, 'A'}, 1,
                    CheckNotEmpty('B'));
}

TEST_F(AlternativeParserTest, TakeOneOrMany2) {
  using Expected = std::variant<char, std::list<char> >;
  auto parser = Any(Char('A'), Some(Char('B')));
  TestParserSuccess(
      "BBBA", parser,
      Expected{std::in_place_index<1>, std::list<char>{'B', 'B', 'B'}}, 3,
      CheckNotEmpty('A'));
}

TEST_F(AlternativeParserTest, ManyAlternatives) {
  auto parser = Many(Any(Some(Char('A')), Some(Char('B'))));
  auto expected = std::list<std::list<char>>{
      std::list<char>{'B', 'B'},
      std::list<char>{'A', 'A', 'A'},
      std::list<char>{'B'},
      std::list<char>{'A', 'A'},
      std::list<char>{'B', 'B', 'B'}
  };

  TestContainerParserSuccess("BBAAABAABBB", parser, expected, 11, CheckEmpty());
}

TEST_F(AlternativeParserTest, AlternativeSkipped1) {
  auto parser = Any(Skip(Some(Char('A'))), Skip(Some(Char('B'))));
  using Expected = decltype(parser)::ValueType;

  TestParserSuccess("ABBBBBC", parser, Expected{}, 1, CheckNotEmpty('B'));
}

TEST_F(AlternativeParserTest, AlternativeSkipped2) {
  auto parser = Any(Skip(Some(Char('A'))), Skip(Some(Char('B'))));
  using Expected = decltype(parser)::ValueType;

  TestParserSuccess("BBBBBAC", parser, Expected{}, 5, CheckNotEmpty('A'));
}
