#include <gtest/gtest.h>

#include "testing.h"
#include "transformers.h"

#include "pcomb/adaptive.h"
#include "pcomb/alternative.h"
#include "pcomb/lexeme.h"
#include "pcomb/many.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"

using pcomb::Adapted;
using pcomb::Char;
using pcomb::Digit;
using pcomb::Many;
using pcomb::StrictAny;
using pcomb::StrictSeq;

class AdaptiveParserTest : public ::testing::Test {
 protected:
  static auto pManyAB() {
    return Adapted(
        Many(Adapted(StrictAny(Char('A'), Char('B')), &variant2char)),
        &list2string);
  }

  static auto pDigit() {
    return Adapted(Digit(), &char2int);
  }

  static auto pTwoDigits() {
    return Adapted(StrictSeq(Digit(), Digit()), &chars2int);
  }
};

TEST_F(AdaptiveParserTest, HeadMatch) {
  TestParserSuccess("8", pDigit(), 8, 1, CheckEmpty());
}

TEST_F(AdaptiveParserTest, HeadNotMatch) {
  TestParserFail("A", pDigit());
}

TEST_F(AdaptiveParserTest, AdaptedManyAlternative) {
  TestParserSuccess("ABBAABABC", pManyAB(), "ABBAABAB", 8, CheckNotEmpty('C'));
}

TEST_F(AdaptiveParserTest, TwoDigitsToNumber1) {
  TestParserSuccess("42", pTwoDigits(), 42, 2, CheckEmpty());
}

TEST_F(AdaptiveParserTest, TwoDigitsToNumber2) {
  TestParserSuccess("131", pTwoDigits(), 13, 2, CheckNotEmpty('1'));
}

TEST_F(AdaptiveParserTest, Memory) {
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
