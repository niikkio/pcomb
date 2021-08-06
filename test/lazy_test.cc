#include <gtest/gtest.h>

#include <memory>

#include "common.h"

#include "pcomb/adaptive.h"
#include "pcomb/alternative.h"
#include "pcomb/lazy.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"
#include "pcomb/skipped.h"

// (Tree) < v > (Tree)
// v

struct Tree {
  using PointerType = std::shared_ptr<Tree>;

  explicit Tree(int v) : L(nullptr), v(v), R(nullptr) { }

  explicit Tree(PointerType lhs, int v, PointerType rhs)
      : L(lhs), v(v), R(rhs) { }

  PointerType L;
  int v;
  PointerType R;
};

using TP = Tree::PointerType;

auto Skip(char ch) {
  return pcomb::SkippedParser(pcomb::PredicateParser<char>(
      [ch](char c) { return c == ch; }));
}

auto Int = pcomb::AdaptiveParser(
      pcomb::PredicateParser<char>(
          [](char c) { return '0' <= c && c <= '9'; }),
      std::function<int(char)>([](char c) { return c - '0'; }));

class TreeParser : pcomb::Parser<char, TP> {
 public:
  using CharType = char;
  using ValueType = TP;

 private:
  using ResultType = pcomb::Result<ValueType>;
  using StreamType = pcomb::IStream<CharType>;

  using LazyTreeParser = pcomb::LazyParser<TreeParser>;

  auto E1() const {
    return pcomb::AdaptiveParser(
        Int,
        std::function<TP(int)>(
            [](int v) { return std::make_shared<Tree>(v); }));
  }

  auto E2() const {
    return pcomb::AdaptiveParser(
        pcomb::SequenceParser(Skip('('), LazyTreeParser(this), Skip(')'),
                              Skip('<'), Int, Skip('>'),
                              Skip('('), LazyTreeParser(this), Skip(')')),
        std::function<TP(TP, int, TP)>([](TP&& t1, int v, TP&& t2) {
            return std::make_shared<Tree>(std::forward<TP>(t1),
                                          v,
                                          std::forward<TP>(t2)); }));
  }

 public:
  ResultType parse(StreamType* stream) const override {
    return pcomb::AlternativeParser(E1(), E2()).parse(stream);
  }
};

class LazyParserTest : public ::testing::Test {
 protected:
  TreeParser treeParser;
};

TEST_F(LazyParserTest, Tree) {
  pcomb::MockStream s("((1)<2>(3))<4>(5)");

  auto res = treeParser.parse(&s);
  EXPECT_TRUE(res.success());
  EXPECT_EQ(res.get_consumed_number(), 17);

  auto tree = res.get_value().get();
  EXPECT_THAT(tree->L->L->v, 1);
  EXPECT_THAT(tree->L->v, 2);
  EXPECT_THAT(tree->L->R->v, 3);
  EXPECT_THAT(tree->v, 4);
  EXPECT_THAT(tree->R->v, 5);
}
