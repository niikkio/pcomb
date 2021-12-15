#ifndef PCOMB_TEST_TESTING_H_
#define PCOMB_TEST_TESTING_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <functional>
#include <sstream>
#include <string>
#include <utility>

#include "pcomb/string_stream.h"

using StreamCheck = std::function<void(const pcomb::StringStream&)>;

inline StreamCheck CheckEmpty() {
  return [](const pcomb::StringStream& s) {
           EXPECT_TRUE(s.empty());
         };
}

inline StreamCheck CheckNotEmpty(char head) {
  return [head](const pcomb::StringStream& s) {
           EXPECT_FALSE(s.empty());
           EXPECT_EQ(s.head(), head);
         };
}

inline std::string OkPosition(const std::string& input,
                              size_t expected_consumed_number) {
  size_t line = 0, column = expected_consumed_number, from = 0, pos;
  while ((pos = input.find('\n', from)) != std::string::npos) {
    if (pos >= expected_consumed_number) {
      break;
    }
    line += 1;
    from = pos + 1;
    column = expected_consumed_number - from;
  }

  std::stringstream ss;
  ss << '[' << expected_consumed_number
     << ',' << line
     << ',' << column
     << ']';

  return ss.str();
}

template <typename Parser, typename Expected>
inline void TestContainerParserSuccess(std::string input,
                                       const Parser& parser,
                                       const Expected& expected,
                                       size_t expected_consumed_number,
                                       const StreamCheck& check) {
  auto expected_pos = OkPosition(input, expected_consumed_number);
  pcomb::StringStream s(std::move(input));

  auto res = parser.parse(&s);
  EXPECT_TRUE(res.success());

  EXPECT_EQ(res.get_consumed_number(), expected_consumed_number);
  EXPECT_EQ(s.position(), expected_pos);

  EXPECT_EQ(res.get_value().size(), expected.size());
  EXPECT_THAT(res.get_value(), ::testing::ContainerEq(expected));

  check(s);
}

template <typename Parser>
inline void TestContainerParserFail(std::string input, const Parser& parser) {
  pcomb::StringStream s(std::move(input));
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
                              size_t expected_consumed_number,
                              const StreamCheck& check) {
  auto expected_pos = OkPosition(input, expected_consumed_number);
  pcomb::StringStream s(std::move(input));

  auto res = parser.parse(&s);
  EXPECT_TRUE(res.success());

  EXPECT_EQ(res.get_consumed_number(), expected_consumed_number);
  EXPECT_EQ(s.position(), expected_pos);

  EXPECT_EQ(res.get_value(), expected);

  check(s);
}

template <typename Parser>
inline void TestParserFail(std::string input, const Parser& parser) {
  pcomb::StringStream s(std::move(input));
  auto check = s.empty() ? CheckEmpty() : CheckNotEmpty(s.head());

  auto res = parser.parse(&s);
  EXPECT_FALSE(res.success());
  EXPECT_EQ(res.get_consumed_number(), 0);

  check(s);
}

#endif  // PCOMB_TEST_TESTING_H_
