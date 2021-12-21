#include <gtest/gtest.h>

#include <memory>

#include "dummy.h"
#include "testing.h"
#include "transformers.h"

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

class LazyParserTest : public ::testing::Test {
 private:
  class TreeParser : pcomb::Parser<char, std::shared_ptr<Tree>> {
    // "(Tree)<v>(Tree)" OR "v"
   public:
    using CharType = char;
    using ValueType = Tree::Pointer;

   private:
    using ResultType = pcomb::Result<ValueType>;
    using StreamType = pcomb::IStream<CharType>;

    auto Int() const {
      return Adapted(Digit(), &char2int);
    }

    auto E1() const {
      return Adapted(Int(), &int2tree);
    }

    auto E2() const {
      return Adapted(Seq(Skip(Char('(')), Lazy(this), Skip(Char(')')),
                         Skip(Char('<')), Int(),        Skip(Char('>')),
                         Skip(Char('(')), Lazy(this), Skip(Char(')'))),
                     &seq2tree);
    }

   public:
    ResultType parse(StreamType* stream) const override {
      return Any(E1(), E2())->parse(stream);
    }
  };

 protected:
  static auto pTree() {
    return TreeParser();
  }
};

TEST_F(LazyParserTest, Tree) {
  auto parser = pTree();

  Stream s("((1)<2>(3))<4>(5)");

  auto res = parser.parse(&s);
  EXPECT_TRUE(res.success());
  EXPECT_EQ(res.get_consumed_number(), 17);

  auto tree = res.get_value().get();

  ASSERT_TRUE(tree != nullptr);
  EXPECT_THAT(tree->v, 4);

  ASSERT_TRUE(tree->L != nullptr);
  EXPECT_THAT(tree->L->v, 2);

  ASSERT_TRUE(tree->L->L != nullptr);
  EXPECT_THAT(tree->L->L->v, 1);

  ASSERT_TRUE(tree->L->R != nullptr);
  EXPECT_THAT(tree->L->R->v, 3);

  ASSERT_TRUE(tree->R != nullptr);
  EXPECT_THAT(tree->R->v, 5);
}
