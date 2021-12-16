#ifndef PCOMB_PARSER_H_
#define PCOMB_PARSER_H_

#include <memory>
#include <string>

#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {

template <typename C, typename V>
class Parser {
 public:
  using CharType = C;
  using ValueType = V;

  virtual ~Parser() = default;

  virtual Result<ValueType> parse(IStream<CharType>* stream) const = 0;

  std::string name() const {
    return name_;
  }

  Parser& with_name(const std::string& name) {
    name_ = name;
    return *this;
  }

 private:
  std::string name_ = "Base";
};

template <typename T, typename C = char>
using ParserType = Parser<C, T>;

template <typename T, typename C = char>
using ParserPointerType = std::shared_ptr<ParserType<T, C>>;

}  // namespace pcomb
#endif  // PCOMB_PARSER_H_
