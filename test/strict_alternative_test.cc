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

class StrictAlternativeParserTest : public ::testing::Test {
 protected:
  static auto pA() {
    return StrictAny(Char('A'));
  }

  static auto expectedA() {
    return std::variant<char>{std::in_place_index<0>, 'A'};
  }

  static auto pABC() {
    return StrictAny(Char('A'), Char('B'), Char('C'));
  }

  static auto expectedABC_A() {
    return std::variant<char, char, char>{std::in_place_index<0>, 'A'};
  }

  static auto expectedABC_B() {
    return std::variant<char, char, char>{std::in_place_index<1>, 'B'};
  }

  static auto expectedABC_C() {
    return std::variant<char, char, char>{std::in_place_index<2>, 'C'};
  }

  static auto pABs() {
    return StrictAny(Char('A'), Some(Char('B')));
  }

  static auto expectedABs_A() {
    return std::variant<char, std::list<char>>{std::in_place_index<0>, 'A'};
  }

  static auto expectedABs_Bs(size_t n) {
    return std::variant<char, std::list<char>>{
      std::in_place_index<1>, std::list<char>(n, 'B')};
  }

  static auto pManyAsBs() {
    return Many(StrictAny(Some(Char('A')), Some(Char('B'))));
  }
};

TEST_F(StrictAlternativeParserTest, SingleMatch) {
  TestParserSuccess("A", pA(), expectedA(), 1, CheckEmpty());
}

TEST_F(StrictAlternativeParserTest, SingleNotMatch) {
  TestParserFail("B", pA());
}

TEST_F(StrictAlternativeParserTest, Take1) {
  TestParserSuccess("ABC", pABC(), expectedABC_A(), 1, CheckNotEmpty('B'));
}

TEST_F(StrictAlternativeParserTest, Take2) {
  TestParserSuccess("BCA", pABC(), expectedABC_B(), 1, CheckNotEmpty('C'));
}

TEST_F(StrictAlternativeParserTest, Take3) {
  TestParserSuccess("CAB", pABC(), expectedABC_C(), 1, CheckNotEmpty('A'));
}

TEST_F(StrictAlternativeParserTest, TakeOneOrMany1) {
  TestParserSuccess("ABBB", pABs(), expectedABs_A(), 1, CheckNotEmpty('B'));
}

TEST_F(StrictAlternativeParserTest, TakeOneOrMany2) {
  TestParserSuccess("BBBA", pABs(), expectedABs_Bs(3), 3, CheckNotEmpty('A'));
}

TEST_F(StrictAlternativeParserTest, ManyAlternatives) {
  using L = std::list<char>;
  using V = std::variant<L, L>;
  using E = std::list<V>;

  auto expected = E{
    V{std::in_place_index<1>, L{'B', 'B'}},
    V{std::in_place_index<0>, L{'A', 'A', 'A'}},
    V{std::in_place_index<1>, L{'B'}},
    V{std::in_place_index<0>, L{'A', 'A'}},
    V{std::in_place_index<1>, L{'B', 'B', 'B'}}
  };

  TestContainerParserSuccess(
      "BBAAABAABBB", pManyAsBs(), expected, 11, CheckEmpty());
}
