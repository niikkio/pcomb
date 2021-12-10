#include <gtest/gtest.h>

#include <list>
#include <variant>

#include "testing.h"

#include "pcomb/alternative.h"
#include "pcomb/many.h"
#include "pcomb/predicate.h"

using pcomb::Char;
using pcomb::Many;
using pcomb::Some;
using pcomb::StrictAny;

class StrictAlternativeParserTest : public ::testing::Test { };

TEST_F(StrictAlternativeParserTest, SingleMatch) {
  TestParserSuccess("A", StrictAny(Char('A')),
                    std::variant<char>{std::in_place_index<0>, 'A'}, 1,
                    CheckEmpty());
}

TEST_F(StrictAlternativeParserTest, SingleNotMatch) {
  TestParserFail("B", StrictAny(Char('A')));
}

TEST_F(StrictAlternativeParserTest, Take1) {
  using Expected = std::variant<char, char, char>;
  TestParserSuccess("ABC", StrictAny(Char('A'), Char('B'), Char('C')),
                    Expected{std::in_place_index<0>, 'A'}, 1,
                    CheckNotEmpty('B'));
}

TEST_F(StrictAlternativeParserTest, Take2) {
  using Expected = std::variant<char, char, char>;
  TestParserSuccess("BCA", StrictAny(Char('A'), Char('B'), Char('C')),
                    Expected{std::in_place_index<1>, 'B'}, 1,
                    CheckNotEmpty('C'));
}

TEST_F(StrictAlternativeParserTest, Take3) {
  using Expected = std::variant<char, char, char>;
  TestParserSuccess("CAB", StrictAny(Char('A'), Char('B'), Char('C')),
                    Expected{std::in_place_index<2>, 'C'}, 1,
                    CheckNotEmpty('A'));
}

TEST_F(StrictAlternativeParserTest, TakeOneOrMany1) {
  using Expected = std::variant<char, std::list<char>>;
  TestParserSuccess("ABBB", StrictAny(Char('A'), Some(Char('B'))),
                    Expected{std::in_place_index<0>, 'A'}, 1,
                    CheckNotEmpty('B'));
}

TEST_F(StrictAlternativeParserTest, TakeOneOrMany2) {
  using Expected = std::variant<char, std::list<char>>;
  TestParserSuccess(
      "BBBA", StrictAny(Char('A'), Some(Char('B'))),
      Expected{std::in_place_index<1>, std::list<char>{'B', 'B', 'B'}}, 3,
      CheckNotEmpty('A'));
}

TEST_F(StrictAlternativeParserTest, ManyAlternatives) {
  using L = std::list<char>;
  using V = std::variant<L, L>;
  using E = std::list<V>;

  auto parser = Many(StrictAny(Some(Char('A')), Some(Char('B'))));

  auto expected = E{
    V{std::in_place_index<1>, L{'B', 'B'}},
    V{std::in_place_index<0>, L{'A', 'A', 'A'}},
    V{std::in_place_index<1>, L{'B'}},
    V{std::in_place_index<0>, L{'A', 'A'}},
    V{std::in_place_index<1>, L{'B', 'B', 'B'}}
  };

  TestContainerParserSuccess("BBAAABAABBB", parser, expected, 11, CheckEmpty());
}
