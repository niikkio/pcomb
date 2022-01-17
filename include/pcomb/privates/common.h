#ifndef PCOMB_PRIVATES_COMMON_H_
#define PCOMB_PRIVATES_COMMON_H_

#include <list>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

namespace pcomb::privates {

class SkippedValue {
 public:
  inline bool operator==(const SkippedValue&) const {
    return true;
  }
};

class NoValue {
 public:
  inline bool operator==(const NoValue&) const {
    return true;
  }
};

template <typename... ParserPointers>
inline std::string to_string(const ParserPointers&... ps) {
  std::stringstream ss;
  const char* sep = "";
  ((static_cast<void>(ss << sep << ps->to_string(false)), sep = ", "), ...);
  return ss.str();
}

template <typename ParserPointer>
inline std::string to_string(const std::list<ParserPointer>& ps) {
  std::stringstream ss;
  const char* sep = "";
  for (const auto& p : ps) {
    ss << sep << to_string(p);
    sep = ", ";
  }
  return ss.str();
}

template <typename... ParserPointers>
inline std::string to_string(const std::tuple<ParserPointers...>& ps) {
  return std::apply([](const auto&... ts) { return to_string(ts...); }, ps);
}

template <typename ParserPointers>
inline std::string wrapped(const ParserPointers& ps) {
  std::stringstream ss;
  ss << "[" << to_string(ps) << "]";
  return ss.str();
}

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_COMMON_H_
