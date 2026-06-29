/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef KYLINSECRETAGENT_H
#define KYLINSECRETAGENT_H


#include <QObject>
#include <QDebug>
#include <QDBusInterface>
#include <QSharedPointer>
#include <QPointer>
#include <QMutex>
#include <QEventLoop>
#include <QTimer>
//#include <NetworkManagerQt/SecretAgent>
#include "networkmanagerqt/secretagent.h"
#include <libsecret/secret.h>
#include "agentutils.h"


// 等待请求封装类
class SecretWaitRequest {
public:
    QString deviceName;
    QString ssid;
    QString userName;
    QString password;
    bool canceled = false;
    QPointer<QEventLoop> loop;
    QMutex mutex;

    // 阻塞等待直到 quit() 被调用
    void wait() {
        QMutexLocker locker(&mutex);
        if (!loop) {
            loop = new QEventLoop();
        }
        locker.unlock();

        QTimer timer;
        timer.setSingleShot(true);
        timer.setInterval(120000); // 120 秒超时
        QObject::connect(&timer, &QTimer::timeout, [this]() {
            // 超时视为取消
            quit(QVariantMap(), true);
        });
        timer.start();
        loop->exec();   // 阻塞
        // 注意：loop 可能在 quit() 后已被删除，但 QPointer 会置空
    }

    // 唤醒等待，并传递结果
    void quit(const QVariantMap &secretMap = QVariantMap(), bool cancel = false) {
        qDebug()<<Q_FUNC_INFO << __LINE__<<secretMap<<cancel<<loop;
        QMutexLocker locker(&mutex);
        if(secretMap.contains(IDENTITY_KEY)){
            userName = secretMap[IDENTITY_KEY].toString();
        }
        if(secretMap.contains(PRIVATE_KEY)) {
            password = secretMap[PRIVATE_KEY].toString();
        } else if(secretMap.contains(PASSWORD_KEY)){
            password = secretMap[PASSWORD_KEY].toString();
        }
        canceled = cancel;
        if (loop) {
            loop->quit();   // 退出事件循环
            // loop 的所有权仍在 wait() 线程，将在 wait() 返回后由调用者删除
        }
    }
};


class KylinSecretAgent: public NetworkManager::SecretAgent
{
    Q_OBJECT

public:
    explicit KylinSecretAgent(const QString &id, QObject *parent = nullptr);
    ~KylinSecretAgent();

public Q_SLOTS:
    // 新增：供 QML 通过 DBus 调用的提交/取消接口
    void onSubmitPassword(QString requestId, const QString &ssid, const QVariantMap &secretMap);
    void onCancelPassword(QString requestId, const QString &ssid);

public:
    // 提供给测试使用的公共接口（纯逻辑函数）
    QStringList getVpnPassword(QVariantMap &vpnSettingMap);
    QString getVpnDataItemByKey(QVariantMap &vpnSettingMap, QString key);
    QString getWepKeyFromId(uint wepId);

private:
    void saveOneSecret(QString &connectUuid, QString settingName,
                       QString secretKey, QString &secretValue, QString &displayNmae);
    void saveConnectSecret(QVariantMap &securityMap, QString secretSettingName,
                           QString &secretKey, QString &connectId, QString &connectUuid);

    void saveWepSecret(QVariantMap &wirelessSecurityMap,
                                QString &connectId, QString &connectUuid);
    void saveWirelessSecret(QVariantMap &wirelessSecurityMap,
                                QString &connectId, QString &connectUuid);

    void saveEnterpriceSecret(QVariantMap &enterpriceSecurityMap,
                                QString &connectId, QString &connectUuidd);

    void saveSecretForWireless(const NMVariantMapMap &connection,
                                QString &connectId, QString &connectUuid);

    void saveSecretForVpn(QVariantMap &vpnSetingMap,
                                QString &connectId, QString &connectUuid);

    void getSecretFromKeyring(GList *p_secretList, const QString &settingName,
                                QString connectType, NMVariantMapMap &secretMap);
    bool hintIsFind(GList *p_secretList, const QStringList &hints);
    bool connectionIsAlwaysAsk(const NMVariantMapMap &connection);

