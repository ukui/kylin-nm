#ifndef BACKTHREAD_H
#define BACKTHREAD_H

#include <QObject>
#include <QDebug>

class IFace{
public:
    QString lname;
    QString wname;
    int lstate; // 0已连接 1未连接 2已关闭
    int wstate; // 0已连接 1未连接 2已关闭
};

class BackThread : public QObject
{
    Q_OBJECT
public:
    explicit BackThread(QObject *parent = nullptr);

    IFace* execGetIface();
    QString getConnProp(QString connName);
    bool execChkWifiExist(QString connName);
    QString execChkLanWidth(QString ethName);

signals:
    void enNetDone();
    void disNetDone();
    void enWifiDone();
    void disWifiDone();

    void connDone(int connFlag);

    void btFinish();

public slots:
    void execEnNet();
    void execDisNet();
    void execEnWifi();
    void execDisWifi();
    void execConnLan(QString connName);
    void execConnWifi(QString connName);
    void execConnWifiPWD(QString connName, QString password);
};

#endif // BACKTHREAD_H
