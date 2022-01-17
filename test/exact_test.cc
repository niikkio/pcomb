#include <gtest/gtest.h>

#include <sstream>

#include "testing.h"

#include "pcomb/exact.h"
#include "pcomb/messages.h"
#include "pcomb/predicate.h"

class ExactParserTest : public ::testing::Test { };

TEST_F(ExactParserTest, Name) {
  auto parser = pcomb::Exact(pcomb::Char('A'));
  TestParserName(parser, "Exact <Exact [Predicate]>");
}

TEST_F(ExactParserTest, Empty) {
  auto input = "";
  auto parser = pcomb::Exact(pcomb::Char('A'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::EMPTY_STREAM;
  trace << "\t" << MakeTrace(pcomb::Char('A'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(ExactParserTest, HeadNotMatch) {
  auto input = "B";
  auto parser = pcomb::Exact(pcomb::Char('A'));

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('B');
  trace << "\t" << MakeTrace(pcomb::Char('A'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(ExactParserTest, HeadMatchStreamEnded) {
  auto input = "A";
  auto parser = pcomb::Exact(pcomb::Char('A'));
  TestParserSuccess(input, parser, 'A', 1, CheckEmpty());
}

TEST_F(ExactParserTest, HeadMatchStreamNotEnded) {
  auto input = "AB";
  auto parser = pcomb::Exact(pcomb::Char('A'));
  auto message = pcomb::messages::END_WAS_EXPECTED('B', {1, 0, 1});
  auto trace = MakeTrace(parser, {0, 0, 0}, message);
  TestParserFail(input, parser, trace);
}

