#ifndef PCOMB_ALTERNATIVE_H_
#define PCOMB_ALTERNATIVE_H_

#include <memory>
#include <utility>

#include "pcomb/privates/alternative.h"
#include "pcomb/privates/strict_alternative.h"

namespace pcomb {

template <typename P1, typename... PS>
inline auto Any(std::shared_ptr<P1>&& p1, std::shared_ptr<PS>&&... ps) {
  return std::make_shared<privates::AlternativeParser<P1, PS...>>(
          std::forward<std::shared_ptr<P1>>(p1),
          std::forward<std::shared_ptr<PS>>(ps)...);
}

template <typename P1, typename... PS>
inline auto StrictAny(std::shared_ptr<P1>&& p1, std::shared_ptr<PS>&&... ps) {
  return std::make_shared<privates::StrictAlternativeParser<P1, PS...>>(
          std::forward<std::shared_ptr<P1>>(p1),
          std::forward<std::shared_ptr<PS>>(ps)...);
}

}  // namespace pcomb
#endif  // PCOMB_ALTERNATIVE_H_
