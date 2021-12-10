#include <gtest/gtest.h>

#include "testing.h"

#include "pcomb/lexeme.h"
#include "pcomb/predicate.h"

using pcomb::Char;
using pcomb::Digit;
using pcomb::Inside;
using pcomb::Line;
using pcomb::NewLine;
using pcomb::Space;
using pcomb::String;
using pcomb::Word;

class LexemeParserTest : public ::testing::Test {
 protected:
  static auto pDigit() {
    return Digit();
  }

  static auto pAinBrackets() {
    return Inside('(', Char('A'), ')');
  }

  static auto pSpace() {
    return Space();
  }

  static auto pLine() {
    return Line();
  }

  static auto pNewLine() {
    return NewLine();
  }

  static auto pEmptyString() {
    return String("");
  }

  static auto pStringABC() {
    return String("ABC");
  }

  static auto pWord() {
    return Word();
  }

  using Expected = std::list<char>;
};

TEST_F(LexemeParserTest, DigitFromEmpty) {
  TestParserFail("", pDigit());
}

TEST_F(LexemeParserTest, DigitMatch) {
  TestParserSuccess("1B", pDigit(), '1', 1, CheckNotEmpty('B'));
}

TEST_F(LexemeParserTest, DigitNotMatch) {
  TestParserFail("B1", pDigit());
}

TEST_F(LexemeParserTest, InsideBracketsMatch) {
  TestParserSuccess("(A)", pAinBrackets(), 'A', 3, CheckEmpty());
}

TEST_F(LexemeParserTest, InsideBracketsNoOpen) {
  TestParserFail("A)B", pAinBrackets());
}

TEST_F(LexemeParserTest, InsideBracketsNoClose) {
  TestParserFail("(AB", pAinBrackets());
}

TEST_F(LexemeParserTest, SpaceFromEmpty) {
  TestParserFail("", pSpace());
}

TEST_F(LexemeParserTest, SpaceNotMatch) {
  TestParserFail("A", pSpace());
}

TEST_F(LexemeParserTest, SpaceMatchSpace) {
  TestParserSuccess(" A", pSpace(), ' ', 1, CheckNotEmpty('A'));
}

TEST_F(LexemeParserTest, SpaceMatchTab) {
  TestParserSuccess("\tA", pSpace(), '\t', 1, CheckNotEmpty('A'));
}

TEST_F(LexemeParserTest, SpaceMatchNewLine) {
  TestParserSuccess("\nA", pSpace(), '\n', 1, CheckNotEmpty('A'));
}

TEST_F(LexemeParserTest, NewLineFromEmpty) {
  TestParserFail("", pNewLine());
}

TEST_F(LexemeParserTest, NewLineNotMatch) {
  TestParserFail("A", pNewLine());
}

TEST_F(LexemeParserTest, NewLineMatch) {
  TestParserSuccess("\nA", pNewLine(), '\n', 1, CheckNotEmpty('A'));
}

TEST_F(LexemeParserTest, StringFromEmpty) {
  TestParserFail("", pStringABC());
}

TEST_F(LexemeParserTest, EmptyStringFromEmpty) {
  TestParserSuccess("", pEmptyString(), "", 0, CheckEmpty());
}

TEST_F(LexemeParserTest, EmptyString) {
  TestParserSuccess("AB", pEmptyString(), "", 0, CheckNotEmpty('A'));
}

TEST_F(LexemeParserTest, StringMatch) {
  TestParserSuccess("ABC", pStringABC(), "ABC", 3, CheckEmpty());
}

TEST_F(LexemeParserTest, StringNotMatch) {
  TestParserFail("ADC", pStringABC());
}

TEST_F(LexemeParserTest, LineFromEmpty) {
  TestContainerParserSuccess("", pLine(), Expected{}, 0, CheckEmpty());
}

TEST_F(LexemeParserTest, LineFromOneLineWithNewLine) {
  TestContainerParserSuccess("AB\n", pLine(),
                             Expected{'A', 'B'}, 2, CheckNotEmpty('\n'));
}

TEST_F(LexemeParserTest, LineFromOneLineWithoutNewLine) {
  TestContainerParserSuccess("AB", pLine(),
                             Expected{'A', 'B'}, 2, CheckEmpty());
}

TEST_F(LexemeParserTest, LineFromManyLines) {
  TestContainerParserSuccess("AB\nCD", pLine(),
                             Expected{'A', 'B'}, 2, CheckNotEmpty('\n'));
}

TEST_F(LexemeParserTest, WordFromEmpty) {
  TestContainerParserSuccess("", pWord(), Expected{}, 0, CheckEmpty());
}

TEST_F(LexemeParserTest, WordFromOneWordWithoutSpace) {
  TestContainerParserSuccess("AB", pWord(),
                             Expected{'A', 'B'}, 2, CheckEmpty());
}

TEST_F(LexemeParserTest, WordFromOneWordWithSpace) {
  TestContainerParserSuccess("AB ", pWord(),
                             Expected{'A', 'B'}, 2, CheckNotEmpty(' '));
}

TEST_F(LexemeParserTest, WordFromOneWordWithNewLine) {
  TestContainerParserSuccess("AB\n", pWord(),
                             Expected{'A', 'B'}, 2, CheckNotEmpty('\n'));
}

TEST_F(LexemeParserTest, WordFromOneWordWithTab) {
  TestContainerParserSuccess("AB\t", pWord(),
                             Expected{'A', 'B'}, 2, CheckNotEmpty('\t'));
}
