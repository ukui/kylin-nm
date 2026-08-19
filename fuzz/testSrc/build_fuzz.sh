#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

#export CC="afl-cc"
#export CXX="afl-c++"
export AFL_USE_UBSAN=1
export AFL_USE_ASAN=1
export AFL_USE_LSAN=1

qmake kylin-nm-fuzz-test.pro
make clean
make -j4 CC="afl-cc -D_GNU_SOURCE" CXX="afl-c++ -D_GNU_SOURCE" LD="afl-c++" LINK="afl-c++"
if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo "Build completed successfully!"
echo "Fuzzing binary is ready in: $SCRIPT_DIR"
echo ""
echo "Usage:"
echo "  ./kylin-nm-fuzz-test <input_file>"
