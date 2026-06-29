#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"


#apt install -y gcc
#apt install -y afl++

export AFL_USE_UBSAN=1
export AFL_USE_ASAN=1
export AFL_USE_LSAN=1


if [ $? -ne 0 ]; then
    echo "Error: build failed."
    exit -1
fi


./fuzz/testSrc/build_fuzz.sh

if [ $? -ne 0 ]; then
    echo "Error: build fuzz test failed."
    exit -1
fi

afl-system-config

echo "start run"
afl-fuzz -i fuzz/in/ -o fuzz/out/ -M master_nm -V 10800 -- ./fuzz/testSrc/kylin-nm-fuzz-test @@ &
#afl-fuzz -i ../in/ -o ../out_nm_base -V 10800 -- ./libfuzz-test fuzz_kylin_nm_base @@
#afl-fuzz -i ../in/ -o ../out_enterprise -V 10800 -- ./libfuzz-test fuzz_enterprise_setting @@
#afl-fuzz -i ../in/ -o ../out_keyring -V 10800 -- ./libfuzz-test fuzz_kylin_keyring @@
##afl-fuzz -i ../in/ -o ../out_nm_base -V 10800 -- ./libfuzz-test fuzz_kylin_nm_base @@1afl-fuzz -i ../in/ -o ../out_networkmanager -V 10800 -- ./libfuzz-test fuzz_kylin_networkmanager @@
