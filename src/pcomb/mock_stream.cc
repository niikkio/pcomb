#include "pcomb/mock_stream.h"

#include <memory>
#include <string>

namespace pcomb {

MockStream::MockStream(std::string source)
    : string_pointer_(std::make_shared<std::string>(std::move(source))),
      begin_(0) {
}

const char& MockStream::head() const {
  return string_pointer_->at(begin_);
}

void MockStream::consume(int n) {
  begin_ += n;
}

bool MockStream::empty() const {
  return begin_ >= string_pointer_->size();
}

MockStream* MockStream::clone() const {
  return new MockStream(*this);
}

}  // namespace pcomb
