#include <gtest/gtest.h>

#include <list>

#include "testing.h"

#include "pcomb/many.h"
#include "pcomb/predicate.h"

using pcomb::Char;
using pcomb::Many;
using pcomb::Some;
using pcomb::Repeat;

class ManyParserTest : public ::testing::Test {
 protected:
  static auto pManyA() {
    return Many(Char('A'));
  }

  static auto pSomeA() {
    return Some(Char('A'));
  }

  static auto pRepeatA(size_t n) {
    return Repeat(Char('A'), n);
  }

  static auto expected(size_t n = 0) {
    return std::list<char>(n, 'A');
  }
};

TEST_F(ManyParserTest, ManyEmpty) {
  TestContainerParserSuccess("", pManyA(), expected(), 0, CheckEmpty());
}

TEST_F(ManyParserTest, ManyNoOne) {
  TestContainerParserSuccess(
      "BBB", pManyA(), expected(), 0, CheckNotEmpty('B'));
}

TEST_F(ManyParserTest, ManySome) {
  TestContainerParserSuccess(
      "AAAB", pManyA(), expected(3), 3, CheckNotEmpty('B'));
}

TEST_F(ManyParserTest, ManyAll) {
  TestContainerParserSuccess("AAAAA", pManyA(), expected(5), 5, CheckEmpty());
}

TEST_F(ManyParserTest, SomeEmpty) {
  TestContainerParserFail("", pSomeA());
}

TEST_F(ManyParserTest, SomeNoOne) {
  TestContainerParserFail("BBB", pSomeA());
}

TEST_F(ManyParserTest, SomeOne) {
  TestContainerParserSuccess(
      "AB", pSomeA(), expected(1), 1, CheckNotEmpty('B'));
}

TEST_F(ManyParserTest, SomeSome) {
  TestContainerParserSuccess(
      "AAAB", pSomeA(), expected(3), 3, CheckNotEmpty('B'));
}

TEST_F(ManyParserTest, SomeAll) {
  TestContainerParserSuccess("AAAAA", pSomeA(), expected(5), 5, CheckEmpty());
}

TEST_F(ManyParserTest, RepeatEmpty) {
  TestContainerParserFail("", pRepeatA(3));
}

TEST_F(ManyParserTest, RepeatNoOne) {
  TestContainerParserFail("B", pRepeatA(3));
}

TEST_F(ManyParserTest, RepeatNotEnough1) {
  TestContainerParserFail("A", pRepeatA(3));
}

TEST_F(ManyParserTest, RepeatNotEnough2) {
  TestContainerParserFail("AAB", pRepeatA(3));
}

TEST_F(ManyParserTest, RepeatEnough) {
  TestContainerParserSuccess("AAA", pRepeatA(3), expected(3), 3, CheckEmpty());
}

TEST_F(ManyParserTest, RepeatMore) {
  TestContainerParserSuccess(
      "AAAA", pRepeatA(3), expected(3), 3, CheckNotEmpty('A'));
}
