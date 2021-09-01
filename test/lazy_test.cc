#include <gtest/gtest.h>

#include <memory>

#include "common.h"

#include "pcomb/adaptive.h"
#include "pcomb/alternative.h"
#include "pcomb/lazy.h"
#include "pcomb/lexeme.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"
#include "pcomb/skipped.h"
#include "pcomb/string_stream.h"

using Stream = pcomb::StringStream;
using pcomb::Adapted;
using pcomb::Any;
using pcomb::Char;
using pcomb::Digit;
using pcomb::Lazy;
using pcomb::Seq;
using pcomb::Skip;

// (Tree) < v > (Tree)
// v

class LazyParserTest : public ::testing::Test {
 protected:
  struct Tree {
    using Pointer = std::shared_ptr<Tree>;

    explicit Tree(int v) : L(nullptr), v(v), R(nullptr) { }

    explicit Tree(Pointer lhs, int v, Pointer rhs)
        : L(lhs), v(v), R(rhs) { }

    Pointer L;
    int v;
    Pointer R;
  };

  class TreeParser : pcomb::Parser<char, Tree::Pointer> {
   public:
    using CharType = char;
    using ValueType = Tree::Pointer;

   private:
    using ResultType = pcomb::Result<ValueType>;
    using StreamType = pcomb::IStream<CharType>;

    auto Int() const {
      return Adapted(Digit(), digit2int);
    }

    auto E1() const {
      return Adapted(Int(), int2tree);
    }

    auto E2() const {
      return Adapted(Seq(Skip(Char('(')), Lazy(this), Skip(Char(')')),
                         Skip(Char('<')), Int(),        Skip(Char('>')),
                         Skip(Char('(')), Lazy(this), Skip(Char(')'))),
                     seq2tree);
    }

   public:
    ResultType parse(StreamType* stream) const override {
      return Any(E1(), E2()).parse(stream);
    }

   private:
    using TP = Tree::Pointer;

    const std::function<int(char)> digit2int{
        [](char c) { return c - '0'; }};

    const std::function<TP(int)> int2tree{
        [](int v) { return std::make_shared<Tree>(v); }};

    const std::function<TP(TP, int, TP)> seq2tree{
        [](TP t1, int v, TP t2) {
          return std::make_shared<Tree>(t1, v, t2);
        }};
  };
};

TEST_F(LazyParserTest, Tree) {
  auto parser = TreeParser();

  Stream s("((1)<2>(3))<4>(5)");
  auto res = parser.parse(&s);
  EXPECT_TRUE(res.success());
  EXPECT_EQ(res.get_consumed_number(), 17);

  auto tree = res.get_value().get();
  EXPECT_THAT(tree->L->L->v, 1);
  EXPECT_THAT(tree->L->v, 2);
  EXPECT_THAT(tree->L->R->v, 3);
  EXPECT_THAT(tree->v, 4);
  EXPECT_THAT(tree->R->v, 5);
}
