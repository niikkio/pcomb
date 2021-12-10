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
  using Expected = std::list<char>;
};

TEST_F(ManyParserTest, ManyEmpty) {
  TestContainerParserSuccess("", Many(Char('A')), Expected{}, 0, CheckEmpty());
}

TEST_F(ManyParserTest, ManyNoOne) {
  TestContainerParserSuccess("BBB", Many(Char('A')),
                             Expected{}, 0,
                             CheckNotEmpty('B'));
}

TEST_F(ManyParserTest, ManySome) {
  TestContainerParserSuccess("AAAB", Many(Char('A')),
                             Expected{'A', 'A', 'A'}, 3,
                             CheckNotEmpty('B'));
}

TEST_F(ManyParserTest, ManyAll) {
  TestContainerParserSuccess("AAAAA", Many(Char('A')),
                             Expected{'A', 'A', 'A', 'A', 'A'}, 5,
                             CheckEmpty());
}

TEST_F(ManyParserTest, SomeEmpty) {
  TestContainerParserFail("", Some(Char('A')));
}

TEST_F(ManyParserTest, SomeNoOne) {
  TestContainerParserFail("BBB", Some(Char('A')));
}

TEST_F(ManyParserTest, SomeOne) {
  TestContainerParserSuccess("AB", Some(Char('A')),
                             Expected{'A'}, 1,
                             CheckNotEmpty('B'));
}

TEST_F(ManyParserTest, SomeSome) {
  TestContainerParserSuccess("AAAB", Some(Char('A')),
                             Expected{'A', 'A', 'A'}, 3,
                             CheckNotEmpty('B'));
}

TEST_F(ManyParserTest, SomeAll) {
  TestContainerParserSuccess("AAAAA", Some(Char('A')),
                             Expected{'A', 'A', 'A', 'A', 'A'}, 5,
                             CheckEmpty());
}

TEST_F(ManyParserTest, RepeatEmpty) {
  TestContainerParserFail("", Repeat(Char('A'), 3));
}

TEST_F(ManyParserTest, RepeatNoOne) {
  TestContainerParserFail("B", Repeat(Char('A'), 3));
}

TEST_F(ManyParserTest, RepeatNotEnough1) {
  TestContainerParserFail("A", Repeat(Char('A'), 3));
}

TEST_F(ManyParserTest, RepeatNotEnough2) {
  TestContainerParserFail("AAB", Repeat(Char('A'), 3));
}

TEST_F(ManyParserTest, RepeatEnough) {
  TestContainerParserSuccess("AAA", Repeat(Char('A'), 3),
                             Expected{'A', 'A', 'A'}, 3,
                             CheckEmpty());
}

TEST_F(ManyParserTest, RepeatMore) {
  TestContainerParserSuccess("AAAA", Repeat(Char('A'), 3),
                             Expected{'A', 'A', 'A'}, 3,
                             CheckNotEmpty('A'));
}
