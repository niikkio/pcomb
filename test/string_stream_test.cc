#include <gtest/gtest.h>

#include <memory>

#include "pcomb/string_stream.h"

TEST(StringStreamTest, CreateNonEmpty) {
  pcomb::StringStream s("A");
  EXPECT_TRUE(!s.empty());
}

TEST(StringStreamTest, Head) {
  pcomb::StringStream s("A");
  EXPECT_EQ(s.head(), 'A');
}

TEST(StringStreamTest, Consume1) {
  pcomb::StringStream s("AB");
  s.consume(1);
  EXPECT_TRUE(!s.empty());
  EXPECT_EQ(s.head(), 'B');
}

TEST(StringStreamTest, Consume3) {
  pcomb::StringStream s("ABC");
  s.consume(3);
  EXPECT_TRUE(s.empty());
}


TEST(StringStreamTest, Clone) {
  pcomb::StringStream s("AB");

  auto p = std::unique_ptr<pcomb::StringStream>(s.clone());
  p->consume(1);

  EXPECT_EQ(s.head(), 'A');
  EXPECT_EQ(p->head(), 'B');
}
