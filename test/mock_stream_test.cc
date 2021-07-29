#include <gtest/gtest.h>

#include <memory>

#include "pcomb/mock_stream.h"

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
