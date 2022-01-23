#include <gtest/gtest.h>

#include "testing.h"

#include "pcomb/messages.h"
#include "pcomb/predicate.h"

class PredicateParserTest : public ::testing::Test { };

TEST_F(PredicateParserTest, Name1) {
  auto parser = pcomb::Char('A');
  TestParserName(parser, "Char('A') <Char('A')>");
}

TEST_F(PredicateParserTest, Name2) {
  auto parser = pcomb::AnyChar();
  TestParserName(parser, "AnyChar <Predicate>");
}

TEST_F(PredicateParserTest, Name3) {
  auto parser = pcomb::LatinChar();
  TestParserName(parser, "LatinChar <Predicate>");
}

TEST_F(PredicateParserTest, Empty) {
  auto input = "";
  auto parser = pcomb::Char('A');

  std::stringstream trace;
  auto message = pcomb::messages::EMPTY_STREAM;
  trace << MakeTrace(parser, {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(PredicateParserTest, HeadMatch) {
  auto input = "AB";
  auto parser = pcomb::Char('A');
  TestParserSuccess(input, parser, 'A', 1, CheckNotEmpty('B'));
}

TEST_F(PredicateParserTest, HeadNotMatch) {
  auto input = "BA";
  auto parser = pcomb::Char('A');

  std::stringstream trace;
  auto message = pcomb::messages::UNEXPECTED_CHAR('B');
  trace << MakeTrace(parser, {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(PredicateParserTest, AnyEmpty) {
  auto input = "";
  auto parser = pcomb::AnyChar();

  std::stringstream trace;
  auto message = pcomb::messages::EMPTY_STREAM;
  trace << MakeTrace(parser, {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(PredicateParserTest, AnyNotEmpty) {
  auto input = "AB";
  auto parser = pcomb::AnyChar();
  TestParserSuccess(input, parser, 'A', 1, CheckNotEmpty('B'));
}

TEST_F(PredicateParserTest, LatinEmpty) {
  auto input = "";
  auto parser = pcomb::LatinChar();

  std::stringstream trace;
  auto message = pcomb::messages::EMPTY_STREAM;
  trace << MakeTrace(parser, {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(PredicateParserTest, LatinUpperMatch) {
  auto input = "MN";
  auto parser = pcomb::LatinChar();
  TestParserSuccess(input, parser, 'M', 1, CheckNotEmpty('N'));
}

TEST_F(PredicateParserTest, LatinLowerMatch) {
  auto input = "mn";
  auto parser = pcomb::LatinChar();
  TestParserSuccess(input, parser, 'm', 1, CheckNotEmpty('n'));
}

TEST_F(PredicateParserTest, LatinHeadNotMatch) {
  auto input = "ЯБ";
  auto parser = pcomb::LatinChar();

  std::stringstream trace;
  auto message = pcomb::messages::UNEXPECTED_CHAR(input[0]);
  trace << MakeTrace(parser, {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}
