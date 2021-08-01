#ifndef PCOMB_TEST_COMMON_H_
#define PCOMB_TEST_COMMON_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <functional>
#include <string>
#include <utility>

#include "pcomb/mock_stream.h"

using StreamCheck = std::function<void(const pcomb::MockStream&)>;

inline StreamCheck CheckEmpty() {
  return [](const pcomb::MockStream& s) {
           EXPECT_TRUE(s.empty());
         };
}

inline StreamCheck CheckNotEmpty(char head) {
  return [head](const pcomb::MockStream& s) {
           EXPECT_FALSE(s.empty());
           EXPECT_EQ(s.head(), head);
         };
}

template <typename Parser, typename Expected>
inline void TestContainerParserSuccess(std::string input,
                                       const Parser& parser,
                                       const Expected& expected,
                                       int expected_consumed_number,
                                       const StreamCheck& check) {
  pcomb::MockStream s(std::move(input));

  auto res = parser.parse(&s);
  EXPECT_TRUE(res.success());
  EXPECT_EQ(res.get_consumed_number(), expected_consumed_number);
  EXPECT_THAT(res.get_value().size(), expected.size());
  EXPECT_THAT(res.get_value(), ::testing::ContainerEq(expected));

  check(s);
}

template <typename Parser>
inline void TestContainerParserFail(std::string input, const Parser& parser) {
  pcomb::MockStream s(std::move(input));
  auto check = s.empty() ? CheckEmpty() : CheckNotEmpty(s.head());

  auto res = parser.parse(&s);
  EXPECT_FALSE(res.success());
  EXPECT_EQ(res.get_consumed_number(), 0);

  check(s);
}

template <typename Parser, typename Expected>
inline void TestParserSuccess(std::string input,
                              const Parser& parser,
                              const Expected& expected,
                              int expected_consumed_number,
                              const StreamCheck& check) {
  pcomb::MockStream s(std::move(input));

  auto res = parser.parse(&s);
  EXPECT_TRUE(res.success());
  EXPECT_EQ(res.get_consumed_number(), expected_consumed_number);
  EXPECT_EQ(res.get_value(), expected);

  check(s);
}

template <typename Parser>
inline void TestParserFail(std::string input, const Parser& parser) {
  pcomb::MockStream s(std::move(input));
  auto check = s.empty() ? CheckEmpty() : CheckNotEmpty(s.head());

  auto res = parser.parse(&s);
  EXPECT_FALSE(res.success());
  EXPECT_EQ(res.get_consumed_number(), 0);

  check(s);
}

#endif  // PCOMB_TEST_COMMON_H_
