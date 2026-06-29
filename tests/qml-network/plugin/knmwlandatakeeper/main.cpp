#include <QCoreApplication>
#include <QDebug>
#include "test_knmwlandatakeeper.h"
#include "../../../../qml-network/plugin/knmwlandatakeeper.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setAttribute(Qt::AA_Use96Dpi, true);
    
    // 启用测试模式以跳过D-Bus初始化
    KnmWlanDataKeeper::setTestMode(true);
    
    TestKnmWlanDataKeeper tc;
    return QTest::qExec(&tc, argc, argv);
}
