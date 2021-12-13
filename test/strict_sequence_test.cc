#include <gtest/gtest.h>

#include <list>
#include <tuple>

#include "testing.h"

#include "pcomb/predicate.h"
#include "pcomb/sequence.h"

using pcomb::Char;
using pcomb::StrictSeq;

class StrictSequenceParserTest : public ::testing::Test {
 protected:
  static auto pA() {
    return StrictSeq(Char('A'));
  }

  static auto expectedA() {
    return std::tuple<char>{'A'};
  }

  static auto pABC() {
    return StrictSeq(Char('A'), Char('B'), Char('C'));
  }

  static auto expectedABC() {
    return std::tuple<char, char, char>('A', 'B', 'C');
  }
};

TEST_F(StrictSequenceParserTest, Seq1Match) {
  TestParserSuccess("A", pA(), expectedA(), 1, CheckEmpty());
}

TEST_F(StrictSequenceParserTest, Seq1NotMatch) {
  TestParserFail("B", pA());
}

TEST_F(StrictSequenceParserTest, Seq3Match) {
  TestParserSuccess("ABC", pABC(), expectedABC(), 3, CheckEmpty());
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch1) {
  TestParserFail("", pABC());
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch2) {
  TestParserFail("A", pABC());
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch3) {
  TestParserFail("AB", pABC());
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch4) {
  TestParserFail("ABB", pABC());
}
