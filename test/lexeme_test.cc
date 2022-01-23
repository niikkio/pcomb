#include <gtest/gtest.h>

#include <list>
#include <sstream>

#include "testing.h"

#include "pcomb/chain.h"
#include "pcomb/lexeme.h"
#include "pcomb/messages.h"
#include "pcomb/predicate.h"
#include "pcomb/skipped.h"

class LexemeParserTest : public ::testing::Test { };

TEST_F(LexemeParserTest, Name1) {
  auto parser = pcomb::Digit();
  TestParserName(parser, "Digit <Predicate>");
}

TEST_F(LexemeParserTest, Name2) {
  auto parser = pcomb::Inside('(', pcomb::Char('A'), ')');
  auto expected_name = "Inside('(...)') <Sequence ["
                       "Skipped [Char('(')], Char('A'), Skipped [Char(')')]]>";
  TestParserName(parser, expected_name);
}

TEST_F(LexemeParserTest, Name3) {
  auto parser = pcomb::Space();
  TestParserName(parser, "Space <Predicate>");
}

TEST_F(LexemeParserTest, Name4) {
  auto parser = pcomb::Line();
  TestParserName(parser, "Line <Until [Char('\n')]>");
}

TEST_F(LexemeParserTest, Name5) {
  auto parser = pcomb::NewLine();
  TestParserName(parser, "NewLine <Char('\n')>");
}

TEST_F(LexemeParserTest, Name6) {
  auto parser = pcomb::String("");
  TestParserName(parser, "String('') <Adaptive [Dynamic Sequence []]>");
}

TEST_F(LexemeParserTest, Name7) {
  auto parser = pcomb::String("ABC");
  auto expected_name = "String('ABC') <Adaptive ["
                       "Dynamic Sequence [Char('A'), Char('B'), Char('C')]]>";
  TestParserName(parser, expected_name);
}

TEST_F(LexemeParserTest, Name8) {
  auto parser = pcomb::Word();
  TestParserName(parser, "Word <Until [Predicate]>");
}

TEST_F(LexemeParserTest, DigitFromEmpty) {
  auto input = "";
  auto parser = pcomb::Digit();
  auto message = pcomb::messages::EMPTY_STREAM;
  auto trace = MakeTrace(parser, {0, 0, 0}, message);
  TestParserFail(input, parser, trace);
}

TEST_F(LexemeParserTest, DigitMatch) {
  auto input = "1B";
  auto parser = pcomb::Digit();
  TestParserSuccess(input, parser, '1', 1, CheckNotEmpty('B'));
}

TEST_F(LexemeParserTest, DigitNotMatch) {
  auto input = "B1";
  auto parser = pcomb::Digit();
  auto message = pcomb::messages::UNEXPECTED_CHAR('B');
  auto trace = MakeTrace(parser, {0, 0, 0}, message);
  TestParserFail(input, parser, trace);
}

TEST_F(LexemeParserTest, InsideBracketsMatch) {
  auto input = "(A)";
  auto parser = pcomb::Inside('(', pcomb::Char('A'), ')');
  TestParserSuccess(input, parser, 'A', 3, CheckEmpty());
}

