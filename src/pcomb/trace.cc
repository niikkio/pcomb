#include "pcomb/trace.h"

#include <list>
#include <sstream>
#include <string>

namespace pcomb {

Trace::Trace(const std::string& parser_name,
             const std::string& position,
             const std::string& message,
             const std::list<Trace>& nested)
    : nested_(nested) {
  std::stringstream ss;
  ss << parser_name << " Parser failed at " << position;

  if (message.size() > 0) {
    ss << " [" << message << "]";
  }

  message_ = ss.str();
}

std::string Trace::to_string(size_t nesting_level) const {
  std::stringstream ss;
  ss << std::string(nesting_level, '\t') << message_ << '\n';
  for (auto& child : nested_) {
    ss << child.to_string(nesting_level + 1);
  }
  return ss.str();
}

}  // namespace pcomb
