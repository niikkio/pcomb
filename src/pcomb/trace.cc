#include "pcomb/trace.h"

#include <list>
#include <sstream>
#include <string>

#include "pcomb/stream_position.h"

namespace pcomb {

std::string Trace::to_string(size_t nesting_level) const {
  std::stringstream ss;
  ss << std::string(nesting_level, '\t')
     << parser_name_ << " failed at " << position_.to_string();

  if (message_.size() > 0) {
    ss << " [" << message_ << ']';
  }

  ss << '\n';

  for (auto& child : nested_) {
    ss << child.to_string(nesting_level + 1);
  }
  return ss.str();
}

}  // namespace pcomb
