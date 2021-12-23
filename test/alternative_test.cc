#include <gtest/gtest.h>

#include <list>
#include <variant>
#include <sstream>

#include "testing.h"

#include "pcomb/alternative.h"
#include "pcomb/many.h"
#include "pcomb/predicate.h"
#include "pcomb/skipped.h"

using pcomb::Any;
using pcomb::Char;
using pcomb::Many;
using pcomb::Skip;
using pcomb::Some;

class AlternativeParserTest : public ::testing::Test {
 protected:
  static auto pA() {
    return with_name(Any(Char('A')), "pA");
  }

  static auto traceA(char ch) {
    std::stringstream ss;
    ss << "pA failed at [0,0,0]\n"
       << "\tChar(A) Parser failed at [0,0,0]"
       << " [unexpected character: \'" << ch << "\']\n";
    return ss.str();
  }

  static auto pABC() {
    return with_name(Any(Char('A'), Char('B'), Char('C')), "pABC");
  }

  static auto traceABC(char c) {
    std::stringstream ss;
    ss << "pABC failed at [0,0,0]\n"
       << "\tChar(A) Parser failed at [0,0,0] "
       << "[unexpected character: \'" << c << "\']\n"
       << "\tChar(B) Parser failed at [0,0,0] "
       << "[unexpected character: \'" << c << "\']\n"
       << "\tChar(C) Parser failed at [0,0,0] "
       << "[unexpected character: \'" << c << "\']\n";
    return ss.str();
  }

  static auto pABs() {
    return Any(Char('A'), Some(Char('B')));
  }

  static auto expectedABs_A() {
    return std::variant<char, std::list<char>> {
        std::in_place_index<0>, 'A'};
  }

  static auto expectedABs_Bs(size_t n) {
    return std::variant<char, std::list<char>> {
        std::in_place_index<1>, std::list<char>(n, 'B')};
  }

  static auto pManyAsBs() {
    return Many(Any(Some(Char('A')), Some(Char('B'))));
  }

  using ExpectedManyAsBs = std::list<std::list<char>>;

  static auto pSkippedAsSkippedBs() {
    return Any(Skip(Some(Char('A'))), Skip(Some(Char('B'))));
  }

  static auto expectedSkippedAsSkippedBs() {
    return decltype(pSkippedAsSkippedBs())::element_type::ValueType();
  }
};

TEST_F(AlternativeParserTest, SingleMatch) {
  TestParserSuccess("A", pA(), 'A', 1, CheckEmpty());
}

TEST_F(AlternativeParserTest, SingleNotMatch) {
  TestParserFail("B", pA(), traceA('B'));
}

TEST_F(AlternativeParserTest, NotMatch) {
  TestParserFail("D", pABC(), traceABC('D'));
}

TEST_F(AlternativeParserTest, Take1) {
  TestParserSuccess("ABC", pABC(), 'A', 1, CheckNotEmpty('B'));
}

TEST_F(AlternativeParserTest, Take2) {
  TestParserSuccess("BCA", pABC(), 'B', 1, CheckNotEmpty('C'));
}

TEST_F(AlternativeParserTest, Take3) {
  TestParserSuccess("CAB", pABC(), 'C', 1, CheckNotEmpty('A'));
}

TEST_F(AlternativeParserTest, TakeOneOrMany1) {
  TestParserSuccess("ABBB", pABs(),
                    expectedABs_A(), 1, CheckNotEmpty('B'));
}

TEST_F(AlternativeParserTest, TakeOneOrMany2) {
  TestParserSuccess("BBBA", pABs(),
                    expectedABs_Bs(3), 3, CheckNotEmpty('A'));
}

TEST_F(AlternativeParserTest, ManyAlternatives) {
  auto expected = ExpectedManyAsBs {
      {'B', 'B'}, {'A', 'A', 'A'}, {'B'}, {'A', 'A'}, {'B', 'B', 'B'}};

  TestContainerParserSuccess("BBAAABAABBB", pManyAsBs(),
                             expected, 11, CheckEmpty());
}

TEST_F(AlternativeParserTest, AlternativeSkipped1) {
  TestParserSuccess("ABBBBBC", pSkippedAsSkippedBs(),
                    expectedSkippedAsSkippedBs(), 1, CheckNotEmpty('B'));
}

TEST_F(AlternativeParserTest, AlternativeSkipped2) {
  TestParserSuccess("BBBBBAC", pSkippedAsSkippedBs(),
                    expectedSkippedAsSkippedBs(), 5, CheckNotEmpty('A'));
}
