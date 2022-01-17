#include <gtest/gtest.h>

#include <sstream>

#include "testing.h"

#include "pcomb/end.h"
#include "pcomb/messages.h"

class EndParserTest : public ::testing::Test { };

TEST_F(EndParserTest, Name) {
  auto parser = pcomb::End<char>();
  TestParserName(parser, "End <End>");
}

TEST_F(EndParserTest, NotEmpty) {
  auto input = "A";
  auto parser = pcomb::End<char>();
  auto message = pcomb::messages::END_WAS_EXPECTED('A', {0, 0, 0});
  auto trace = MakeTrace(parser, {0, 0, 0}, message);
  TestParserFail(input, parser, trace);
}

TEST_F(EndParserTest, Empty) {
  auto input = "";
  auto parser = pcomb::End<char>();
  auto expected = decltype(parser)::element_type::ValueType();
  TestParserSuccess(input, parser, expected, 0, CheckEmpty());
}
