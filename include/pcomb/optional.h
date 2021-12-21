#ifndef PCOMB_OPTIONAL_H_
#define PCOMB_OPTIONAL_H_

#include <functional>
#include <memory>
#include <utility>

#include "pcomb/adaptive.h"
#include "pcomb/privates/optional.h"

namespace pcomb {

template <typename P>
inline auto Opt(std::shared_ptr<P>&& parser) {
  return std::make_shared<privates::OptionalParser<P>>(
      std::forward<std::shared_ptr<P>>(parser));
}

template <typename P, typename T>
inline auto ParseWithDefault(std::shared_ptr<P>&& parser,
                             const T& default_value) {
  auto maybe = Opt(std::forward<std::shared_ptr<P>>(parser));

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
