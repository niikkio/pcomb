#include <gtest/gtest.h>

#include <memory>

#include "pcomb/mock_stream.h"

TEST(MockStreamTest, CreateNonEmpty) {
  pcomb::MockStream s("A");
  EXPECT_TRUE(!s.empty());
}

TEST(MockStreamTest, Head) {
  pcomb::MockStream s("A");
  EXPECT_EQ(s.head(), 'A');
}

TEST(MockStreamTest, Consume1) {
  pcomb::MockStream s("AB");
  s.consume(1);
  EXPECT_TRUE(!s.empty());
  EXPECT_EQ(s.head(), 'B');
}

TEST(MockStreamTest, Consume3) {
  pcomb::MockStream s("ABC");
  s.consume(3);
  EXPECT_TRUE(s.empty());
}


TEST(MockStreamTest, Clone) {
  pcomb::MockStream s("AB");

  auto p = std::unique_ptr<pcomb::MockStream>(s.clone());
  p->consume(1);

  EXPECT_EQ(s.head(), 'A');
  EXPECT_EQ(p->head(), 'B');
}
