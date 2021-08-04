#include <gtest/gtest.h>

#include <list>
#include <tuple>

#include "common.h"

#include "pcomb/sequence.h"
#include "pcomb/predicate.h"

class SequenceParserTest : public ::testing::Test {
 protected:
  using Ch = pcomb::PredicateParser<char>;
  using Skip = pcomb::SkippedParser<Ch>;

  using S1 = pcomb::SequenceParser<Ch>;
  using S2 = pcomb::SequenceParser<Skip, Skip>;
  using S3 = pcomb::SequenceParser<Ch, Ch, Ch>;
  using S5 = pcomb::SequenceParser<Skip, Ch, Ch, Ch, Skip>;

  using Stream = pcomb::MockStream;

  using Expected1 = char;
  using Expected2 = pcomb::SkippedValue;
  using Expected3 = std::tuple<char, char, char>;
  using Expected5 = Expected3;

  Ch pa = Ch([](char c) { return c == 'A'; });
  Ch pb = Ch([](char c) { return c == 'B'; });
  Ch pc = Ch([](char c) { return c == 'C'; });

  Skip pob = Skip(Ch([](char c) { return c == '('; }));
  Skip pcb = Skip(Ch([](char c) { return c == ')'; }));

  S1 s1 = S1(pa);
  S2 s2 = S2(Skip(pa), Skip(pb));
  S3 s3 = S3(pa, pb, pc);
  S5 s5 = S5(pob, pa, pb, pc, pcb);
};

TEST_F(SequenceParserTest, Seq1Match) {
  TestParserSuccess("A", s1, Expected1{'A'}, 1, CheckEmpty());
}

TEST_F(SequenceParserTest, Seq1NotMatch) {
  TestParserFail("B", s1);
}

TEST_F(SequenceParserTest, Seq3Match) {
  TestParserSuccess("ABC", s3, Expected3{'A', 'B', 'C'}, 3, CheckEmpty());
}

TEST_F(SequenceParserTest, Seq3NotMatch1) {
  TestParserFail("", s3);
}

TEST_F(SequenceParserTest, Seq3NotMatch2) {
  TestParserFail("A", s3);
}

TEST_F(SequenceParserTest, Seq3NotMatch3) {
  TestParserFail("AB", s3);
}

TEST_F(SequenceParserTest, Seq3NotMatch4) {
  TestParserFail("ABB", s3);
}

TEST_F(SequenceParserTest, Skip2) {
  TestParserSuccess("ABC", s2, Expected2{}, 2, CheckNotEmpty('C'));
}

TEST_F(SequenceParserTest, Skip2NotEnough) {
  TestParserFail("A", s2);
}

TEST_F(SequenceParserTest, Brackets) {
  TestParserSuccess("(ABC)", s5, Expected5{'A', 'B', 'C'}, 5, CheckEmpty());
}
