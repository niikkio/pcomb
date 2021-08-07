#include <gtest/gtest.h>

#include <functional>
#include <list>
#include <variant>

#include "common.h"

#include "pcomb/adaptive.h"
#include "pcomb/alternative.h"
#include "pcomb/many.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"

using pcomb::Adapted;
using pcomb::Char;
using pcomb::Digit;
using pcomb::StrictAny;
using pcomb::StrictSeq;

class AdaptiveParserTest : public ::testing::Test {
 protected:
  std::function<int(char)> char2int{
      [](char c) { return c - '0'; }};

  std::function<char(std::variant<char, char>)> var2char{
      [](std::variant<char, char>&& v) {
        auto value = std::get_if<0>(&v);
        if (value != nullptr) return *value;
        return *std::get_if<1>(&v);
      }};

  std::function<std::string(std::list<char>)> list2string{
      [](std::list<char>&& chars) {
        return std::string(chars.cbegin(), chars.cend());
      }};

  std::function<int(char, char)> chars2int{
      [](char d1, char d0) {
        return 10 * (d1 - '0') + (d0 - '0');
      }};
};

TEST_F(AdaptiveParserTest, HeadMatch) {
  TestParserSuccess("8", Adapted(Digit(), char2int), 8, 1, CheckEmpty());
}

TEST_F(AdaptiveParserTest, HeadNotMatch) {
  TestParserFail("A", Adapted(Digit(), char2int));
}

TEST_F(AdaptiveParserTest, AdaptedManyAlternative) {
  auto parser = Adapted(
      Many(Adapted(StrictAny(Char('A'), Char('B')), var2char)), list2string);

  TestParserSuccess("ABBAABABC", parser,
                    std::string("ABBAABAB"), 8,
                    CheckNotEmpty('C'));
}

TEST_F(AdaptiveParserTest, TwoDigitsToNumber) {
  auto parser = Adapted(StrictSeq(Digit(), Digit()), chars2int);

  TestParserSuccess("42", parser, 42, 2, CheckEmpty());
  TestParserSuccess("131", parser, 13, 2, CheckNotEmpty('1'));
}

TEST_F(AdaptiveParserTest, Memory) {
  static int n = 0;
  static int n_copy = 0;
  static int n_move = 0;

  struct A {
    A() { ++n; }

    A(const A& other) { ++n_copy; }
    A& operator=(const A& other) { ++n_copy; return *this; }

    A(A&& other) { ++n_move; }
    A& operator=(A&& other) { ++n_move; return *this; }

    bool operator==(const A& other) const {
      return true;
    }
  };

  std::function<A(char)> char2a{[](char c) { return A(); }};
  std::function<A(A&&)> a2a{[](A&& a) { return A(std::forward<A>(a)); }};

  auto parser = Adapted(Adapted(Digit(), char2a), a2a);
  TestParserSuccess("0", parser, A(), 1, CheckEmpty());

  EXPECT_EQ(2, n);
  EXPECT_EQ(0, n_copy);
  EXPECT_EQ(3, n_move);
}
