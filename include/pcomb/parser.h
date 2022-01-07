#ifndef PCOMB_PARSER_H_
#define PCOMB_PARSER_H_

#include <memory>
#include <string>
#include <utility>

#include "pcomb/result.h"
#include "pcomb/stream.h"

namespace pcomb {

template <typename P>
using ParserPointer = std::shared_ptr<P>;

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

  friend ParserPointer<Parser>&& with_name(
      ParserPointer<Parser>&& p, const std::string& name) {
    p->name_ = name;
    return std::forward<ParserPointer<Parser>>(p);
  }

 protected:
  std::string name_ = "Parser";
};

template <typename P, typename... Args>
inline std::shared_ptr<P> make(Args&&... args) {
  return std::make_shared<P>(std::forward<Args>(args)...);
}

}  // namespace pcomb
#endif  // PCOMB_PARSER_H_
