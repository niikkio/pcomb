#include "pcomb/stream_position.h"

#include <sstream>
#include <string>

namespace pcomb {

bool StreamPosition::operator==(const StreamPosition& other) const {
  return index == other.index && row == other.row && column == other.column;
}

std::string StreamPosition::to_string() const {
  std::stringstream ss;
  ss << '[' << index << ',' << row << ',' << column << ']';
  return ss.str();
}

}  // namespace pcomb
