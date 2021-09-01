#include <gtest/gtest.h>

#include "common.h"

#include "pcomb/lexeme.h"
#include "pcomb/predicate.h"

using pcomb::Char;
using pcomb::Digit;
using pcomb::Inside;
using pcomb::NewLine;
using pcomb::Space;
using pcomb::String;

class LexemeParserTest : public ::testing::Test {
 protected:
  using Expected = std::list<char>;
};

TEST_F(LexemeParserTest, DigitFromEmpty) {
  TestParserFail("", Digit());
}

TEST_F(LexemeParserTest, DigitMatch) {
  TestParserSuccess("1B", Digit(), '1', 1, CheckNotEmpty('B'));
}

TEST_F(LexemeParserTest, DigitNotMatch) {
  TestParserFail("B1", Digit());
}

TEST_F(LexemeParserTest, InsideBracketsMatch) {
  TestParserSuccess("(A)", Inside('(', Char('A'), ')'), 'A', 3, CheckEmpty());
}

TEST_F(LexemeParserTest, InsideBracketsNoOpen) {
  TestParserFail("A)B", Inside('(', Char('A'), ')'));
}

TEST_F(LexemeParserTest, InsideBracketsNoClose) {
  TestParserFail("(AB", Inside('(', Char('A'), ')'));
}

TEST_F(LexemeParserTest, SpaceFromEmpty) {
  TestParserFail("", Space());
}

TEST_F(LexemeParserTest, SpaceNotMatch) {
  TestParserFail("A", Space());
}

TEST_F(LexemeParserTest, SpaceMatchSpace) {
  TestParserSuccess(" A", Space(), ' ', 1, CheckNotEmpty('A'));
}

TEST_F(LexemeParserTest, SpaceMatchTab) {
  TestParserSuccess("\tA", Space(), '\t', 1, CheckNotEmpty('A'));
}

TEST_F(LexemeParserTest, SpaceMatchNewLine) {
  TestParserSuccess("\nA", Space(), '\n', 1, CheckNotEmpty('A'));
}

TEST_F(LexemeParserTest, NewLineFromEmpty) {
  TestParserFail("", NewLine());
}

TEST_F(LexemeParserTest, NewLineNotMatch) {
  TestParserFail("A", NewLine());
}

TEST_F(LexemeParserTest, NewLineMatch) {
  TestParserSuccess("\nA", NewLine(), '\n', 1, CheckNotEmpty('A'));
}

TEST_F(LexemeParserTest, StringFromEmpty) {
  TestContainerParserFail("", String("ABC"));
}

TEST_F(LexemeParserTest, EmptyStringFromEmpty) {
  TestContainerParserSuccess("", String(""), Expected{}, 0, CheckEmpty());
}

TEST_F(LexemeParserTest, EmptyString) {
  TestContainerParserSuccess("AB", String(""),
                             Expected{}, 0, CheckNotEmpty('A'));
}

TEST_F(LexemeParserTest, StringMatch) {
  TestContainerParserSuccess("ABC", String("ABC"),
                             Expected{'A', 'B', 'C'}, 3, CheckEmpty());
}

TEST_F(LexemeParserTest, StringNotMatch) {
  TestContainerParserFail("ADC", String("ABC"));
}
