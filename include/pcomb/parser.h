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

 protected:
  std::string name_ = "Parser";
};

template <typename P, typename... Args>
inline std::shared_ptr<P> make(Args&&... args) {
  return std::make_shared<P>(std::forward<Args>(args)...);
}

template <typename P>
using ParserPointer = std::shared_ptr<P>;

}  // namespace pcomb
#endif  // PCOMB_PARSER_H_
