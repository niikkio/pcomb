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
  using Expected1 = std::tuple<char>;
  using Expected2 = std::tuple<char, char>;
  using Expected3 = std::tuple<char, char, char>;
};

TEST_F(StrictSequenceParserTest, Seq1Match) {
  TestParserSuccess("A", StrictSeq(Char('A')), Expected1{'A'}, 1, CheckEmpty());
}

TEST_F(StrictSequenceParserTest, Seq1NotMatch) {
  TestParserFail("B", StrictSeq(Char('A')));
}

TEST_F(StrictSequenceParserTest, Seq3Match) {
  TestParserSuccess("ABC", StrictSeq(Char('A'), Char('B'), Char('C')),
                    Expected3{'A', 'B', 'C'}, 3,
                    CheckEmpty());
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch1) {
  TestParserFail("", StrictSeq(Char('A'), Char('B'), Char('C')));
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch2) {
  TestParserFail("A", StrictSeq(Char('A'), Char('B'), Char('C')));
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch3) {
  TestParserFail("AB", StrictSeq(Char('A'), Char('B'), Char('C')));
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch4) {
  TestParserFail("ABB", StrictSeq(Char('A'), Char('B'), Char('C')));
}
