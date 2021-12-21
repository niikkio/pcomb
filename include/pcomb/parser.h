#ifndef PCOMB_PARSER_H_
#define PCOMB_PARSER_H_

#include <memory>
#include <string>
#include <utility>

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

  friend std::shared_ptr<Parser>&& with_name(
      std::shared_ptr<Parser>&& p, const std::string& name) {
    p->name_ = name;
    return std::forward<std::shared_ptr<Parser>>(p);
  }

 private:
  std::string name_ = "Parser";
};

}  // namespace pcomb
#endif  // PCOMB_PARSER_H_
