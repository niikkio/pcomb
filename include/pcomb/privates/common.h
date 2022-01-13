#ifndef PCOMB_PRIVATES_COMMON_H_
#define PCOMB_PRIVATES_COMMON_H_

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

}  // namespace pcomb::privates
#endif  // PCOMB_PRIVATES_COMMON_H_
