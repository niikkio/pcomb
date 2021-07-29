#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <list>

#include "pcomb/mock_stream.h"
#include "pcomb/many.h"
#include "pcomb/predicate.h"

class ManyParserTest : public ::testing::Test {
protected:
    using Ch = pcomb::PredicateParser<char>;
    using Many = pcomb::ManyParser<Ch>;
    using Expected = std::list<char>;
    using Stream = pcomb::MockStream;

    // void SetUp() override { }
    // void TearDown() override { }

    void CheckParserSuccess(Stream* stream,
                            const Many& parser,
                            const Expected& expected) {

        auto res = parser.parse(stream);
        EXPECT_TRUE(res.success());
        EXPECT_EQ(res.get_consumed_number(), expected.size());
        EXPECT_THAT(res.get_value(), ::testing::ContainerEq(expected));
    }

    void CheckParserFail(Stream* stream,
                         const Many& parser) {
        auto res = parser.parse(stream);
        EXPECT_FALSE(res.success());
    }

    Ch pa = Ch([](char c) { return c == 'A'; });
    Many many = Many(pa);
    Many some = Many(pa, 1);
    Many exact3 = Many(pa, 3, 3);
};

TEST_F(ManyParserTest, ManyEmpty) {
    Stream s("");
    CheckParserSuccess(&s, many, Expected{});
}

TEST_F(ManyParserTest, ManyNoOne) {
    Stream s("BBB");
    CheckParserSuccess(&s, many, Expected{});
    EXPECT_EQ(s.head(), 'B');
}

TEST_F(ManyParserTest, ManySome) {
    Stream s("AAAB");
    CheckParserSuccess(&s, many, Expected{'A','A','A'});
    EXPECT_EQ(s.head(), 'B');
}

TEST_F(ManyParserTest, ManyAll) {
    Stream s("AAAAA");
    CheckParserSuccess(&s, many, Expected{'A','A','A','A','A'});
    EXPECT_TRUE(s.empty());
}

TEST_F(ManyParserTest, SomeEmpty) {
    Stream s("");
    CheckParserFail(&s, some);
}

TEST_F(ManyParserTest, SomeNoOne) {
    Stream s("BBB");
    CheckParserFail(&s, some);
}

TEST_F(ManyParserTest, SomeOne) {
    Stream s("AB");
    CheckParserSuccess(&s, some, Expected{'A'});
    EXPECT_EQ(s.head(), 'B');
}

TEST_F(ManyParserTest, SomeSome) {
    Stream s("AAAB");
    CheckParserSuccess(&s, some, Expected{'A','A','A'});
    EXPECT_EQ(s.head(), 'B');
}

TEST_F(ManyParserTest, SomeAll) {
    Stream s("AAAAA");
    CheckParserSuccess(&s, some, Expected{'A','A','A','A','A'});
    EXPECT_TRUE(s.empty());
}

TEST_F(ManyParserTest, RepeatEmpty) {
    Stream s("");
    CheckParserFail(&s, exact3);
}

TEST_F(ManyParserTest, RepeatNoOne) {
    Stream s("B");
    CheckParserFail(&s, exact3);
    EXPECT_EQ(s.head(), 'B');
}

TEST_F(ManyParserTest, RepeatNotEnough1) {
    Stream s("A");
    CheckParserFail(&s, exact3);
    EXPECT_EQ(s.head(), 'A');
}

TEST_F(ManyParserTest, RepeatNotEnough2) {
    Stream s("AAB");
    CheckParserFail(&s, exact3);
    EXPECT_EQ(s.head(), 'A');
}

TEST_F(ManyParserTest, RepeatEnough) {
    Stream s("AAA");
    CheckParserSuccess(&s, exact3, Expected{'A','A','A'});
    EXPECT_TRUE(s.empty());
}

TEST_F(ManyParserTest, RepeatMore) {
    Stream s("AAAA");
    CheckParserSuccess(&s, exact3, Expected{'A','A','A'});
    EXPECT_EQ(s.head(), 'A');
}
