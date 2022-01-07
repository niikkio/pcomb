#include <gtest/gtest.h>

#include "pcomb/string_stream.h"

using pcomb::StringStream;

TEST(StringStreamTest, CreateNonEmpty) {
  StringStream s("A");
  EXPECT_TRUE(!s.empty());
}

TEST(StringStreamTest, Head) {
  StringStream s("A");
  EXPECT_EQ(s.head(), 'A');
}

TEST(StringStreamTest, Consume1) {
  StringStream s("AB");
  s.consume(1);
  EXPECT_TRUE(!s.empty());
  EXPECT_EQ(s.head(), 'B');
}

TEST(StringStreamTest, Consume3) {
  StringStream s("ABC");
  s.consume(3);
  EXPECT_TRUE(s.empty());
}

TEST(StringStreamTest, Clone) {
  StringStream s("AB");

  auto p = s.clone();
  p->consume(1);

  EXPECT_EQ(s.head(), 'A');
  EXPECT_EQ(p->head(), 'B');
}
