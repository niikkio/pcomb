#ifndef PCOMB_TRACE_H_
#define PCOMB_TRACE_H_

#include <list>
#include <string>

namespace pcomb {

class Trace {
 public:
  Trace(const std::string& parser_name,
        const std::string& position,
        const std::string& message = "",
        const std::list<Trace>& nested = {});

  std::string to_string(size_t nesting_level = 0) const;

 private:
  std::list<Trace> nested_;
  std::string message_;
};

}  // namespace pcomb
#endif  // PCOMB_TRACE_H_
