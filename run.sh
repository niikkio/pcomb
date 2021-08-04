cmake -S . -B build || exit 1
cmake --build build -- -j4 || exit 1

ctest_command="ctest"
if [ -n "$1" ]; then
  ctest_command="${ctest_command} -R \"$1\""
fi

cd build && eval "$ctest_command"
