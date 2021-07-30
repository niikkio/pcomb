#include <gtest/gtest.h>

#include "common.h"

#include "pcomb/predicate.h"

class PredicateParserTest : public ::testing::Test {
protected:
    using Ch = pcomb::PredicateParser<char>;

    Ch pa = Ch([](char c) { return c == 'A'; });
};

TEST_F(PredicateParserTest, Empty) {
    TestParserFail("", pa);
}

TEST_F(PredicateParserTest, HeadMatch) {
    TestParserSuccess("AB", pa, 'A', CheckNotEmpty('B'), 1);
}

TEST_F(PredicateParserTest, HeadNotMatch) {
    TestParserFail("BA", pa);
}
