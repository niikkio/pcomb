cmake -S . -B build -D pcomb_build_tests=ON || exit 1
cmake --build build -- -j4 || exit 1
cd build && ctest
