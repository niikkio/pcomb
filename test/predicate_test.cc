#include <gtest/gtest.h>

#include "pcomb/mock_stream.h"
#include "pcomb/predicate.h"

TEST(PredicateParserTest, Empty) {
    using namespace pcomb;
    MockStream s("");
    auto p = PredicateParser<char>([](char c) { return c == 'A'; });
    auto res = p.parse(&s);
    EXPECT_FALSE(res.success());
}

TEST(PredicateParserTest, HeadMatch) {
    using namespace pcomb;
    MockStream s("AB");
    auto p = PredicateParser<char>([](char c) { return c == 'A'; });
    auto res = p.parse(&s);
    EXPECT_TRUE(res.success());
    EXPECT_EQ(res.get_value(), 'A');
    EXPECT_EQ(res.get_consumed_number(), 1);
    EXPECT_EQ(s.head(), 'B');

    char c(std::move(res).get_value());
    EXPECT_EQ(c, 'A');
}

TEST(PredicateParserTest, HeadNotMatch) {
    using namespace pcomb;
    MockStream s("AB");
    auto p = PredicateParser<char>([](char c) { return c == 'B'; });
    auto res = p.parse(&s);
    EXPECT_FALSE(res.success());
    EXPECT_EQ(res.get_consumed_number(), 0);
    EXPECT_EQ(s.head(), 'A');
}
