#include <gtest/gtest.h>

#include <memory>

#include "pcomb/mock_stream.h"
#include "pcomb/predicate.h"

TEST(MockStreamTest, CreateNonEmpty) {
    using namespace pcomb;
    MockStream s("A");
    EXPECT_TRUE(!s.empty());
}

TEST(MockStreamTest, Head) {
    using namespace pcomb;
    MockStream s("A");
    EXPECT_EQ(s.head(), 'A');
}

TEST(MockStreamTest, Consume1) {
    using namespace pcomb;
    MockStream s("AB");
    s.consume(1);
    EXPECT_TRUE(!s.empty());
    EXPECT_EQ(s.head(), 'B');
}

TEST(MockStreamTest, Consume3) {
    using namespace pcomb;
    MockStream s("ABC");
    s.consume(3);
    EXPECT_TRUE(s.empty());
}


TEST(MockStreamTest, Clone) {
    using namespace pcomb;
    MockStream s("AB");

    auto p = std::unique_ptr<MockStream>(s.clone());
    p->consume(1);

    EXPECT_EQ(s.head(), 'A');
    EXPECT_EQ(p->head(), 'B');
}

TEST(PredicateParserTest, HeadMatch) {
    using namespace pcomb;
    MockStream s("AB");
    auto p = PredicateParser<char>([](char c) { return c == 'A'; });
    auto res = p.parse(&s);
    EXPECT_TRUE(res.success());
    EXPECT_EQ(res.get_value(), 'A');
    EXPECT_EQ(res.get_consumed(), 1);
    EXPECT_EQ(s.head(), 'B');
}

TEST(PredicateParserTest, HeadNotMatch) {
    using namespace pcomb;
    MockStream s("AB");
    auto p = PredicateParser<char>([](char c) { return c == 'B'; });
    auto res = p.parse(&s);
    EXPECT_FALSE(res.success());
    EXPECT_EQ(res.get_consumed(), 0);
    EXPECT_EQ(s.head(), 'A');
}
