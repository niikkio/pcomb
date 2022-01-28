#include <gtest/gtest.h>

#include <memory>

#include "dummy.h"
#include "testing.h"
#include "transformers.h"

#include "pcomb/adaptive.h"
#include "pcomb/alternative.h"
#include "pcomb/exact.h"
#include "pcomb/lazy.h"
#include "pcomb/lexeme.h"
#include "pcomb/messages.h"
#include "pcomb/number.h"
#include "pcomb/predicate.h"
#include "pcomb/sequence.h"
#include "pcomb/skipped.h"
#include "pcomb/string_stream.h"
#include "pcomb/trace.h"

class LazyParserTest : public ::testing::Test {
 protected:
  class TreeParser : public pcomb::Parser<char, std::shared_ptr<Tree>> {
    // "(Tree)<v>(Tree)" OR "v"
   protected:
    std::string to_string_without_name() const override {
      return "Tree";
    }

   public:
    using CharType = char;
    using ValueType = Tree::Pointer;

    TreeParser() {
      this->name_ = "T";
    }

   private:
    using ResultType = pcomb::Result<ValueType>;
    using StreamType = pcomb::IStream<CharType>;

    auto E1() const {
      using pcomb::Adapted;
      return Adapted(pcomb::Int(), &int2tree);
    }

    auto E2() const {
      using pcomb::Adapted, pcomb::Seq, pcomb::Skip, pcomb::Lazy, pcomb::Char;
      return Adapted(Seq(Skip(Char('(')), Lazy(this),   Skip(Char(')')),
                         Skip(Char('<')), pcomb::Int(), Skip(Char('>')),
                         Skip(Char('(')), Lazy(this),   Skip(Char(')'))),
                     &seq2tree);
    }

   public:
    ResultType parse(StreamType* stream) const override {
      auto result = pcomb::Any(E1(), E2())->parse(stream);
      if (!result.success()) {
        return ResultType(pcomb::Trace(this, stream,
                                       pcomb::messages::NO_MESSAGE,
                                       {std::move(result).get_trace()}));
      }
      return result;
    }
  };
};

TEST_F(LazyParserTest, Name1) {
  auto tree_parser = TreeParser();
  auto parser = pcomb::Lazy(&tree_parser);
  TestParserName(parser, "Lazy <Lazy [Tree]>");
}

TEST_F(LazyParserTest, Name2) {
  auto parser = pcomb::make<TreeParser>();
  TestParserName(parser, "T <Tree>");
}

TEST_F(LazyParserTest, Tree) {
  pcomb::StringStream s("((1)<2>(3))<4>(5)");
  auto parser = pcomb::make<TreeParser>();

  auto res = parser->parse(&s);
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

TEST_F(LazyParserTest, TreeFail1) {
  auto good_input = std::string("((1)<2>(3))<4>(5)");
  auto parser = pcomb::Exact(pcomb::make<TreeParser>());


  for (size_t i = 0; i <= good_input.size(); ++i) {
    auto input = std::string(good_input).insert(i, 1, 'x');
    TestParserFail(input, parser);
  }

  for (size_t i = 0; i < good_input.size(); ++i) {
    auto input = std::string(good_input).erase(i, 1);
    TestParserFail(input, parser);
  }
}
