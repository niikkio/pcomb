#include "pcomb/string_stream.h"

#include <memory>
#include <sstream>
#include <string>

#include "pcomb/stream_position.h"

namespace pcomb {

StringStream::StringStream(std::string source)
    : string_pointer_(std::make_shared<std::string>(std::move(source))),
      pos_{0, 0, 0} {
}

const char& StringStream::head() const {
  return string_pointer_->at(pos_.index);
}

void StringStream::consume(size_t n) {
  for (size_t i = 0; i < n; ++i) {
    if (head() == '\n') {
      pos_.row += 1;
      pos_.column = 0;

    } else {
      pos_.column += 1;
    }

    pos_.index += 1;
  }
}

bool StringStream::empty() const {
  return pos_.index >= string_pointer_->size();
}

StringStream* StringStream::clone() const {
  return new StringStream(*this);
}

StreamPosition StringStream::position() const {
  return pos_;
}

}  // namespace pcomb
