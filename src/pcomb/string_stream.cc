#include "pcomb/string_stream.h"

#include <memory>
#include <string>

namespace pcomb {

StringStream::StringStream(std::string source)
    : string_pointer_(std::make_shared<std::string>(std::move(source))),
      begin_(0) {
}

const char& StringStream::head() const {
  return string_pointer_->at(begin_);
}

void StringStream::consume(size_t n) {
  begin_ += n;
}

bool StringStream::empty() const {
  return begin_ >= string_pointer_->size();
}

StringStream* StringStream::clone() const {
  return new StringStream(*this);
}

}  // namespace pcomb
