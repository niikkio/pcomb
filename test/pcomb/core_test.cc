#include <gtest/gtest.h>

#include <pcomb/core.h>

TEST(CoreTest, FooAssertion) {
    EXPECT_EQ(13, foo());
}
