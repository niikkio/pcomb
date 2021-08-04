#include <gtest/gtest.h>

#include <list>
#include <tuple>

#include "common.h"

#include "pcomb/strict_sequence.h"
#include "pcomb/predicate.h"

class StrictSequenceParserTest : public ::testing::Test {
 protected:
  using Ch = pcomb::PredicateParser<char>;

  using S1 = pcomb::StrictSequenceParser<Ch>;
  using S2 = pcomb::StrictSequenceParser<Ch, Ch>;
  using S3 = pcomb::StrictSequenceParser<Ch, Ch, Ch>;

  using Stream = pcomb::MockStream;

  using Expected1 = std::tuple<char>;
  using Expected2 = std::tuple<char, char>;
  using Expected3 = std::tuple<char, char, char>;

  Ch pa = Ch([](char c) { return c == 'A'; });
  Ch pb = Ch([](char c) { return c == 'B'; });
  Ch pc = Ch([](char c) { return c == 'C'; });

  S1 s1 = S1(pa);
  S3 s3 = S3(pa, pb, pc);
};

TEST_F(StrictSequenceParserTest, Seq1Match) {
  TestParserSuccess("A", s1, Expected1{'A'}, 1, CheckEmpty());
}

TEST_F(StrictSequenceParserTest, Seq1NotMatch) {
  TestParserFail("B", s1);
}

TEST_F(StrictSequenceParserTest, Seq3Match) {
  TestParserSuccess("ABC", s3, Expected3{'A', 'B', 'C'}, 3, CheckEmpty());
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch1) {
  TestParserFail("", s3);
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch2) {
  TestParserFail("A", s3);
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch3) {
  TestParserFail("AB", s3);
}

TEST_F(StrictSequenceParserTest, Seq3NotMatch4) {
  TestParserFail("ABB", s3);
}
