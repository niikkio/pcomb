#ifndef PCOMB_TEST_TESTING_H_
#define PCOMB_TEST_TESTING_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <functional>
#include <sstream>
#include <string>
#include <utility>

#include "pcomb/stream_position.h"
#include "pcomb/string_stream.h"

using StreamCheck = std::function<void(const pcomb::StringStream&)>;
using StreamPosition = pcomb::StreamPosition;

template <typename ParserPointer>
inline auto TestParserName(const ParserPointer& parser,
                           const std::string& expected_name) {
  EXPECT_EQ(parser->to_string(), expected_name);
}

template <typename ParserPointer>
inline auto MakeTrace(const ParserPointer& parser, StreamPosition&& pos,
                      const std::string& message = "") {
  std::stringstream ss;
  ss << parser->to_string() << " failed at " << pos.to_string();
  if (message.size() > 0) {
    ss << " [" << message << "]";
  }
  ss << "\n";
  return ss.str();
}

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

inline StreamPosition OkPosition(const std::string& input,
                              size_t expected_consumed_number) {
  StreamPosition pos{expected_consumed_number, 0, expected_consumed_number};
  size_t from = 0, i;
  while ((i = input.find('\n', from)) != std::string::npos) {
    if (i >= expected_consumed_number) {
      break;
    }
    pos.row += 1;
    from = i + 1;
  }
  pos.column = expected_consumed_number - from;

  return pos;
}

template <typename Parser, typename Expected>
inline void TestContainerParserSuccess(std::string input,
                                       const Parser& parser,
                                       const Expected& expected,
                                       size_t expected_consumed_number,
                                       const StreamCheck& check) {
  auto expected_pos = OkPosition(input, expected_consumed_number);
  pcomb::StringStream s(std::move(input));

  auto res = parser->parse(&s);
  EXPECT_TRUE(res.success());

  EXPECT_EQ(res.get_consumed_number(), expected_consumed_number);
  EXPECT_EQ(s.position(), expected_pos);

  EXPECT_EQ(res.get_value().size(), expected.size());
  EXPECT_THAT(res.get_value(), ::testing::ContainerEq(expected));

  check(s);
}

template <typename Parser>
inline void TestContainerParserFail(std::string input, const Parser& parser,
                                    const std::string& expected_trace = "") {
  pcomb::StringStream s(std::move(input));
  auto check = s.empty() ? CheckEmpty() : CheckNotEmpty(s.head());

  auto res = parser->parse(&s);
  EXPECT_FALSE(res.success());
  EXPECT_EQ(res.get_consumed_number(), 0);

  if (expected_trace.size() > 0) {
    EXPECT_EQ(res.get_trace().to_string(), expected_trace);
  }

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

  auto res = parser->parse(&s);
  EXPECT_TRUE(res.success());

  EXPECT_EQ(res.get_consumed_number(), expected_consumed_number);
  EXPECT_EQ(s.position(), expected_pos);

  EXPECT_EQ(res.get_value(), expected);

  check(s);
}

template <typename Parser>
inline void TestParserFail(std::string input,
                           const Parser& parser,
                           const std::string& expected_trace = "") {
  pcomb::StringStream s(std::move(input));
  auto check = s.empty() ? CheckEmpty() : CheckNotEmpty(s.head());

  auto res = parser->parse(&s);
  EXPECT_FALSE(res.success());
  EXPECT_EQ(res.get_consumed_number(), 0);

  if (expected_trace.size() > 0) {
    EXPECT_EQ(res.get_trace().to_string(), expected_trace);
  }

  check(s);
}

#endif  // PCOMB_TEST_TESTING_H_