TEST_F(LexemeParserTest, InsideBracketsNoOpen) {
  using pcomb::Inside, pcomb::Char, pcomb::Skip;
  auto input = "A)B";
  auto parser = Inside('(', Char('A'), ')');

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  trace << "\t" << MakeTrace(Skip(Char('(')), {0, 0, 0});
  auto message = pcomb::messages::UNEXPECTED_CHAR('A');
  trace << "\t\t" << MakeTrace(Char('('), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(LexemeParserTest, InsideBracketsNoClose) {
  using pcomb::Inside, pcomb::Char, pcomb::Skip;
  auto input = "(AB";
  auto parser = Inside('(', Char('A'), ')');

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});
  trace << "\t" << MakeTrace(Skip(Char(')')), {2, 0, 2});
  auto message = pcomb::messages::UNEXPECTED_CHAR('B');
  trace << "\t\t" << MakeTrace(Char(')'), {2, 0, 2}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(LexemeParserTest, SpaceFromEmpty) {
  auto input = "";
  auto parser = pcomb::Space();
  auto message = pcomb::messages::EMPTY_STREAM;
  auto trace = MakeTrace(parser, {0, 0, 0}, message);
  TestParserFail(input, parser, trace);
}

TEST_F(LexemeParserTest, SpaceNotMatch) {
  auto input = "A";
  auto parser = pcomb::Space();
  auto message = pcomb::messages::UNEXPECTED_CHAR('A');
  auto trace = MakeTrace(parser, {0, 0, 0}, message);
  TestParserFail(input, parser, trace);
}

TEST_F(LexemeParserTest, SpaceMatchSpace) {
  auto input = " A";
  auto parser = pcomb::Space();
  TestParserSuccess(input, parser, ' ', 1, CheckNotEmpty('A'));
}

TEST_F(LexemeParserTest, SpaceMatchTab) {
  auto input = "\tA";
  auto parser = pcomb::Space();
  TestParserSuccess(input, parser, '\t', 1, CheckNotEmpty('A'));
}

TEST_F(LexemeParserTest, SpaceMatchNewLine) {
  auto input = "\nA";
  auto parser = pcomb::Space();
  TestParserSuccess(input, parser, '\n', 1, CheckNotEmpty('A'));
}

TEST_F(LexemeParserTest, NewLineFromEmpty) {
  auto input = "";
  auto parser = pcomb::NewLine();
  auto message = pcomb::messages::EMPTY_STREAM;
  auto trace = MakeTrace(parser, {0, 0, 0}, message);
  TestParserFail(input, parser, trace);
}

TEST_F(LexemeParserTest, NewLineNotMatch) {
  auto input = "A";
  auto parser = pcomb::NewLine();
  auto message = pcomb::messages::UNEXPECTED_CHAR('A');
  auto trace = MakeTrace(parser, {0, 0, 0}, message);
  TestParserFail(input, parser, trace);
}

TEST_F(LexemeParserTest, NewLineMatch) {
  auto input = "\nA";
  auto parser = pcomb::NewLine();
  TestParserSuccess(input, parser, '\n', 1, CheckNotEmpty('A'));
}

TEST_F(LexemeParserTest, EmptyStringFromEmpty) {
  auto input = "";
  auto parser = pcomb::String("");
  TestParserSuccess(input, parser, "", 0, CheckEmpty());
}

TEST_F(LexemeParserTest, EmptyString) {
  auto input = "AB";
  auto parser = pcomb::String("");
  TestParserSuccess(input, parser, "", 0, CheckNotEmpty('A'));
}

TEST_F(LexemeParserTest, StringFromEmpty) {
  using pcomb::String, pcomb::Chain, pcomb::Char;
  auto input = "";
  auto parser = String("ABC");

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});

  using pcomb::ParserPointer, pcomb::CharParserType;
  using L = std::list<ParserPointer<CharParserType<char>>>;
  trace << "\t" << MakeTrace(
      Chain(L{Char('A'), Char('B'), Char('C')}), {0, 0, 0});

  auto message = pcomb::messages::EMPTY_STREAM;
  trace << "\t\t" << MakeTrace(Char('A'), {0, 0, 0}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(LexemeParserTest, StringMatch) {
  auto input = "ABC";
  auto parser = pcomb::String("ABC");
  TestParserSuccess(input, parser, "ABC", 3, CheckEmpty());
}

TEST_F(LexemeParserTest, StringNotMatch) {
  using pcomb::String, pcomb::Chain, pcomb::Char;
  auto input = "ADC";
  auto parser = String("ABC");

  std::stringstream trace;
  trace << MakeTrace(parser, {0, 0, 0});

  using pcomb::ParserPointer, pcomb::CharParserType;
  using L = std::list<ParserPointer<CharParserType<char>>>;
  trace << "\t" << MakeTrace(
      Chain(L{Char('A'), Char('B'), Char('C')}), {0, 0, 0});

  auto message = pcomb::messages::UNEXPECTED_CHAR('D');
  trace << "\t\t" << MakeTrace(Char('B'), {1, 0, 1}, message);

  TestParserFail(input, parser, trace.str());
}

TEST_F(LexemeParserTest, LineFromEmpty) {
  auto input = "";
  auto parser = pcomb::Line();
  auto expected = std::list<char>{};
  TestContainerParserSuccess(input, parser, expected, 0, CheckEmpty());
}

TEST_F(LexemeParserTest, LineFromOneLineWithNewLine) {
  auto input = "AB\n";
  auto parser = pcomb::Line();
  auto expected = std::list<char>{'A', 'B'};
  TestContainerParserSuccess(input, parser, expected, 2, CheckNotEmpty('\n'));
}

TEST_F(LexemeParserTest, LineFromOneLineWithoutNewLine) {
  auto input = "AB";
  auto parser = pcomb::Line();
  auto expected = std::list<char>{'A', 'B'};
  TestContainerParserSuccess(input, parser, expected, 2, CheckEmpty());
}

TEST_F(LexemeParserTest, LineFromManyLines) {
  auto input = "AB\nCD";
  auto parser = pcomb::Line();
  auto expected = std::list<char>{'A', 'B'};
  TestContainerParserSuccess(input, parser, expected, 2, CheckNotEmpty('\n'));
}

TEST_F(LexemeParserTest, WordFromEmpty) {
  auto input = "";
  auto parser = pcomb::Word();
  auto expected = std::list<char>{};
  TestContainerParserSuccess(input, parser, expected, 0, CheckEmpty());
}

TEST_F(LexemeParserTest, WordFromOneWordWithoutSpace) {
  auto input = "AB";
  auto parser = pcomb::Word();
  auto expected = std::list<char>{'A', 'B'};
  TestContainerParserSuccess(input, parser, expected, 2, CheckEmpty());
}

TEST_F(LexemeParserTest, WordFromOneWordWithSpace) {
  auto input = "AB ";
  auto parser = pcomb::Word();
  auto expected = std::list<char>{'A', 'B'};
  TestContainerParserSuccess(input, parser, expected, 2, CheckNotEmpty(' '));
}

TEST_F(LexemeParserTest, WordFromOneWordWithNewLine) {
  auto input = "AB\n";
  auto parser = pcomb::Word();
  auto expected = std::list<char>{'A', 'B'};
  TestContainerParserSuccess(input, parser, expected, 2, CheckNotEmpty('\n'));
}

TEST_F(LexemeParserTest, WordFromOneWordWithTab) {
  auto input = "AB\t";
  auto parser = pcomb::Word();
  auto expected = std::list<char>{'A', 'B'};
  TestContainerParserSuccess(input, parser, expected, 2, CheckNotEmpty('\t'));
}
