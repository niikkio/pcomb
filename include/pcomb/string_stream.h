#ifndef PCOMB_STRING_STREAM_H_
#define PCOMB_STRING_STREAM_H_

#include <memory>
#include <string>

#include "pcomb/stream.h"

namespace pcomb {

class StringStream : public IStream<char> {
 public:
  explicit StringStream(std::string s);

  StringStream(const StringStream&) = default;
  StringStream& operator=(const StringStream&) = default;

  const char& head() const override;
  void consume(int n) override;
  bool empty() const override;
  StringStream* clone() const override;

 private:
  const std::shared_ptr<const std::string> string_pointer_;
  int begin_;
};

}  // namespace pcomb
#endif  // PCOMB_STRING_STREAM_H_
