#ifndef PCOMB_OPTIONAL_H_
#define PCOMB_OPTIONAL_H_

#include <functional>
#include <utility>

#include "pcomb/adaptive.h"
#include "pcomb/parser.h"
#include "pcomb/privates/optional.h"

namespace pcomb {

template <typename P>
inline auto Opt(ParserPointer<P>&& parser) {
  return make<privates::OptionalParser<P>>(
      std::forward<ParserPointer<P>>(parser));
}

template <typename P, typename T>
inline auto ParseWithDefault(ParserPointer<P>&& parser,
                             const T& default_value) {
  auto maybe = Opt(std::forward<ParserPointer<P>>(parser));

  using R = typename decltype(maybe)::element_type::ValueType;
  using V = typename R::value_type;
  std::function<V(R)> adapter =
      [=](R&& opt) {
        return opt.value_or(default_value);
      };
  return Adapted(std::move(maybe), std::move(adapter));
}

}  // namespace pcomb
#endif  // PCOMB_OPTIONAL_H_
