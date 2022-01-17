#include <gtest/gtest.h>

#include <sstream>

#include "testing.h"
#include "transformers.h"

#include "pcomb/adaptive.h"
#include "pcomb/alternative.h"
#include "pcomb/lexeme.h"
#include "pcomb/many.h"
#include "pcomb/messages.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"

class AdaptiveParserTest : public ::testing::Test { };

TEST_F(AdaptiveParserTest, Name1) {
  using pcomb::Adapted, pcomb::Many, pcomb::StrictAny, pcomb::Char;
  auto parser = Adapted(
      Many(Adapted(StrictAny(Char('A'), Char('B')), &variant2char)),
      &list2string);
  auto expected_name = "Adapted <Adaptive [Many [0..] [Adaptive ["
                       "Strict Alternative [Predicate, Predicate]]]]>";
  TestParserName(parser, expected_name);
}

TEST_F(AdaptiveParserTest, Name2) {
  auto parser = pcomb::Adapted(pcomb::Digit(), &char2int);
  auto expected_name = "Adapted <Adaptive [Predicate]>";
  TestParserName(parser, expected_name);
}

TEST_F(AdaptiveParserTest, Name3) {
  using pcomb::Adapted, pcomb::StrictSeq, pcomb::Digit;
  auto parser = Adapted(StrictSeq(Digit(), Digit()), &chars2int);
  auto expected_name =
      "Adapted <Adaptive [Strict Sequence [Predicate, Predicate]]>";
  TestParserName(parser, expected_name);
}

TEST_F(AdaptiveParserTest, HeadMatch) {
  auto input = "8";
  auto parser = pcomb::Adapted(pcomb::Digit(), &char2int);
  TestParserSuccess(input, parser, 8, 1, CheckEmpty());
}

TEST_F(AdaptiveParserTest, HeadNotMatch) {
  auto input = "A";
  auto parser = pcomb::Adapted(pcomb::Digit(), &char2int);

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('A');
  trace << "\t" << MakeTrace(pcomb::Digit(), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(AdaptiveParserTest, AdaptedManyAlternative) {
  using pcomb::Adapted, pcomb::Many, pcomb::StrictAny, pcomb::Char;
  auto input = "ABBAABABC";
  auto parser = Adapted(
      Many(Adapted(StrictAny(Char('A'), Char('B')), &variant2char)),
      &list2string);
  TestParserSuccess(input, parser, "ABBAABAB", 8, CheckNotEmpty('C'));
}

TEST_F(AdaptiveParserTest, TwoDigitsToNumber1) {
  using pcomb::Adapted, pcomb::StrictSeq, pcomb::Digit;
  auto input = "42";
  auto parser = Adapted(StrictSeq(Digit(), Digit()), &chars2int);
  TestParserSuccess(input, parser, 42, 2, CheckEmpty());
}

TEST_F(AdaptiveParserTest, TwoDigitsToNumber2) {
  using pcomb::Adapted, pcomb::StrictSeq, pcomb::Digit;
  auto input = "131";
  auto parser = Adapted(StrictSeq(Digit(), Digit()), &chars2int);
  TestParserSuccess(input, parser, 13, 2, CheckNotEmpty('1'));
}

TEST_F(AdaptiveParserTest, Memory) {
  using pcomb::Adapted, pcomb::Digit;

  static int n = 0;
  static int n_copy = 0;
  static int n_move = 0;

  struct A {
    A() { ++n; }

    A(const A&) { ++n_copy; }
    A& operator=(const A&) { ++n_copy; return *this; }

    A(A&&) { ++n_move; }
    A& operator=(A&&) { ++n_move; return *this; }

    bool operator==(const A&) const {
      return true;
    }

    static A process_char(char) {
      return A();
    }

    static A process_other(A&& other) {
      return A(std::forward<A>(other));
    }
  };

  auto parser = Adapted(Adapted(Digit(), &A::process_char), &A::process_other);
  TestParserSuccess("0", parser, A(), 1, CheckEmpty());

  EXPECT_EQ(2, n);
  EXPECT_EQ(0, n_copy);
  EXPECT_EQ(3, n_move);
}
