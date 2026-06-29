#ifndef MOCK_SERVICES_H
#define MOCK_SERVICES_H

#include <QObject>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QMap>
#include <QVariant>

// 模拟 Ukcc DBus 服务
class MockUkccService : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.ukui.ukcc.session.interface")
public:
    explicit MockUkccService(QObject* parent = nullptr);
    ~MockUkccService();

    bool isValid() const { return m_registered; }
    void setModuleHideStatus(const QMap<QString, QVariant>& data);

public Q_SLOTS:
    QMap<QString, QVariant> getModuleHideStatus();

private:
    QMap<QString, QVariant> m_moduleHideStatus;
    bool m_registered = false;
};

// 模拟系统睡眠信号
class MockLogin1Service : public QObject
{
    Q_OBJECT
public:
    explicit MockLogin1Service(QObject* parent = nullptr);
    ~MockLogin1Service();

    void emitPrepareForSleep(bool state);

private:
    bool m_registered = false;
};

#endif // MOCK_SERVICES_H