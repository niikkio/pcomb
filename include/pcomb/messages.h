#ifndef PCOMB_MESSAGES_H_
#define PCOMB_MESSAGES_H_

#include <sstream>
#include <string>

#include "pcomb/stream_position.h"

namespace pcomb::messages {
  inline const std::string NO_MESSAGE {""};

  inline std::string END_WAS_EXPECTED() {
    return "unexpected characters at the end";
  }

  template <typename CharType>
  inline std::string END_WAS_EXPECTED(const CharType& ch,
                                      const StreamPosition& pos) {
    std::stringstream ss;
    ss << "unexpected characters at the end: \'"
       << ch << "...\' at " << pos.to_string();
    return ss.str();
  }

  inline std::string NOT_ENOUGH_RESULTS(size_t n, size_t expected) {
    std::stringstream ss;
    ss << "not enough results: " << n << " < " << expected;
    return ss.str();
  }

  template <typename CharType>
  inline std::string UNEXPECTED_CHAR(CharType ch) {
    std::stringstream ss;
    ss << "unexpected character: \'" << ch << "\'";
    return ss.str();
  }

  inline const std::string EMPTY_STREAM {"characters not found"};
}  // namespace pcomb::messages
#endif  // PCOMB_MESSAGES_H_
