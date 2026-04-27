# rm -rf build
# cmake -S . -B build
set -e
cmake --build build
ctest --test-dir ./build