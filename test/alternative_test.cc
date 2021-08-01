#include <gtest/gtest.h>

#include <list>
#include <variant>

#include "common.h"

#include "pcomb/alternative.h"
#include "pcomb/many.h"
#include "pcomb/predicate.h"

class AlternativeParserTest : public ::testing::Test {
 protected:
  using Ch = pcomb::PredicateParser<char>;
  using Many = pcomb::ManyParser<Ch>;

  using A1 = pcomb::AlternativeParser<Ch>;
  using A2 = pcomb::AlternativeParser<Ch, Many>;
  using A3 = pcomb::AlternativeParser<Ch, Ch, Ch>;

  using Stream = pcomb::MockStream;

  using Expected1 = std::variant<char>;
  using Expected2 = std::variant<char, std::list<char> >;
  using Expected3 = std::variant<char, char, char>;

  Ch pa = Ch([](char c) { return c == 'A'; });
  Ch pb = Ch([](char c) { return c == 'B'; });
  Ch pc = Ch([](char c) { return c == 'C'; });

  Many ma = Many(pa, 1);
  Many mb = Many(pb, 1);

  A1 a1 = A1(pa);
  A2 a2 = A2(pa, mb);
  A3 a3 = A3(pa, pb, pc);
};

TEST_F(AlternativeParserTest, SingleMatch) {
  TestParserSuccess("A", a1,
                    Expected1{std::in_place_index<0>, 'A'}, 1,
                    CheckEmpty());
}

TEST_F(AlternativeParserTest, SingleNotMatch) {
  TestParserFail("B", a1);
}

TEST_F(AlternativeParserTest, Take1) {
  TestParserSuccess("ABC", a3,
                    Expected3{std::in_place_index<0>, 'A'}, 1,
                    CheckNotEmpty('B'));
}

TEST_F(AlternativeParserTest, Take2) {
  TestParserSuccess("BCA", a3,
                    Expected3{std::in_place_index<1>, 'B'}, 1,
                    CheckNotEmpty('C'));
}

TEST_F(AlternativeParserTest, Take3) {
  TestParserSuccess("CAB", a3,
                    Expected3{std::in_place_index<2>, 'C'}, 1,
                    CheckNotEmpty('A'));
}

TEST_F(AlternativeParserTest, TakeOneOrMany1) {
  TestParserSuccess("ABBB", a2,
                    Expected2{std::in_place_index<0>, 'A'}, 1,
                    CheckNotEmpty('B'));
}

TEST_F(AlternativeParserTest, TakeOneOrMany2) {
  TestParserSuccess(
      "BBBA", a2,
      Expected2{std::in_place_index<1>, std::list<char>{'B', 'B', 'B'}}, 3,
      CheckNotEmpty('A'));
}

TEST_F(AlternativeParserTest, ManyAlternatives) {
  using A = pcomb::AlternativeParser<Many, Many>;
  using M = pcomb::ManyParser<A>;
  using L = Many::ValueType;  // list<char>
  using V = A::ValueType;     // variant<list<char>, list<char>>
  using E = M::ValueType;     // list<V>

  auto p = M(A(ma, mb));
  auto expected = E{
    V{std::in_place_index<1>, L{'B', 'B'}},
    V{std::in_place_index<0>, L{'A', 'A', 'A'}},
    V{std::in_place_index<1>, L{'B'}},
    V{std::in_place_index<0>, L{'A', 'A'}},
    V{std::in_place_index<1>, L{'B', 'B', 'B'}}
  };

  TestContainerParserSuccess("BBAAABAABBB", p, expected, 11, CheckEmpty());
}
