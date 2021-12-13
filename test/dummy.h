#ifndef PCOMB_TEST_DUMMY_
#define PCOMB_TEST_DUMMY_

#include <memory>

struct Tree {
  using Pointer = std::shared_ptr<Tree>;

  explicit Tree(int v) : L(nullptr), v(v), R(nullptr) { }

  explicit Tree(Pointer lhs, int v, Pointer rhs)
      : L(lhs), v(v), R(rhs) { }

  Pointer L;
  int v;
  Pointer R;
};

#endif  // PCOMB_TEST_DUMMY_
