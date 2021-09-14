#include <gtest/gtest.h>

#include <functional>
#include <tuple>

#include "common.h"

#include "pcomb/adaptive.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"

using pcomb::Char;
using pcomb::Seq;
using pcomb::StrictAdapted;

class StrictAdaptiveParserTest : public ::testing::Test {
 protected:
  struct Pair {
    char a;
    char b;

    bool operator==(const Pair& other) const {
      return a == other.a && b == other.b;
    }
  };

  template <class Tuple, size_t... Is>
  static Pair construct(Tuple&& tuple, std::index_sequence<Is...> ) {
    return Pair{std::get<Is>(std::forward<Tuple>(tuple))...};
  }

  template <class Tuple>
  static Pair construct(Tuple&& tuple) {
    return construct(std::forward<Tuple>(tuple),
        std::make_index_sequence<
            std::tuple_size<std::decay_t<Tuple>>::value>{});
  }

  using ValueType = std::tuple<char, char>;

  std::function<Pair(ValueType&&)> adapter = [](ValueType&& t) {
    return construct(std::forward<ValueType>(t));
  };
};

TEST_F(StrictAdaptiveParserTest, Empty) {
  auto parser = StrictAdapted(Seq(Char('A'), Char('B')), adapter);
  TestParserFail("", parser);
}

TEST_F(StrictAdaptiveParserTest, Match) {
  auto parser = StrictAdapted(Seq(Char('A'), Char('B')), adapter);
  TestParserSuccess("AB", parser, Pair{'A', 'B'}, 2, CheckEmpty());
}

TEST_F(StrictAdaptiveParserTest, NotMatch) {
  auto parser = StrictAdapted(Seq(Char('A'), Char('B')), adapter);
  TestParserFail("BA", parser);
}
