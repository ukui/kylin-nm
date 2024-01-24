/*
 * Copyright (C) 2023, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#ifndef KYLINSECRETAGENT_H
#define KYLINSECRETAGENT_H

#include <libsecret/secret.h>

#include <QObject>
//#include <NetworkManagerQt/SecretAgent>
#include "networkmanagerqt/secretagent.h"

#include "agentutils.h"

class KylinSecretAgent: public NetworkManager::SecretAgent
{
    Q_OBJECT

public:
    explicit KylinSecretAgent(const QString &id, QWidget *pWidget = nullptr, QObject *parent = nullptr);
    ~KylinSecretAgent();

private:
    QWidget *parentWidget = nullptr;
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

    QStringList getVpnPassword(QVariantMap &vpnSettingMap);
    QString getVpnDataItemByKey(QVariantMap &vpnSettingMap, QString key);
    void saveSecretForVpn(QVariantMap &vpnSetingMap,
                                QString &connectId, QString &connectUuid);

    void getSecretFromKeyring(GList *p_secretList, const QString &settingName,
                                QString connectType, NMVariantMapMap &secretMap);
    bool hintIsFind(GList *p_secretList, const QStringList &hints);
    bool connectionIsAlwaysAsk(const NMVariantMapMap &connection);
    QString getWepKeyFromId(uint wepId);


    void askSecretForLeap(QVariantMap &wirelessSecurityMap,
                          const QString &settingName, NMVariantMapMap &secretMap, QString connectionID);
    void askSecretForWep(QVariantMap &wirelessSecurityMap,
                          const QString &settingName, NMVariantMapMap &secretMap, QString connectionID);
    void askSecretForPassword(QVariantMap &securityMap, QString passwordKey,
                          const QString &settingName, NMVariantMapMap &secretMap, QString connectionID);
    void askSecretForWireless(const NMVariantMapMap &connection,
                           const QString settingName, NMVariantMapMap &secretMap);

    void askSecretForTls(QVariantMap securityMap,
                                       const QString &settingName, NMVariantMapMap &secretMap, QString connectionID);
    void askSecretWithIdentityAndPassword(QVariantMap securityMap, QString userKey,
                               QString passwordKey, const QString &settingName, NMVariantMapMap &secretMap, QString connectionID);
    void askSecretForEnterpriceWireless(const NMVariantMapMap &connection,
                                        const QString settingName, NMVariantMapMap &secretMap);
    void askSecretForVpn(const NMVariantMapMap &connection,
                                          const QString settingName, NMVariantMapMap &secretMap);
    void askForSecret(const NMVariantMapMap &connection, const QString &settingName,
                                                QString connectType, NMVariantMapMap &secretMap);

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
    QMap<QString, QString> enterpriceInfo;
    QMap<QString, QString> wirelessInfo;
};

#endif // KYLINSECRETAGENT_H
