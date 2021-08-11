#include <gtest/gtest.h>

#include "common.h"

#include "pcomb/end.h"

using pcomb::End;

class EndParserTest : public ::testing::Test { };

TEST_F(EndParserTest, NotEmpty) {
  TestParserFail("A", End<char>());
}

TEST_F(EndParserTest, Empty) {
  using Expected = decltype(End<char>())::ValueType;
  TestParserSuccess("", End<char>(), Expected{}, 0, CheckEmpty());
}
