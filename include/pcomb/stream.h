#ifndef PCOMB_STREAM_H_
#define PCOMB_STREAM_H_

#include "pcomb/stream_position.h"

#include <memory>

namespace pcomb {

template <typename S>
using StreamPointer = std::unique_ptr<S>;

template <typename CharType>
class IStream {
 public:
  virtual const CharType& head() const = 0;
  virtual void consume(size_t n) = 0;
  virtual bool empty() const = 0;
  virtual StreamPointer<IStream> clone() const = 0;
  virtual StreamPosition position() const = 0;
  virtual ~IStream() = default;
};

}  // namespace pcomb
#endif  // PCOMB_STREAM_H_
