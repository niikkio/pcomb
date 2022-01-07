#ifndef PCOMB_STRING_STREAM_H_
#define PCOMB_STRING_STREAM_H_

#include <memory>
#include <string>

#include "pcomb/stream.h"
#include "pcomb/stream_position.h"

namespace pcomb {

class StringStream : public IStream<char> {
 public:
  explicit StringStream(std::string s);

  const char& head() const override;
  void consume(size_t n) override;
  bool empty() const override;
  StreamPointer<IStream<char>> clone() const override;
  StreamPosition position() const override;

 private:
  StringStream(const StringStream&) = default;
  StringStream& operator=(const StringStream&) = default;

  const std::shared_ptr<const std::string> string_pointer_;
  StreamPosition pos_;
};

}  // namespace pcomb
#endif  // PCOMB_STRING_STREAM_H_
