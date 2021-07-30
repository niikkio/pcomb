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

    using Expected2 = std::variant<char, std::list<char> >;
    using Expected3 = std::variant<char, char, char>;

    Ch pa = Ch([](char c) { return c == 'A'; });
    Ch pb = Ch([](char c) { return c == 'B'; });
    Ch pc = Ch([](char c) { return c == 'C'; });

    Many many = Many(pb);

    A1 a1 = A1(pa);
    A2 a2 = A2(pa, many);
    A3 a3 = A3(pa, pb, pc);
};

TEST_F(AlternativeParserTest, TakeFirst) {
    TestParserSuccess("BA", a3, Expected3{std::in_place_index<1>, 'B'}, CheckNotEmpty('A'), 1);
}

TEST_F(AlternativeParserTest, TakeSecond) {
    TestParserSuccess("AB", a3, Expected3{std::in_place_index<0>, 'A'}, CheckNotEmpty('B'), 1);
}

TEST_F(AlternativeParserTest, TakeOneOrMany1) {
    TestParserSuccess("ABBB", a2, Expected2{std::in_place_index<0>, 'A'}, CheckNotEmpty('B'), 1);
}

TEST_F(AlternativeParserTest, TakeOneOrMany2) {
    TestParserSuccess("BBBA", a2, Expected2{std::in_place_index<1>, std::list<char>{'B','B','B'}}, CheckNotEmpty('A'), 3);
}
