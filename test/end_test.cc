#include <gtest/gtest.h>

#include "testing.h"

#include "pcomb/end.h"

using pcomb::End;

class EndParserTest : public ::testing::Test {
 protected:
  static auto pEnd() {
    return End<char>();
  }

  static auto expectedEnd() {
    return decltype(pEnd())::ValueType();
  }
};

TEST_F(EndParserTest, NotEmpty) {
  TestParserFail("A", pEnd());
}

TEST_F(EndParserTest, Empty) {
  TestParserSuccess("", pEnd(), expectedEnd(), 0, CheckEmpty());
}
