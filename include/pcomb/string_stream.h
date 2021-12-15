#ifndef PCOMB_STRING_STREAM_H_
#define PCOMB_STRING_STREAM_H_

#include <memory>
#include <string>

#include "pcomb/stream.h"

namespace pcomb {

class StringStream : public IStream<char> {
 public:
  explicit StringStream(std::string s);

  const char& head() const override;
  void consume(size_t n) override;
  bool empty() const override;
  StringStream* clone() const override;

  std::string position() const override;

 private:
  struct Position {
    size_t index;
    size_t row;
    size_t column;
  };

  StringStream(const StringStream&) = default;
  StringStream& operator=(const StringStream&) = default;

  const std::shared_ptr<const std::string> string_pointer_;
  Position pos_;
};

}  // namespace pcomb
#endif  // PCOMB_STRING_STREAM_H_
