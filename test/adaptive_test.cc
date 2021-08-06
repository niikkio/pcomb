#include <gtest/gtest.h>

#include <functional>
#include <list>
#include <variant>

#include "common.h"

#include "pcomb/adaptive.h"
#include "pcomb/many.h"
#include "pcomb/predicate.h"
#include "pcomb/strict_alternative.h"
#include "pcomb/strict_sequence.h"

class AdaptiveParserTest : public ::testing::Test {
 protected:
  using Ch = pcomb::PredicateParser<char>;
  using Adaptive = pcomb::AdaptiveParser<Ch, std::function<int(char)>>;

  Ch pd = Ch([](char c) { return '0' <= c && c <= '9'; });
  Adaptive a = Adaptive(pd, [](char c) { return c - '0'; });
};

TEST_F(AdaptiveParserTest, HeadMatch) {
  TestParserSuccess("8", a, 8, 1, CheckEmpty());
}

TEST_F(AdaptiveParserTest, HeadNotMatch) {
  TestParserFail("A", a);
}

TEST_F(AdaptiveParserTest, AdaptedManyAlternative) {
  Ch pa = Ch([](char c) { return c == 'A'; });
  Ch pb = Ch([](char c) { return c == 'B'; });

  auto a = pcomb::StrictAlternativeParser(pa, pb);
  auto f_a = [](std::variant<char, char>&& v) {
               auto value = std::get_if<0>(&v);
               if (value != nullptr) return *value;
               return *std::get_if<1>(&v);
             };

  auto a_a = pcomb::AdaptiveParser(a, f_a);
  auto m_a_a = pcomb::ManyParser(a_a);
  auto f_m_a_a = [](std::list<char>&& cs) {
                   return std::string(cs.cbegin(), cs.cend());
                 };

  auto a_m_a_a = pcomb::AdaptiveParser(m_a_a, f_m_a_a);

  TestParserSuccess("ABBAABABC", a_m_a_a,
                    std::string("ABBAABAB"), 8,
                    CheckNotEmpty('C'));
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

  using PA = pcomb::AdaptiveParser<Ch, std::function<A(char)>>;
  std::function<A(char)> f([](char c) { return A(); });
  PA pa = pcomb::AdaptiveParser(pd, f);

  using PAA = pcomb::AdaptiveParser<PA, std::function<A(A&&)>>;
  std::function<A(A&&)> fa([](A&& a) { return A(std::forward<A>(a)); });
  PAA paa = pcomb::AdaptiveParser(pa, fa);

  TestParserSuccess("0", paa, A(), 1, CheckEmpty());

  EXPECT_EQ(2, n);
  EXPECT_EQ(0, n_copy);
  EXPECT_EQ(3, n_move);
}

TEST_F(AdaptiveParserTest, Number) {
  Ch digit = Ch([](char c) { return '0' <= c && c <= '9'; });

  using Seq = pcomb::StrictSequenceParser<Ch, Ch>;
  auto number = Seq(digit, digit);

  using CharsToInt = std::function<int(char, char)>;
  CharsToInt chars2int = [](char d1, char d0) {
                           return 10 * (d1 - '0') + (d0 - '0');
                         };
  auto p = pcomb::AdaptiveParser<Seq, CharsToInt>(number, chars2int);

  TestParserSuccess("42", p, 42, 2, CheckEmpty());
  TestParserSuccess("131", p, 13, 2, CheckNotEmpty('1'));
}
