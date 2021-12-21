#ifndef PCOMB_MANY_H_
#define PCOMB_MANY_H_

#include <memory>
#include <utility>

#include "pcomb/privates/many.h"

namespace pcomb {

template <typename P>
inline auto Many(std::shared_ptr<P>&& parser) {
  return std::make_shared<privates::ManyParser<P>>(
      std::forward<std::shared_ptr<P>>(parser));
}

template <typename P>
inline auto Some(std::shared_ptr<P>&& parser) {
  return std::make_shared<privates::ManyParser<P>>(
      std::forward<std::shared_ptr<P>>(parser), 1);
}

template <typename P>
inline auto Maybe(std::shared_ptr<P>&& parser) {
  return std::make_shared<privates::ManyParser<P>>(
      std::forward<std::shared_ptr<P>>(parser), 0, 1);
}

template <typename P>
inline auto Repeat(std::shared_ptr<P>&& parser, size_t count) {
  return std::make_shared<privates::ManyParser<P>>(
      std::forward<std::shared_ptr<P>>(parser), count, count);
}

}  // namespace pcomb
#endif  // PCOMB_MANY_H_
