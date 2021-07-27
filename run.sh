cmake -S . -B build || exit 1
cmake --build build -- -j4 || exit 1

cd build && ctest
