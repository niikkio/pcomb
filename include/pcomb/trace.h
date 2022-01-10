#ifndef PCOMB_TRACE_H_
#define PCOMB_TRACE_H_

#include <list>
#include <string>
#include <utility>

#include "pcomb/stream_position.h"

namespace pcomb {

class Trace {
 public:
  template <typename StreamPointer>
  Trace(const std::string& parser_name,
        const StreamPointer stream_pointer,
        const std::string& message = "",
        std::list<Trace>&& nested = {})
      : parser_name_(parser_name)
      , position_(stream_pointer->position())
      , message_(message)
      , nested_(std::forward<std::list<Trace>>(nested)) {
  }

  std::string to_string(size_t nesting_level = 0) const;

 private:
  std::string parser_name_;
  StreamPosition position_;
  std::string message_;
  std::list<Trace> nested_;
};

}  // namespace pcomb
#endif  // PCOMB_TRACE_H_
