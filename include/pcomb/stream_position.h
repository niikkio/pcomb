#ifndef PCOMB_STREAM_POSITION_H_
#define PCOMB_STREAM_POSITION_H_

#include <string>

namespace pcomb {

struct StreamPosition {
  size_t index;
  size_t row;
  size_t column;

  bool operator==(const StreamPosition& other) const;
  std::string to_string() const;
};

}  // namespace pcomb
#endif  // PCOMB_STREAM_POSITION_H_
