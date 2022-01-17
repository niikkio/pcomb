#ifndef PCOMB_TRACE_H_
#define PCOMB_TRACE_H_

#include <list>
#include <string>
#include <utility>

#include "pcomb/stream_position.h"

namespace pcomb {

class Trace {
 public:
  using LogType = std::list<Trace>;

  template <typename ParserPointer, typename StreamPointer>
  Trace(const ParserPointer& parser,
        const StreamPointer& stream,
        const std::string& message = "",
        LogType&& nested = {})
      : parser_name_(parser->to_string()),
        position_(stream->position()),
        message_(message),
        nested_(std::forward<LogType>(nested)) {
  }

  std::string to_string(size_t nesting_level = 0) const;

 private:
  std::string parser_name_;
  StreamPosition position_;
  std::string message_;
  LogType nested_;
};

}  // namespace pcomb
#endif  // PCOMB_TRACE_H_
