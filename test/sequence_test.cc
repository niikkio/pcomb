#include <gtest/gtest.h>

#include <tuple>

#include "testing.h"

#include "pcomb/predicate.h"
#include "pcomb/sequence.h"
#include "pcomb/skipped.h"

using pcomb::Char;
using pcomb::Seq;
using pcomb::Skip;

class SequenceParserTest : public ::testing::Test {
 protected:
  static auto pA() {
    return Seq(Char('A'));
  }

  static auto expectedA() {
    return 'A';
  }

  static auto pABC() {
    return Seq(Char('A'), Char('B'), Char('C'));
  }

  static auto expectedABC() {
    return std::tuple<char, char, char>('A', 'B', 'C');
  }

  static auto pSkippedASkippedB() {
    return Seq(Skip(Char('A')), Skip(Char('B')));
  }

  static auto expectedSkippedASkippedB() {
    return decltype(pSkippedASkippedB())::element_type::ValueType();
  }

  static auto pABCwithSkippedBraces() {
      return Seq(
          Skip(Char('(')), Char('A'), Char('B'), Char('C'), Skip(Char(')')));
  }
};

TEST_F(SequenceParserTest, Seq1Match) {
  TestParserSuccess("A", pA(), expectedA(), 1, CheckEmpty());
}

TEST_F(SequenceParserTest, Seq1NotMatch) {
  TestParserFail("B", pA());
}

TEST_F(SequenceParserTest, Seq3Match) {
  TestParserSuccess("ABC", pABC(), expectedABC(), 3, CheckEmpty());
}

TEST_F(SequenceParserTest, Seq3NotMatch1) {
  TestParserFail("", pABC());
}

TEST_F(SequenceParserTest, Seq3NotMatch2) {
  TestParserFail("A", pABC());
}

TEST_F(SequenceParserTest, Seq3NotMatch3) {
  TestParserFail("AB", pABC());
}

TEST_F(SequenceParserTest, Seq3NotMatch4) {
  TestParserFail("ABB", pABC());
}

TEST_F(SequenceParserTest, Skip2) {
  TestParserSuccess("ABC", pSkippedASkippedB(),
                    expectedSkippedASkippedB(), 2,
                    CheckNotEmpty('C'));
}

TEST_F(SequenceParserTest, Skip2NotEnough) {
  TestParserFail("A", pSkippedASkippedB());
}

TEST_F(SequenceParserTest, Brackets) {
  TestParserSuccess("(ABC)", pABCwithSkippedBraces(),
                    expectedABC(), 5, CheckEmpty());
}
