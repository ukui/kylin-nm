#!/bin/bash 
# 提醒：大部分不涉及：如果你的代码会编出动态库，并且要对动态库中的代码进行测试，要先构建带覆盖率参数的动态库，才能统计到动态库代码的覆盖率，请注意
apt install -y libcap-dev libgsettings-qt-dev libkf5networkmanagerqt-dev libkf5windowsystem-dev libkysdk-qtwidgets-dev libkysdk-sysinfo-dev libkysdk-waylandhelper-dev libnm-dev libqt5svg5-dev libqt5x11extras5-dev libsecret-1-dev libukcc-dev libukui-common-dev libukui-log4qt-dev libx11-dev qt5-qmake qtbase5-dev qtbase5-dev-tools qtchooser qtscript5-dev qttools5-dev-tools qtdeclarative5-dev libukui-quick-dev libjson-glib-dev lcov libgtest-dev libgmock-dev gcovr

echo "apt install successful!!!"

cd "$(dirname "$0")" || exit 1 
if [ -d "build" ]; then
    echo "Removing existing build directory..."
    rm -rf build  # 强制删除build目录及所有内容
fi
mkdir build
cd build
echo "running qmake"
qmake  ..
if [ $? -ne 0 ]; then
    echo "Error: qmake failed."
    exit -1
fi
echo "Running make..."
make -j4
#if [ $? -ne 0 ]; then
#    echo "Error: make failed."
#    exit -1
#fi
echo "Build successful!"
exit 0