    void askSecretForLeap(QVariantMap &wirelessSecurityMap,
                          const QString &settingName, NMVariantMapMap &secretMap, QVariantMap connectMap, uint flags, bool isWired = false);
    void askSecretForWep(QVariantMap &wirelessSecurityMap,
                          const QString &settingName, NMVariantMapMap &secretMap, QVariantMap connectMap, uint flags, bool isWired = false);
    void askSecretForPassword(QVariantMap &securityMap, QString passwordKey,
                          const QString &settingName, NMVariantMapMap &secretMap, QVariantMap connectMap, uint flags, bool isWired = false);
    void askSecretForWireless(const NMVariantMapMap &connection,
                           const QString settingName, NMVariantMapMap &secretMap, uint flags, bool isWired = false);

    void askSecretForTls(QVariantMap securityMap,
                                       const QString &settingName, NMVariantMapMap &secretMap, QVariantMap connectMap, uint flags, bool isWired = false);
    void askSecretWithIdentityAndPassword(QVariantMap securityMap, QString userKey,
                               QString passwordKey, const QString &settingName, NMVariantMapMap &secretMap, QVariantMap connectMap, uint flags, bool isWired = false);
    void askSecretForEnterpriceWireless(const NMVariantMapMap &connection,
                                        const QString settingName, NMVariantMapMap &secretMap, uint flags, bool isWired = false);
    void askSecretForVpn(const NMVariantMapMap &connection,
                                          const QString settingName, NMVariantMapMap &secretMap);
    void askForSecret(const NMVariantMapMap &connection, const QString &settingName,
                                                QString connectType, NMVariantMapMap &secretMap, uint flags);

    int blockWaitVpnRequestResult(unsigned int timeoutMs,NMVariantMapMap &secretMap);
public Q_SLOTS:

    /*
     * kylin-nm的agent功能主要是实现如下四个函数，这四个函数继承自父类NetworkManager::SecretAgent
     * 当发生错误的时候，调用父类的sendError函数，给networkmanager返回结果
     * 调试方式
     * 根据kylin-nm移植的nm-applet的agent功能代码添加日志，进行流程调试梳理，完成了该代码的开发，
     * 后续出了问题可以通过这种方式理顺流程调试解决问题。
     * 注意事项：
     * 1）所有的函数keyring、的操作都是同步的，需要其单独的agent线程，否则容易阻塞
     * 2）目前代码只是对能够测试到的情况做了代码实现，有可能会有遗漏的地方，后续出现新情况，在完善。
    */

    /*
     * 该函数从秘钥环上面获取密码，并将获取到的密码组成安全的mapmap返回，失败后通过sendError返回错误，
     * 函数的返回值将变得无效
    */
    NMVariantMapMap GetSecrets(const NMVariantMapMap &connection, const QDBusObjectPath &connection_path,
                               const QString &setting_name, const QStringList &hints, uint flags);

    /*
     * 该函数用于取消agent的操作，从测试的情况来看，目前没有发现在什么时候会出现这种情况
     * 暂时未实现，后期注意观察，出现问题后根据具体情况实现，
     * 返回值为空，如果出现错误，通过sendError返回错误
    */
    void CancelGetSecrets(const QDBusObjectPath &connection_path, const QString &setting_name);

    /*
     * 该函数将密码保存到keyring上面，要保存的密码可以在connection参数中查找到，
     * 返回值为空，如果出现错误，通过sendError返回错误
    */
    void SaveSecrets(const NMVariantMapMap &connection, const QDBusObjectPath &connection_path);
    /*
     * 该函数将密码从keyring上面删除，返回值为空，如果出现错误，通过sendError返回错误
    */
    void DeleteSecrets(const NMVariantMapMap &connection, const QDBusObjectPath &connection_path);

private:
    friend class TestKylinSecretAgent;
    QMap<QString, QString> enterpriceInfo;
    QMap<QString, QString> wirelessInfo;
    QString m_uuid;
    QDBusInterface *m_dbusInterface = nullptr;

    // 等待请求映射表，键 = "deviceName:uuid"
    QMap<QString, QSharedPointer<SecretWaitRequest>> m_waitingRequests;
    QMutex m_waitingMutex;
};

#endif // KYLINSECRETAGENT_H
