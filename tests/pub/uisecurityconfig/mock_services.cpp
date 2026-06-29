#include "mock_services.h"

// 模拟 Ukcc DBus 服务
MockUkccService::MockUkccService(QObject* parent) : QObject(parent) {
    m_registered = QDBusConnection::sessionBus().registerService("org.ukui.ukcc.session") &&
                   QDBusConnection::sessionBus().registerObject("/", this, QDBusConnection::ExportAllSlots);
}

MockUkccService::~MockUkccService() {
    if (m_registered) {
        QDBusConnection::sessionBus().unregisterService("org.ukui.ukcc.session");
    }
}

void MockUkccService::setModuleHideStatus(const QMap<QString, QVariant>& data) {
    m_moduleHideStatus = data;
}

QMap<QString, QVariant> MockUkccService::getModuleHideStatus() {
    return m_moduleHideStatus;
}

// 模拟系统睡眠信号
MockLogin1Service::MockLogin1Service(QObject* parent) : QObject(parent) {
    m_registered = QDBusConnection::systemBus().registerService("org.freedesktop.login1") &&
                   QDBusConnection::systemBus().registerObject("/org/freedesktop/login1", this);
}

MockLogin1Service::~MockLogin1Service() {
    if (m_registered) {
        QDBusConnection::systemBus().unregisterService("org.freedesktop.login1");
    }
}

void MockLogin1Service::emitPrepareForSleep(bool state) {
    QDBusMessage msg = QDBusMessage::createSignal(
        "/org/freedesktop/login1",
        "org.freedesktop.login1.Manager",
        "PrepareForSleep"
    );
    msg << state;
    QDBusConnection::systemBus().send(msg);
}