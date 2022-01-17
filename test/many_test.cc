#include <gtest/gtest.h>

#include <list>
#include <sstream>

#include "testing.h"

#include "pcomb/many.h"
#include "pcomb/messages.h"
#include "pcomb/predicate.h"

class ManyParserTest : public ::testing::Test { };

TEST_F(ManyParserTest, Name1) {
  auto parser = pcomb::Many(pcomb::Char('A'));
  TestParserName(parser, "Many <Many [0..] [Predicate]>");
}

TEST_F(ManyParserTest, Name2) {
  auto parser = pcomb::Some(pcomb::Char('A'));
  TestParserName(parser, "Some <Many [1..] [Predicate]>");
}

TEST_F(ManyParserTest, Name3) {
  auto parser = pcomb::Repeat(pcomb::Char('A'), 3);
  TestParserName(parser, "Repeat(3) <Many [3..3] [Predicate]>");
}

TEST_F(ManyParserTest, ManyEmpty) {
  auto input = "";
  auto parser = pcomb::Many(pcomb::Char('A'));
  auto expected = std::list<char>{};
  TestContainerParserSuccess(input, parser, expected, 0, CheckEmpty());
}

TEST_F(ManyParserTest, ManyNoOne) {
  auto input = "BBB";
  auto parser = pcomb::Many(pcomb::Char('A'));
  auto expected = std::list<char>{};
  TestContainerParserSuccess(input, parser, expected, 0, CheckNotEmpty('B'));
}

TEST_F(ManyParserTest, ManySome) {
  auto input = "AAAB";
  auto parser = pcomb::Many(pcomb::Char('A'));
  auto expected = std::list<char>(3, 'A');
  TestContainerParserSuccess(input, parser, expected, 3, CheckNotEmpty('B'));
}

TEST_F(ManyParserTest, ManyAll) {
  auto input = "AAAAA";
  auto parser = pcomb::Many(pcomb::Char('A'));
  auto expected = std::list<char>(5, 'A');
  TestContainerParserSuccess(input, parser, expected, 5, CheckEmpty());
}

TEST_F(ManyParserTest, SomeEmpty) {
  auto input = "";
  auto parser = pcomb::Some(pcomb::Char('A'));

  auto message = pcomb::messages::NOT_ENOUGH_RESULTS(0, 1);
  auto trace = MakeTrace(parser, {0, 0, 0}, message);

  TestContainerParserFail(input, parser, trace);
}

TEST_F(ManyParserTest, SomeNoOne) {
  auto input = "BBB";
  auto parser = pcomb::Some(pcomb::Char('A'));

  auto message = pcomb::messages::NOT_ENOUGH_RESULTS(0, 1);
  auto trace = MakeTrace(parser, {0, 0, 0}, message);

  TestContainerParserFail(input, parser, trace);
}

TEST_F(ManyParserTest, SomeOne) {
  auto input = "AB";
  auto parser = pcomb::Some(pcomb::Char('A'));
  auto expected = std::list<char>(1, 'A');
  TestContainerParserSuccess(input, parser, expected, 1, CheckNotEmpty('B'));
}

TEST_F(ManyParserTest, SomeSome) {
  auto input = "AAAB";
  auto parser = pcomb::Some(pcomb::Char('A'));
  auto expected = std::list<char>(3, 'A');
  TestContainerParserSuccess(input, parser, expected, 3, CheckNotEmpty('B'));
}

TEST_F(ManyParserTest, SomeAll) {
  auto input = "AAAAA";
  auto parser = pcomb::Some(pcomb::Char('A'));
  auto expected = std::list<char>(5, 'A');
  TestContainerParserSuccess(input, parser, expected, 5, CheckEmpty());
}

TEST_F(ManyParserTest, RepeatEmpty) {
  auto input = "";
  auto parser = pcomb::Repeat(pcomb::Char('A'), 3);

  auto message = pcomb::messages::NOT_ENOUGH_RESULTS(0, 3);
  auto trace = MakeTrace(parser, {0, 0, 0}, message);

  TestContainerParserFail(input, parser, trace);
}

TEST_F(ManyParserTest, RepeatNoOne) {
  auto input = "B";
  auto parser = pcomb::Repeat(pcomb::Char('A'), 3);

  auto message = pcomb::messages::NOT_ENOUGH_RESULTS(0, 3);
  auto trace = MakeTrace(parser, {0, 0, 0}, message);

  TestContainerParserFail(input, parser, trace);
}

TEST_F(ManyParserTest, RepeatNotEnough1) {
  auto input = "AB";
  auto parser = pcomb::Repeat(pcomb::Char('A'), 3);

  auto message = pcomb::messages::NOT_ENOUGH_RESULTS(1, 3);
  auto trace = MakeTrace(parser, {0, 0, 0}, message);

  TestContainerParserFail(input, parser, trace);
}

TEST_F(ManyParserTest, RepeatNotEnough2) {
  auto input = "AAB";
  auto parser = pcomb::Repeat(pcomb::Char('A'), 3);

  auto message = pcomb::messages::NOT_ENOUGH_RESULTS(2, 3);
  auto trace = MakeTrace(parser, {0, 0, 0}, message);

  TestContainerParserFail(input, parser, trace);
}

TEST_F(ManyParserTest, RepeatEnough) {
  auto input = "AAA";
  auto parser = pcomb::Repeat(pcomb::Char('A'), 3);
  auto expected = std::list<char>(3, 'A');
  TestContainerParserSuccess(input, parser, expected, 3, CheckEmpty());
}

TEST_F(ManyParserTest, RepeatMore) {
  auto input = "AAAAA";
  auto parser = pcomb::Repeat(pcomb::Char('A'), 3);
  auto expected = std::list<char>(3, 'A');
  TestContainerParserSuccess(input, parser, expected, 3, CheckNotEmpty('A'));
}
