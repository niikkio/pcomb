#include <gtest/gtest.h>

#include <list>

#include "common.h"

#include "pcomb/many.h"
#include "pcomb/predicate.h"

class ManyParserTest : public ::testing::Test {
protected:
    using Ch = pcomb::PredicateParser<char>;
    using Many = pcomb::ManyParser<Ch>;
    using Expected = std::list<char>;
    using Stream = pcomb::MockStream;

    Ch pa = Ch([](char c) { return c == 'A'; });
    Many many = Many(pa);
    Many some = Many(pa, 1);
    Many exact3 = Many(pa, 3, 3);
};

TEST_F(ManyParserTest, ManyEmpty) {
    TestContainerParserSuccess("", many,
            Expected{}, CheckEmpty(), 0);
}

TEST_F(ManyParserTest, ManyNoOne) {
    TestContainerParserSuccess("BBB", many,
            Expected{}, CheckNotEmpty('B'), 0);
}

TEST_F(ManyParserTest, ManySome) {
    TestContainerParserSuccess("AAAB", many,
            Expected{'A','A','A'}, CheckNotEmpty('B'), 3);
}

TEST_F(ManyParserTest, ManyAll) {
    TestContainerParserSuccess("AAAAA", many,
            Expected{'A','A','A','A','A'}, CheckEmpty(), 5);
}

TEST_F(ManyParserTest, SomeEmpty) {
    TestContainerParserFail("", some);
}

TEST_F(ManyParserTest, SomeNoOne) {
    TestContainerParserFail("BBB", some);
}

TEST_F(ManyParserTest, SomeOne) {
    TestContainerParserSuccess("AB", some,
            Expected{'A'}, CheckNotEmpty('B'), 1);
}

TEST_F(ManyParserTest, SomeSome) {
    TestContainerParserSuccess("AAAB", some,
            Expected{'A','A','A'}, CheckNotEmpty('B'), 3);
}

TEST_F(ManyParserTest, SomeAll) {
    TestContainerParserSuccess("AAAAA", some,
            Expected{'A','A','A','A','A'}, CheckEmpty(), 5);
}

TEST_F(ManyParserTest, RepeatEmpty) {
    TestContainerParserFail("", exact3);
}

TEST_F(ManyParserTest, RepeatNoOne) {
    TestContainerParserFail("B", exact3);
}

TEST_F(ManyParserTest, RepeatNotEnough1) {
    TestContainerParserFail("A", exact3);
}

TEST_F(ManyParserTest, RepeatNotEnough2) {
    TestContainerParserFail("AAB", exact3);
}

TEST_F(ManyParserTest, RepeatEnough) {
    TestContainerParserSuccess("AAA", exact3,
            Expected{'A','A','A'}, CheckEmpty(), 3);
}

TEST_F(ManyParserTest, RepeatMore) {
    TestContainerParserSuccess("AAAA", exact3,
            Expected{'A','A','A'}, CheckNotEmpty('A'), 3);
}
