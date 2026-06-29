#!/bin/bash
export QT_QPA_PLATFORM=offscreen
export QT_LOGGING_RULES="qt.qpa.*=false"
export QT_FATAL_WARNINGS=0
cd "$(dirname "$0")" || exit 1
# 启用lastpipe解决子shell变量：隔离问题
shopt -s lastpipe 
BASE_DIR=$(pwd)
# 检查依赖命令是否存在
if ! command -v file &> /dev/null; then
    echo "错误：系统未安装 file 命令，无法识别二进制文件！" >&2
    exit 1
fi
# 初始化计数器：仅统计「有执行权限+非.so/.a/.o后缀+二进制格式」的文件
binary_count=0
# 提示信息
echo "===== 开始递归扫描 ${BASE_DIR} 目录下有执行权限的二进制程序 ====="
echo
# 核心逻辑：递归查找所有文件，过滤规则：有执行权限 + 非.so/.a/.o后缀 + 二进制格式
find "${BASE_DIR}" -type f -print0 | while IFS= read -r -d $'\0' file; do
    # 跳过符号链接（如需包含，注释此行）
    if [ -L "${file}" ]; then
        continue
    fi
    # 第一步：过滤后缀为.so/.a/.o的文件（直接跳过）
    if [[ "${file}" =~ \.(so|a|o)$ ]]; then
        continue
    fi
    # 第二步：判断是否有可执行权限（无则跳过）
    if [ ! -x "${file}" ]; then
        continue
    fi
    # 第三步：识别是否为二进制文件（过滤文本/脚本/空文件等）
    if file -b "${file}" | grep -qiE 'elf|executable|binary' && \
       file -b "${file}" | grep -qiEv 'text|ascii|script'; then
        echo "=============运行程序：${file}=================="
        "${file}"
        echo "=============程序结束：${file}=================="
        # 计数器+1（仅统计符合所有条件的文件）
        ((binary_count++))
    fi
done
echo
# 检查计数器：无符合条件的文件则报错退出
if [ "${binary_count}" -eq 0 ]; then
    echo "错误：在 ${BASE_DIR} 及其子目录中未扫描到有执行权限的二进制程序"
    exit 2  # 非零退出码标识错误
else
    echo "===== 脚本运行结束，共找到 ${binary_count} 个符合条件的二进制程序 ====="
    exit 0
fi

