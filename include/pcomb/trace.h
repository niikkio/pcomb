#ifndef PCOMB_TRACE_H_
#define PCOMB_TRACE_H_

#include <list>
#include <string>

#include "pcomb/stream_position.h"

namespace pcomb {

class Trace {
 public:
  Trace(const std::string& parser_name,
        StreamPosition&& position,
        std::string&& message = "",
        std::list<Trace>&& nested = {});

  std::string to_string(size_t nesting_level = 0) const;

 private:
  std::string parser_name_;
  StreamPosition position_;
  std::string message_;
  std::list<Trace> nested_;
};

}  // namespace pcomb
#endif  // PCOMB_TRACE_H_
