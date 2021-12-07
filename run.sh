cmake -S . -B build -D pcomb_build_tests=ON || exit 1
cmake --build build -- -j4 || exit 1

ctest_command="ctest"
if [ -n "$1" ]; then
  ctest_command="${ctest_command} -R \"$1\""
fi

cd build && eval "$ctest_command"

COVERAGE_WORKING_PATH=test/CMakeFiles/core_test.dir
COVERAGE_INFO_FILENAME=${COVERAGE_WORKING_PATH}/cov.info
FILTERED_COVERAGE_INFO_FILENAME=${COVERAGE_WORKING_PATH}/filtered_cov.info

lcov -o ${COVERAGE_INFO_FILENAME} -c -d ${COVERAGE_WORKING_PATH}
lcov --remove ${COVERAGE_INFO_FILENAME} '*/test/*' '*/build/_deps/*' '/usr/include/*' -o ${FILTERED_COVERAGE_INFO_FILENAME}
genhtml --demangle-cpp -o report ${FILTERED_COVERAGE_INFO_FILENAME}
lcov --zerocounters -d ${COVERAGE_WORKING_PATH}
