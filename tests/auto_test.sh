#!/bin/bash

# 设置退出脚本当命令失败时 (非零退出状态)
#set -e

# 函数：运行单元测试
run_unit_tests() {
    local pattern="unit_test_*"
    
    # 遍历所有匹配模式的目录
    for dir in $pattern; do
        if [ -d "$dir" ] && [ -x "$dir/$(basename "$dir")" ]; then
            echo "Running tests in $dir..."
	    # 执行单元测试
	    (cd "$dir" && "./$(basename "$dir")")
            # 收集代码覆盖率数据
	    echo "Collecting coverage data..."
	    (cd "$dir" && (find ./ -name '*.o' | xargs gcov --preserve-paths))
        else
	    echo "Skipping non-existent or non-executable directory: $dir, $dir/$(basename "$dir")"
        fi
    done
}

# 上传 result.zip 到平台
URL=$1
upload_result() {
    echo "current pwd : $(pwd)"
    # 收集覆盖率信息
    lcov -d . -c -o r.info

    # 删除不需要的文件或路径
    lcov -r r.info "$(pwd)/unit_test_*" "$(pwd)/../registeredSession/universalinterface.cpp" "/usr/include/*" "/opt/*" -o coverage.info

    # 生成html覆盖率报告
    genhtml "$(pwd)/coverage.info" -o result

    # 打包
    zip -r  result.zip result
    # 上传平台
    curl --insecure -X POST -F "file=@/$(pwd)/result.zip" -F "package=ukui-control-center" -F "username=hesisheng" $URL
    echo "all parameter : $@"
}

# 编译项目
echo "Compiling the project..."
qmake . && make

# 运行各个单元测试
echo "Running unit tests..."
run_unit_tests

# 上传到平台
#upload_result

echo "All tests have been run successfully, and coverage data has been collected."
