#include <gtest/gtest.h>

#include <list>

#include "testing.h"

#include "pcomb/predicate.h"
#include "pcomb/until.h"

using pcomb::Char;
using pcomb::Until;

class UntilParserTest : public ::testing::Test {
 protected:
  static auto pA() {
    return Until(Char('A'));
  }

  using Expected = std::list<char>;
};

TEST_F(UntilParserTest, Empty) {
  TestContainerParserSuccess("", pA(), Expected{}, 0, CheckEmpty());
}

TEST_F(UntilParserTest, EmptyMatch) {
  TestContainerParserSuccess(
      "A", pA(), Expected{}, 0, CheckNotEmpty('A'));
}

TEST_F(UntilParserTest, Match) {
  TestContainerParserSuccess(
      "BCA", pA(), Expected{'B', 'C'}, 2, CheckNotEmpty('A'));
}
