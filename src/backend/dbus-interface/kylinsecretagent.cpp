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

#include <QDebug>
#include <QMessageBox>
#include <QThread>

#include "networkmanagerqt/vpnsetting.h"
//#include <kylin_common.h>
//#include <glib.h>
//#include <glib/gvariant.h>
#include "kylinsecretagent.h"

#include "../vpnsecrets/kylinvpnrequest.h"
#include "../vpnsecrets/kylinvpnagent.h"
#include "../../../sys-dbus-netctrl/kylin/common/kylin_async_task.h"


#define LOG_FLAG "[KylinSecretAgent]"

#define KEYRING_UUID_TAG   "connection-uuid"
#define KEYRING_SN_TAG     "setting-name"
#define KEYRING_SK_TAG     "setting-key"

static const SecretSchema network_manager_secret_schema = {
    "org.freedesktop.NetworkManager.Connection",
    SECRET_SCHEMA_DONT_MATCH_NAME,
    {
        { KEYRING_UUID_TAG, SECRET_SCHEMA_ATTRIBUTE_STRING },
        { KEYRING_SN_TAG, SECRET_SCHEMA_ATTRIBUTE_STRING },
        { KEYRING_SK_TAG, SECRET_SCHEMA_ATTRIBUTE_STRING },
        { NULL, SECRET_SCHEMA_ATTRIBUTE_STRING },
    }
};

/*
* 目前wpa3存在的问题：
* 1、所有用户可见的存储策略情况下，当密码错误的时候，networkmanager不会调用getSecret接口
* 2、当选择仅用户可见时，存储错误密码，由于传递给getSecret的flag参数一直是13，
* 导致无法弹出修改密码的框，
* 正常情况下，flag第一次是5，从keyring获取密码  第二次是7，由于密码错误，弹框获取新密码
* 3、每次询问密码正常。
*
* wep的存在的问题：
* 1、所有用户可见时，当输入错误的密码的时候，networkmanager没有调用getSecret函数，弹出密码框
* 2、仅用户可见的时候，正确的密码可以正常连接，当密码错误的时候，不调用getsecret，不会弹出密码框
* 3、每次询问密码的时候，第一次可以弹出密码框，如果输入错误的密码，有时弹出密码框，有时不弹，取决于输入的密码位数
*
*
* 企业wifi leap加密方式
* 无法选择每次询问用户
*
*/


KylinSecretAgent::KylinSecretAgent(const QString &id, QObject *parent):SecretAgent(id, parent)
{
    enterpriceInfo[PASSWORD_FLAG] = PASSWORD_KEY;
    enterpriceInfo[PIN_FLAG] = PIN_KEY;
    enterpriceInfo[PHASE2_PRIVATE_KEY_FLAG] = PHASE2_PRIVATE_KEY;
    enterpriceInfo[PRIVATE_KEY_FLAG] = PRIVATE_KEY;
    enterpriceInfo[PHASE2_CLIENT_FLAG] = PHASE2_CLIENT;
    enterpriceInfo[PHASE2_CACERT_FLAG] = PHASE2_CACERT;
    enterpriceInfo[PHASE1_AUTH_FLAG] = PHASE1_AUTH;
    enterpriceInfo[CLIENT_CERT_FLAG] = CLIENT_CERT;
    enterpriceInfo[CACERT_FLAG] = CACERT;

    wirelessInfo[PSK_FLAG] = PSK_KEY;
    wirelessInfo[LEAP_PASSWORD_FLAG] = LEAP_PASSWORD_KEY;


    m_dbusInterface = new QDBusInterface("com.kylin.network",
                                     "/com/kylin/network",
                                     "com.kylin.network",
                                     QDBusConnection::sessionBus());
    if(!m_dbusInterface->isValid()) {
        qWarning() << qPrintable(QDBusConnection::sessionBus().lastError().message());
    }

}

KylinSecretAgent::~KylinSecretAgent()
{
}


void KylinSecretAgent::onSubmitPassword(QString requestId, const QString &ssid,  const QVariantMap &secretMap)
{
    qDebug()<<Q_FUNC_INFO << __LINE__<<requestId<<ssid<<secretMap<<m_waitingRequests;

    QMutexLocker locker(&m_waitingMutex);
    auto request = m_waitingRequests.value(requestId);
    if (request) {
        // 需要在 request 自己的线程中唤醒，但 quit() 是线程安全的
        request->quit(secretMap, false);
    }
}

void KylinSecretAgent::onCancelPassword(QString requestId, const QString &ssid)
{
    qDebug()<<Q_FUNC_INFO << __LINE__<<requestId<<ssid<<m_waitingRequests;

    QMutexLocker locker(&m_waitingMutex);
    auto request = m_waitingRequests.value(requestId);
    if (request) {
        request->quit(QVariantMap(), true);
    }
}

bool KylinSecretAgent::hintIsFind(GList *p_secretList, const QStringList &hints)
{
    GList *p_iter;
    bool hintFind = false;

    if (hints.isEmpty()) {
        return true;
    }

    for (p_iter = p_secretList; p_iter != NULL; p_iter = g_list_next(p_iter)) {
        SecretItem *p_item = (SecretItem *)p_iter->data;
        SecretValue *p_secret;
        const char *p_keyName;
        GHashTable *p_attributes;

        p_secret = secret_item_get_secret(p_item);
        if (p_secret) {
            p_attributes = secret_item_get_attributes (p_item);
            p_keyName = (char *)g_hash_table_lookup (p_attributes, KEYRING_SK_TAG);
            if (!p_keyName) {
                g_hash_table_unref (p_attributes);
                secret_value_unref (p_secret);
                continue;
            }

            /* See if this property matches a given hint */
            for (int index = 0; index < hints.size(); ++index) {
                QString hintValue = hints.at(index);
                if (!g_strcmp0(hintValue.toUtf8().constData(), p_keyName)) {
                    hintFind = true;
                }
            }

            g_hash_table_unref (p_attributes);
            secret_value_unref (p_secret);
            break;
        }
    }

    return hintFind;
}

void KylinSecretAgent::getSecretFromKeyring(GList *p_secretList,
                                    const QString &settingName,
                                    QString connectType,
                                    NMVariantMapMap &secretMap)
{
    GList *p_iter = nullptr;
    QVariantMap secretKeyMap;

    for (p_iter = p_secretList; p_iter != NULL; p_iter = g_list_next(p_iter)) {
        SecretItem *p_item = (SecretItem *)p_iter->data;
        SecretValue *p_secret;
        const char *p_keyName;
        GHashTable *p_attributes;

        p_secret = secret_item_get_secret(p_item);
        if (p_secret) {
            p_attributes = secret_item_get_attributes (p_item);
            p_keyName = (char *)g_hash_table_lookup (p_attributes, KEYRING_SK_TAG);
            if (!p_keyName) {
                g_hash_table_unref (p_attributes);
                secret_value_unref (p_secret);
                continue;
            }

            if (VPN_TYPE == connectType) {
                /*
                *  目前根据测试的情况vpn只会存储密码，涉及的字段
                * vpn.secrets: password = 123456
                *  注意的问题：不知道会不会出现多个密码的问题，
                */
                QString vpnSecretValue = QString(p_keyName)
                        + "=" + QString(secret_value_get(p_secret, NULL));
                secretKeyMap[VPN_SECRET] = vpnSecretValue;
            } else {
                secretKeyMap[p_keyName] = QString(secret_value_get(p_secret, NULL));
            }
            g_hash_table_unref (p_attributes);
            secret_value_unref (p_secret);
            break;
        }
    }

    secretMap[settingName] = secretKeyMap;

    return;
}

/*
* 目前根据测试掌握的情况来看，该函数无用原因：
* 当每次都是询问的时候，keyring上面并不会存储密码，那么从keyring上面获取的密码为空
* 程序根本无法走到此处
*
* 注意事项：
*   后期根据测试遇到的情况在调整该函数。
*/
bool KylinSecretAgent::connectionIsAlwaysAsk(const NMVariantMapMap &connection)
{
    return false;
}

/*
*
* 802-11-wireless-security.key-mgmt:      sae/wpa-psk
* wpa-psk 是WPA/WPA2个人加密方式
* sae  是WPA3个人加密方式
* 涉及的字段：
* 802-11-wireless-security.key-mgmt:      sae
* 802-11-wireless-security.psk:           asdasd
* 802-11-wireless-security.psk-flags:     0（无）
*
* 该函数通过弹出密码框获取密码，并构建相应类型的密码
*/
void KylinSecretAgent::askSecretForPassword(QVariantMap &securityMap,
                                            QString passwordKey,
                                            const QString &settingName,
                                            NMVariantMapMap &secretMap,
                                            QVariantMap connectMap,
                                            uint flags,
                                            bool isWired)
{
    QString secretValue = "";

    if (secretMap.contains(settingName)) {
        QVariantMap secretKeyMap = secretMap.value(settingName);
        if (!secretKeyMap.isEmpty()) {
            secretValue = secretKeyMap.value(passwordKey).toString();
        }
    }

    if (secretValue.isEmpty()) {
        secretValue = securityMap.value(passwordKey).toString();
    }

    if (!m_dbusInterface->isValid()) {
        secretMap.clear();
        return;
    }

    QString connectionID = connectMap.value(ID_KEY).toString();
    QString interfaceName = connectMap.value(INTERFACE_NAME).toString();//网卡信息
    QString connectUuid = connectMap.value(UUID_KEY).toString();
    qDebug() << Q_FUNC_INFO << __LINE__ <<securityMap<< passwordKey<<settingName<<secretMap<<connectionID<<connectUuid<<secretValue;


    // 如果是首次请求（flags 不含 RequestNew），直接尝试从已有密码或 keyring 获取
    if (!(flags & RequestNew)) {
       // 如果密码存在，直接使用；否则可能返回空（让 NetworkManager 继续尝试）
       if (!secretValue.isEmpty()) {
           QVariantMap newSecretKeyMap = securityMap;
           newSecretKeyMap[passwordKey] = secretValue;
           secretMap[settingName] = newSecretKeyMap;
       }
       return;
    }

    // 密码错误重试：需要等待用户输入
    QString requestId = interfaceName + ":" + connectUuid;
    QSharedPointer<SecretWaitRequest> request;

    {
       QMutexLocker locker(&m_waitingMutex);
       if (m_waitingRequests.contains(requestId)) {
           qWarning() << LOG_FLAG << "Duplicate password request for" << requestId << ", ignoring.";
           secretMap.clear();
           sendError(InternalError, "Duplicate password request, please try again.");
           return;
       }

       request = QSharedPointer<SecretWaitRequest>::create();
       request->deviceName = interfaceName;
       request->ssid = connectionID;
       m_waitingRequests.insert(requestId, request);
    }

    // 发送密码错误信号给 QML（通过 DBus）
    m_dbusInterface->call("sendPasswordError", requestId, interfaceName, connectionID, secretValue);

    // 阻塞等待用户输入（在 DBus 线程中）
    request->wait();

    // 用户已响应，处理结果
    {
       QMutexLocker locker(&m_waitingMutex);
       m_waitingRequests.remove(requestId);
    }

    if (request->canceled) {
       secretMap.clear();
       sendError(UserCanceled, "User canceled password input");
    } else {
       QVariantMap newSecurityMap = securityMap;
       newSecurityMap[passwordKey] = request->password;
       secretMap[settingName] = newSecurityMap;
       qDebug()<<Q_FUNC_INFO << __LINE__ <<requestId<<secretMap;
    }
    return;
}

/*
* 802-11-wireless-security.wep-key-type:  passphrase  or key
* passphrase 是 WEP128位密码句
* key 是WEP40/128位秘钥（十六进制 或 ASCII）
* 两者对应的相同的mgmt类型：802-11-wireless-security.key-mgmt:      none
* 共用该函数，保存密码到秘钥环
* 涉及的字段
* 802-11-wireless-security.key-mgmt:      none
* 802-11-wireless-security.wep-tx-keyidx: 2
* 802-11-wireless-security.wep-key0:      --
* 802-11-wireless-security.wep-key1:      --
* 802-11-wireless-security.wep-key2:      --
* 802-11-wireless-security.wep-key3:      --
* 802-11-wireless-security.wep-key-flags: 1
* 802-11-wireless-security.wep-key-type:  passphrase
*/

QString KylinSecretAgent::getWepKeyFromId(uint wepId)
{
    QString wepKey = "";

    switch(wepId) {
        case KEY_TYPE0:
            wepKey = WEP_KEY0;
            break;
        case KEY_TYPE1:
            wepKey = WEP_KEY1;
            break;
        case KEY_TYPE2:
            wepKey = WEP_KEY2;
            break;
        case KEY_TYPE3:
            wepKey = WEP_KEY3;
            break;
        default:
            qWarning() << LOG_FLAG
                       << "the wep key id" << wepId << "is undefined.";
    }

    return wepKey;
}

void KylinSecretAgent::askSecretForWep(QVariantMap &wirelessSecurityMap,
                                            const QString &settingName,
                                            NMVariantMapMap &secretMap,
                                            QVariantMap connectMap,
                                            uint flags,
                                            bool isWired)
{
    QString wepKey = "";
    QString wepSecretValue = "";
    uint wepId = 0;

    if (wirelessSecurityMap.contains(WEP_ID_KEY)) {
        wepId = wirelessSecurityMap.value(WEP_ID_KEY).toUInt();
    }

    wepKey = getWepKeyFromId(wepId);
    if (wepKey.isEmpty()) {
        QString errorMsg = "the wep key is invalide.";
        qWarning() << LOG_FLAG << errorMsg;
        sendError(InvalidConnection, errorMsg);
        return;
    }

    if (secretMap.contains(settingName)) {
        QVariantMap secretKeyMap = secretMap.value(settingName);
        if (!secretKeyMap.isEmpty()) {
            wepSecretValue = secretKeyMap.value(wepKey).toString();
        }
    }

    if (wepSecretValue.isEmpty()) {
        wepSecretValue = wirelessSecurityMap.value(wepKey).toString();
    }

    // 首次请求：尝试从已有密码获取
    if (!(flags & RequestNew)) {

        if (!wepSecretValue.isEmpty()) {
            QVariantMap newSecurityMap = wirelessSecurityMap;
            newSecurityMap[wepKey] = wepSecretValue;
            secretMap[settingName] = newSecurityMap;
        }
        return;
    }

    if (!m_dbusInterface->isValid()) {
        secretMap.clear();
        return;
    }

    QString connectionID = connectMap.value(ID_KEY).toString();
    QString interfaceName = connectMap.value(INTERFACE_NAME).toString();//网卡信息
    QString connectUuid = connectMap.value(UUID_KEY).toString();
    qDebug() << Q_FUNC_INFO << __LINE__ <<wirelessSecurityMap<< wepKey<<settingName<<secretMap<<connectionID<<connectUuid<<wepSecretValue;

    // 密码错误重试
    QString requestId = interfaceName + ":" + connectUuid;
    QSharedPointer<SecretWaitRequest> request;

    {
        QMutexLocker locker(&m_waitingMutex);
        if (m_waitingRequests.contains(requestId)) {
            qWarning() << LOG_FLAG << "Duplicate WEP password request for" << requestId << ", ignoring.";
            secretMap.clear();
            sendError(InternalError, "Duplicate password request, please try again.");
            return;
        }

        request = QSharedPointer<SecretWaitRequest>::create();
        request->deviceName = interfaceName;
        request->ssid = connectionID;
        m_waitingRequests.insert(requestId, request);
    }

    // 发送密码错误信号（WEP 也使用普通密码错误信号）
    m_dbusInterface->call("sendPasswordError", requestId, interfaceName, connectionID, wepSecretValue);

    request->wait();

    {
        QMutexLocker locker(&m_waitingMutex);
        m_waitingRequests.remove(requestId);
    }

    if (request->canceled) {
        secretMap.clear();
        sendError(UserCanceled, "User canceled password input");
    } else {
        QVariantMap newSecurityMap = wirelessSecurityMap;
        newSecurityMap[wepKey] = request->password;
        secretMap[settingName] = newSecurityMap;
        qDebug()<<Q_FUNC_INFO << __LINE__ <<requestId<<secretMap;
    }

    return;
}

/*
* leap加密类型对应的mgmt值：
* 802-11-wireless-security.key-mgmt:      ieee8021x
* 涉及的字段
* 802-11-wireless-security.leap-username: sadasd
* 802-11-wireless-security.leap-password: sadasd
* 802-11-wireless-security.leap-password-flags:0
*
* 该函数通过弹窗获取正确的密码和用户名，需要构建相应的secretmap
*/

void KylinSecretAgent::askSecretForLeap(QVariantMap &wirelessSecurityMap,
                                            const QString &settingName,
                                            NMVariantMapMap &secretMap,
                                            QVariantMap connectMap,
                                            uint flags,
                                            bool isWired)
{
    uint leapPasswordFlag = 0;
    QString leapPassword = "";
    QString leapUserName = "";

    if (wirelessSecurityMap.contains(LEAP_USER_KEY)) {
        leapUserName = wirelessSecurityMap.value(LEAP_USER_KEY).toString();
    }

    if (wirelessSecurityMap.contains(LEAP_PASSWORD_FLAG)) {
        leapPasswordFlag = wirelessSecurityMap.value(LEAP_PASSWORD_FLAG).toUInt();
    }

    if (secretMap.contains(settingName)) {
        QVariantMap secretKeyMap = secretMap.value(settingName);
        if (!secretKeyMap.isEmpty()) {
            leapPassword = secretKeyMap.value(LEAP_PASSWORD_KEY).toString();
        }
    }

    if (leapPassword.isEmpty()) {
        leapPassword = wirelessSecurityMap.value(LEAP_PASSWORD_KEY).toString();
    }


    // 如果 flags 不包含 RequestNew（首次请求），尝试从已有密码获取
    if (!(flags & RequestNew)) {
       if (!leapPassword.isEmpty()) {
           QVariantMap newSecurityMap = wirelessSecurityMap;
           newSecurityMap[LEAP_PASSWORD_KEY] = leapPassword;
           // 用户名通常已存在，保持不变
           secretMap[settingName] = newSecurityMap;
       }
       return;
    }
    if (!m_dbusInterface->isValid()) {
        secretMap.clear();
        return;
    }

   QString connectionID = connectMap.value(ID_KEY).toString();
   QString interfaceName = connectMap.value(INTERFACE_NAME).toString();//网卡信息
   QString connectUuid = connectMap.value(UUID_KEY).toString();
   // 密码错误重试：需要等待用户输入新密码
   QString requestId = interfaceName + ":" + connectUuid;
   QSharedPointer<SecretWaitRequest> request;

   {
       QMutexLocker locker(&m_waitingMutex);
       if (m_waitingRequests.contains(requestId)) {
           qWarning() << LOG_FLAG << "Duplicate LEAP password request for" << requestId << ", ignoring.";
           secretMap.clear();
           sendError(InternalError, "Duplicate password request, please try again.");
           return;
       }

       request = QSharedPointer<SecretWaitRequest>::create();
       request->deviceName = interfaceName;
       request->ssid = connectionID;
       m_waitingRequests.insert(requestId, request);
   }
   QVariantMap enterpriseInfo;
   enterpriseInfo[SSID_KEY] = connectionID;          // 这里connectionID实际是SSID
   enterpriseInfo[UUID_KEY] = connectUuid;
   enterpriseInfo[IDENTITY_KEY] = leapUserName;
   enterpriseInfo[PASSWORD_KEY] = leapPassword;
   enterpriseInfo[EAP_KEY] = QStringList(LEAP_EAP);
   qDebug()<<Q_FUNC_INFO << __LINE__<<requestId<<interfaceName<<connectionID<<enterpriseInfo;
   // 发送密码错误信号
   m_dbusInterface->call("sendPasswordError", requestId, interfaceName, connectionID, leapPassword, enterpriseInfo);

   {
       QMutexLocker locker(&m_waitingMutex);
       m_waitingRequests.remove(requestId);
   }

   if (request->canceled) {
       secretMap.clear();
       sendError(UserCanceled, "User canceled password input");
   } else {
       QVariantMap newSecurityMap = wirelessSecurityMap;
       newSecurityMap[LEAP_USER_KEY] = request->userName;
       newSecurityMap[LEAP_PASSWORD_KEY] = request->password;
       newSecurityMap[LEAP_PASSWORD_FLAG] = leapPasswordFlag;
       secretMap[settingName] = newSecurityMap;
       qDebug()<<Q_FUNC_INFO << __LINE__ <<requestId<<secretMap;
   }



//    if (nullptr != pwdDialog) {
//        pwdDialog->close();
//        delete pwdDialog;
//        pwdDialog = nullptr;
//    }

//    pwdDialog = new KyPasswordDialog("userAndPwd", connectionID, m_uuid, isWired);
//    pwdDialog->setUserName(leapUserName);
//    pwdDialog->setPassword(leapPassword);
//    pwdDialog->InitConnect();

////    pwdDialog->exec();
//    if(pwdDialog->exec()==QDialog::Accepted){
//        QString password = pwdDialog->getPassword();
//        QString userName = pwdDialog->getUserName();
//        QVariantMap securityMap;
//        securityMap[LEAP_PASSWORD_KEY] = password;
//        securityMap[LEAP_PASSWORD_FLAG] = leapPasswordFlag;
//        securityMap[LEAP_USER_KEY] = userName;
//        secretMap[settingName] = securityMap;

//        qDebug() << LOG_FLAG << "password:" << password << userName;
//    } else {
//        secretMap.clear();
//    }

    return;
}

/*
* 功能：获取普通wifi的密码，分为三类
* 1）wpa/wpa2/wpa3
* 2) leap
* 3) wep
*/
void KylinSecretAgent::askSecretForWireless(const NMVariantMapMap &connection,
                                            const QString settingName,
                                            NMVariantMapMap &secretMap,
                                            uint flags,
                                            bool isWired)
{
    qDebug() << LOG_FLAG <<"askSecretForWireless connection" << connection;
    QString secretType = "";

    QVariantMap wirelessSecurityMap = connection.value(settingName);
    if (wirelessSecurityMap.contains(KEY_MGMT)) {
        secretType = wirelessSecurityMap.value(KEY_MGMT).toString();
    } else {
        QString errorMsg = "can not get wireless secret type.";
        sendError(InvalidConnection, errorMsg);
        qWarning()<< LOG_FLAG << errorMsg;
        return;
    }

    QVariantMap connectMap = connection.value(CONNECTION_KEY);
    QString connectId = connectMap.value(ID_KEY).toString();

    if (PSK_MGMT == secretType || SAE_MGMT == secretType) {
        askSecretForPassword(wirelessSecurityMap, PSK_KEY, settingName, secretMap, connectMap, flags, isWired);
    } else if (WEP_MGMT == secretType || WEP128_MGMT == secretType) {
        askSecretForWep(wirelessSecurityMap, settingName, secretMap, connectMap, flags, isWired);
    } else if (LEAP_MGMT == secretType) {
        askSecretForLeap(wirelessSecurityMap, settingName, secretMap, connectMap, flags, isWired);
    } else {
        QString errorMsg =  "the secret type" + secretType + "is undefined";
        sendError(InvalidConnection, errorMsg);
        qWarning()<< LOG_FLAG << errorMsg;
        return;
    }
    if (secretMap.isEmpty()) {
        QString errorMsg =  "user cancel get wireless secret";
        sendError(UserCanceled, errorMsg);
        qWarning()<< LOG_FLAG << errorMsg;
        return;
    }
    QVariantMap newSecurityMap = secretMap.value(WIRELESS_SECURITY_NAME);
//    QVariantMap connectMap = connection.value(CONNECTION_KEY);
    QString connectUuid = connectMap.value(UUID_KEY).toString();
//    QString connectId = connectMap.value(ID_KEY).toString();
    saveWirelessSecret(newSecurityMap, connectId, connectUuid);

    return;
}

/*
功能：获取企业wifi类型为非tls的密码，涉及的字段
802-11-wireless-security.key-mgmt:      wpa-eap/ieee8021x
wpa-eap  是wpa/wpa2企业
ieee8021x 是动态wep（802.1x）需要和普通wifi的leep作区分
802-1x.eap:                             ttls/pwd/leap/fast/peap
802-1x.identity:                        steve
802-1x.password:                        23445
802-1x.password-flags:                  1
*/
void KylinSecretAgent::askSecretWithIdentityAndPassword(QVariantMap securityMap,
                                            QString userKey,
                                            QString passwordKey,
                                            const QString &settingName,
                                            NMVariantMapMap &secretMap,
                                            QVariantMap connectMap,
                                            uint flags,
                                            bool isWired)
{
    QString password = "";
    QString userName = "";

    if (securityMap.contains(userKey)) {
        userName = securityMap.value(userKey).toString();
    }

    if (secretMap.contains(settingName)) {
        QVariantMap secretKeyMap = secretMap.value(settingName);
        if (!secretKeyMap.isEmpty()) {
            password = secretKeyMap.value(passwordKey).toString();
        }
    }

    if (password.isEmpty()) {
        password = securityMap.value(passwordKey).toString();
    }

    if (!m_dbusInterface->isValid()) {
        secretMap.clear();
        return;
    }

    QString connectionID = connectMap.value(ID_KEY).toString();
    QString interfaceName = connectMap.value(INTERFACE_NAME).toString();//网卡信息
    QString connectUuid = connectMap.value(UUID_KEY).toString();
    qDebug()<<Q_FUNC_INFO << __LINE__<<securityMap<<userKey<<passwordKey<<settingName<<secretMap<<connectionID<<interfaceName<<flags;

    // 密码错误重试：需要等待用户输入
    QString requestId = interfaceName + ":" + connectUuid;
    QSharedPointer<SecretWaitRequest> request;

    {
       QMutexLocker locker(&m_waitingMutex);
       if (m_waitingRequests.contains(requestId)) {
           qWarning() << LOG_FLAG << "Duplicate password request for" << requestId << ", ignoring.";
           secretMap.clear();
           sendError(InternalError, "Duplicate password request, please try again.");
           return;
       }

       request = QSharedPointer<SecretWaitRequest>::create();
       request->deviceName = interfaceName;
       request->ssid = connectionID;
       m_waitingRequests.insert(requestId, request);
    }

    QVariantMap enterpriseInfo;
    enterpriseInfo[SSID_KEY] = connectionID;          // 这里connectionID实际是SSID
    enterpriseInfo[UUID_KEY] = connectUuid;
    enterpriseInfo[IDENTITY_KEY] = userName;
    enterpriseInfo[PASSWORD_KEY] = password;
    // 提取EAP类型（可能是列表）
    enterpriseInfo[EAP_KEY] = securityMap.value(EAP_KEY);
    // 提取阶段2认证
    if (securityMap.contains(QStringLiteral("phase2-auth"))) {
        enterpriseInfo[QStringLiteral("phase2-auth")] = securityMap.value(QStringLiteral("phase2-auth"));
    }
    if (securityMap.contains(QStringLiteral("phase2-autheap-method"))) {
        enterpriseInfo[QStringLiteral("phase2-autheap-method")] =
            securityMap.value(QStringLiteral("phase2-autheap-method"));
    }
    static const QStringList kExtra8021xForEnterpriseRetry = {
        QStringLiteral("anonymous-identity"),
        QStringLiteral("pac-file"),
        QStringLiteral("phase1-fast-provisioning"),
    };
    for (const QString &ek : kExtra8021xForEnterpriseRetry) {
        if (securityMap.contains(ek)) {
            enterpriseInfo.insert(ek, securityMap.value(ek));
        }
    }
    qDebug()<<Q_FUNC_INFO << __LINE__<<requestId<<interfaceName<<connectionID<<password<<enterpriseInfo;
    // 发送密码错误信号
    m_dbusInterface->call("sendPasswordError", requestId, interfaceName, connectionID, password, enterpriseInfo);
    // 用户已响应，处理结果
    {
       QMutexLocker locker(&m_waitingMutex);
       m_waitingRequests.remove(requestId);
    }

    if (request->canceled) {
       secretMap.clear();
       sendError(UserCanceled, "User canceled password input");
    } else {
       QVariantMap newSecurityMap = securityMap;
       newSecurityMap[userKey] = request->userName;
       newSecurityMap[passwordKey] = request->password;
       secretMap[settingName] = newSecurityMap;
       qDebug()<<Q_FUNC_INFO << __LINE__<<requestId<<secretMap;

    }

//    } else {
//        secretMap.clear();
//        sendError(UserCanceled, "User canceled retry");
//        request->quit(QString(), true);
//    }

//    if (nullptr != pwdDialog) {
//        pwdDialog->close();
//        delete pwdDialog;
//        pwdDialog = nullptr;
//    }

//    pwdDialog = new KyPasswordDialog(tr("userAndPwd"), connectionID, m_uuid, isWired);
//    pwdDialog->setUserName(userName);
//    pwdDialog->setPassword(password);
//    pwdDialog->InitConnect();

////    pwdDialog.exec();
//    if(pwdDialog->exec()==QDialog::Accepted){
//        QString newPassword = pwdDialog->getPassword();
//        QString newUserName = pwdDialog->getUserName();
//        QVariantMap newSecurityMap = securityMap;
//        newSecurityMap[userKey] = newUserName;
//        newSecurityMap[passwordKey] = newPassword;
//        secretMap[settingName] = newSecurityMap;

//        qDebug() << LOG_FLAG << "password:" << newPassword << newUserName;
//    } else {
//        secretMap.clear();
//    }

    return;
}

/*
* 功能：获取tls类型的密码，涉及的字段
802-1x.eap:                             tls
802-1x.domain-suffix-match:
802-1x.private-key-password:            --
802-1x.private-key-password-flags:      0（无）
802-1x.identity:                        steve
*/
void KylinSecretAgent::askSecretForTls(QVariantMap securityMap,
                                      const QString &settingName,
                                      NMVariantMapMap &secretMap,
                                      QVariantMap connectMap,
                                      uint flags,
                                      bool isWired)
{
    QString password = "";
    QString identity = "";
    QString domain = "";

    qDebug() << LOG_FLAG << "tls security map" << securityMap;
    if (securityMap.contains(IDENTITY_KEY)) {
        identity = securityMap.value(IDENTITY_KEY).toString();
    }

    if (securityMap.contains(DOMAIN_KEY)) {
        domain = securityMap.value(DOMAIN_KEY).toString();
        qDebug() << LOG_FLAG << "domain match" << domain;
    }

    if (secretMap.contains(settingName)) {
        QVariantMap secretKeyMap = secretMap.value(settingName);
        if (!secretKeyMap.isEmpty()) {
            password = secretKeyMap.value(PRIVATE_KEY).toString();
        }
    }

    if (password.isEmpty()) {
        password = securityMap.value(PRIVATE_KEY).toString();
    }

    if (!m_dbusInterface->isValid()) {
        secretMap.clear();
        return;
    }

    QString connectionID = connectMap.value(ID_KEY).toString();
    QString interfaceName = connectMap.value(INTERFACE_NAME).toString();//网卡信息
    QString connectUuid = connectMap.value(UUID_KEY).toString();

    qDebug()<< Q_FUNC_INFO << __LINE__ <<securityMap<<settingName<<secretMap<<connectionID<<interfaceName<<flags;
    // 密码错误重试：需要等待用户输入
    QString requestId = interfaceName + ":" + connectUuid;
    QSharedPointer<SecretWaitRequest> request;

    {
       QMutexLocker locker(&m_waitingMutex);
       if (m_waitingRequests.contains(requestId)) {
           qWarning() << LOG_FLAG << "Duplicate password request for" << requestId << ", ignoring.";
           secretMap.clear();
           sendError(InternalError, "Duplicate password request, please try again.");
           return;
       }

       request = QSharedPointer<SecretWaitRequest>::create();
       request->deviceName = interfaceName;
       request->ssid = connectionID;
       m_waitingRequests.insert(requestId, request);
    }

    // 收集所有 TLS 相关信息
    QVariantMap enterpriseInfo;
    enterpriseInfo[SSID_KEY] = connectionID;    // SSID
    enterpriseInfo[UUID_KEY] = connectUuid;
    enterpriseInfo[IDENTITY_KEY] = identity;
    enterpriseInfo[PRIVATE_KEY] = password;     // 证书密码

    // 证书路径（可能存在的键）
    QStringList certKeys = { "ca-cert", "client-cert", "private-key" };
    for (const QString &key : certKeys) {
        if (securityMap.contains(key)) {
            enterpriseInfo[key] = securityMap.value(key);
        }
    }
    // 域后缀匹配（与 NM / agentutils DOMAIN_KEY 一致，便于前端回填）
    if (!domain.isEmpty()) {
        enterpriseInfo[DOMAIN_KEY] = domain;
    }
    // EAP 类型（固定为 tls）
    enterpriseInfo[EAP_KEY] = QStringList("tls");
    // 发送密码错误信号
    m_dbusInterface->call("sendPasswordError", requestId, interfaceName, connectionID, password, enterpriseInfo);

    // 用户已响应，处理结果
    {
       QMutexLocker locker(&m_waitingMutex);
       m_waitingRequests.remove(requestId);
    }

    if (request->canceled) {
       secretMap.clear();
       sendError(UserCanceled, "User canceled password input");
    } else {
       QVariantMap newSecurityMap = securityMap;
       if (!request->userName.isEmpty()) {
           newSecurityMap[IDENTITY_KEY] = request->userName;
       }
       newSecurityMap[PRIVATE_KEY]  = request->password;
       secretMap[settingName] = newSecurityMap;
       qDebug()<<Q_FUNC_INFO << __LINE__<<requestId<<secretMap;
    }

//    if (nullptr != pwdDialog) {
//        pwdDialog->close();
//        delete pwdDialog;
//        pwdDialog = nullptr;
//    }

//    pwdDialog = new KyPasswordDialog(tr("tls"), connectionID, m_uuid, isWired);
//    pwdDialog->setUserName(identity);
//    pwdDialog->setPassword(password);
//    pwdDialog->InitConnect();

////    pwdDialog.exec();
//    if(pwdDialog->exec()==QDialog::Accepted){
//        QString newPassword = pwdDialog->getPassword();
//        QString newIdentity = pwdDialog->getUserName();
//        QVariantMap newSecurityMap = securityMap;
//        newSecurityMap[IDENTITY_KEY] = newIdentity;
//        newSecurityMap[PRIVATE_KEY] = newPassword;
//        newSecurityMap[DOMAIN_KEY] = domain;
//        secretMap[settingName] = newSecurityMap;

//        qDebug() << LOG_FLAG << "tls password:" << newPassword << newIdentity << domain;
//    } else {
//        secretMap.clear();
//    }

    return;
}

void KylinSecretAgent::askSecretForEnterpriceWireless(const NMVariantMapMap &connection,
                                                      const QString settingName,
                                                      NMVariantMapMap &secretMap,
                                                      uint flags,
                                                      bool isWired)
{
    qDebug() << LOG_FLAG <<"askSecretForEnterpriceWireless connection" << connection;
    QString secretType = "";

    QVariantMap enterpriceSecurityMap = connection.value(settingName);
    if (enterpriceSecurityMap.contains(EAP_KEY)) {
        secretType = enterpriceSecurityMap.value(EAP_KEY).toString();
    } else {
        QString errorStr = "can not get enterprice secret type.";
        sendError(InvalidConnection, errorStr);
        qWarning()<< LOG_FLAG << errorStr;
        return;
    }
    QVariantMap connectMap = connection.value(CONNECTION_KEY);
    QString connectId = connectMap.value(ID_KEY).toString();

    if (TLS_EAP == secretType) {
        askSecretForTls(enterpriceSecurityMap, settingName, secretMap, connectMap, flags, isWired);
    } else {
        askSecretWithIdentityAndPassword(enterpriceSecurityMap,
                                    IDENTITY_KEY, PASSWORD_KEY, settingName, secretMap, connectMap, flags, isWired);
    }

    if (secretMap.isEmpty()) {
        QString errorStr = "user cancel get enterprice wireless secret";
        sendError(UserCanceled, errorStr);
        qWarning()<< LOG_FLAG << errorStr;
    }

    QVariantMap newSecurityMap = secretMap.value(settingName);
//    QVariantMap connectMap = connection.value(CONNECTION_KEY);
    QString connectUuid = connectMap.value(UUID_KEY).toString();
//    QString connectId = connectMap.value(ID_KEY).toString();
    saveEnterpriceSecret(newSecurityMap, connectId, connectUuid);

    return;
}

void KylinSecretAgent::askSecretForVpn(const NMVariantMapMap &connection,
                                       const QString settingName,
                                       NMVariantMapMap &secretMap)
{
    QVariantMap newVpnSetingMap;

    QVariantMap vpnSetingMap = connection.value(VPN_NAME);
    if (vpnSetingMap.isEmpty()) {
        QString errorStr = "the VPN map of connection is invalide";
        sendError(InvalidConnection, errorStr);
        qWarning() << LOG_FLAG << errorStr;
        return;
    }

    QVariantMap vpnSecretMap = secretMap.value(VPN_NAME);
    QStringList vpnSecret = getVpnPassword(vpnSecretMap);
    if (vpnSecret.isEmpty() || vpnSecret.size() < 2) {
        vpnSecret = getVpnPassword(vpnSetingMap);
    }
    QString secretKey = vpnSecret.at(0);
    QString secretValue= vpnSecret.at(1);

//    if (nullptr != pwdDialog) {
//        pwdDialog->close();
//        delete pwdDialog;
//        pwdDialog = nullptr;
//    }

//    pwdDialog = new KyPasswordDialog("","", m_uuid);
//    pwdDialog->setPassword(secretValue);
//    pwdDialog->InitConnect();

////    pwdDialog.exec();
//    if(pwdDialog->exec()==QDialog::Accepted){
//        QString newPassword = pwdDialog->getPassword();
//        newVpnSetingMap = vpnSetingMap;
//        newVpnSetingMap[VPN_SECRET] = secretKey + "=" + newPassword;
//        secretMap[VPN_NAME] = newVpnSetingMap;

//        QVariantMap connectMap = connection.value(CONNECTION_KEY);
//        QString connectUuid = connectMap.value(UUID_KEY).toString();
//        QString connectId = connectMap.value(ID_KEY).toString();
//        saveSecretForVpn(newVpnSetingMap, connectId, connectUuid);

//        qDebug() << LOG_FLAG << "vpn password:" << newPassword ;
//    } else {
//        QString errorStr = "user canceled get vpn secret";
//        sendError(UserCanceled, errorStr);
//        qWarning() << LOG_FLAG << errorStr;
//    }

    return;
}

void KylinSecretAgent::askForSecret(const NMVariantMapMap &connection,
                                    const QString &settingName,
                                    QString connectType,
                                    NMVariantMapMap &secretMap,
                                    uint flags)
{
    QVariantMap connectMap = connection.value(CONNECTION_KEY);
    m_uuid = connectMap.value(UUID_KEY).toString();

    if (VPN_TYPE == connectType) {
        askSecretForVpn(connection, settingName, secretMap);
    } else {
        QVariantMap enterpriceSecurityMap = connection.value(ENTERPRICE_NAME);
        if (!enterpriceSecurityMap.isEmpty()) {
            askSecretForEnterpriceWireless(connection, settingName, secretMap, flags,  WIRED_TYPE == connectType);
        } else {
            QVariantMap wirelessSecurityMap = connection.value(WIRELESS_SECURITY_NAME);
            if (!wirelessSecurityMap.isEmpty()) {
                askSecretForWireless(connection, settingName, secretMap, flags, WIRED_TYPE == connectType);
            } else {
                qWarning()<< LOG_FLAG << "can not ask secret for wireless";
            }
        }
    }

    return;
}

NMVariantMapMap KylinSecretAgent::GetSecrets(const NMVariantMapMap &connection,
                                             const QDBusObjectPath &connection_path,
                                             const QString &settingName,
                                             const QStringList &hints,
                                             uint flags)
{
    //qDebug() << LOG_FLAG <<"GetSecrets connection" << connection;

    qDebug() << LOG_FLAG <<"GetSecrets setting name" << settingName;
    qDebug() << LOG_FLAG <<"GetSecrets hints" << hints;
    qDebug() << LOG_FLAG <<"GetSecrets flag" << flags;
    qDebug() << LOG_FLAG <<"GetSecrets connection" << connection;
    qDebug() << LOG_FLAG <<"GetSecrets connection_path" << connection_path;

    NMVariantMapMap secretMap;

    GHashTable *p_attributes = NULL;
    GError *p_secretError = NULL;
    GCancellable *p_cancellable = g_cancellable_new();

    QVariantMap connectMap = connection.value(CONNECTION_KEY);
    QString connectUuid = connectMap.value(UUID_KEY).toString();
    QString connectType = connectMap.value(TYPE_KEY).toString();

/*
    applet_secrets_request_new (size_t totsize,
                                NMConnection *connection,
                                gpointer request_id,
                                const char *setting_name,
                                const char **hints,
                                guint32 flags,
                                AppletAgentSecretsCallback callback,
                                gpointer callback_data,
                                AppletAgent *agent)
                                */
    /* VPN secrets get handled a bit differently */
    if (VPN_TYPE == connectType)  {


        if(!connection.value(VPN_NAME).isEmpty()){
            NetworkManager::VpnSetting vpn;
            vpn.fromMap(connection.value(VPN_NAME));
             /*vpn没有secretflags*/
            if(!vpn.secrets().isEmpty())
            {
                secretMap[VPN_NAME]=connection.value(VPN_NAME);
                //qDebug() << LOG_FLAG << "mqtest vpn secretMap!"<<secretMap;
                return secretMap;
            }
        }
        NMClient*client=nm_client_new(NULL,NULL);
        NMConnection *nmConnection=(NMConnection *)nm_client_get_connection_by_path(client,connection_path.path().toUtf8().constData());

        if(!nmConnection){
            qDebug() << LOG_FLAG << "vpn trans failed!";
            return secretMap;
        }
        //NMConnection* nmConnection=nmVariantMapMapToConnection(connection);
        // 1. 创建一个 QVector<QByteArray> 来持有 UTF-8 数据，保证生命周期
        QVector<QByteArray> utf8List;
        utf8List.reserve(hints.size());
        for (const QString &s : hints) {
            utf8List.append(s.toUtf8());
        }
        // 2. 创建一个 char* 数组，长度比实际多 1（用于 NULL 结尾）
        QVector<const char*> cstrList;
        cstrList.reserve(utf8List.size() + 1);
        for (const QByteArray &ba : utf8List) {
            cstrList.append(ba.constData());
        }
        cstrList.append(nullptr);  // NULL 终
        // 3. 获取 char** 指针
        const char **nmHints = const_cast<const char**>(cstrList.data());

        SecretsRequest * req = applet_secrets_request_new (applet_vpn_request_get_secrets_size(),
                                          nmConnection,
                                          0,
                                          settingName.toUtf8().constData(),
                                          nmHints,
                                          flags,
                                          NULL,
                                          NULL,
                                          NULL);

        if (!applet_vpn_request_get_secrets (req, &p_secretError)){
            qDebug() << LOG_FLAG << " failed vpn get secrets";
           return secretMap;
           }



         blockWaitVpnRequestResult(500,secretMap);

        return secretMap;
    }


    p_attributes = secret_attributes_build (&network_manager_secret_schema,
                                     KEYRING_UUID_TAG, connectUuid.toLocal8Bit().data(),
                                     KEYRING_SN_TAG, settingName.toLocal8Bit().data(),
                                     NULL);
    if (NULL == p_attributes) {
        qWarning() << LOG_FLAG << "create secret arributes for getting secret failed.";
        //sendError();
        return secretMap;
    }

    GList *p_secretList = secret_service_search_sync(NULL, &network_manager_secret_schema, p_attributes,
                   (SecretSearchFlags)(SECRET_SEARCH_ALL | SECRET_SEARCH_UNLOCK | SECRET_SEARCH_LOAD_SECRETS),
                   p_cancellable, &p_secretError);
    if (p_secretError) {
        if ((flags & AllowInteraction)
                 && g_error_matches (p_secretError, G_DBUS_ERROR, G_DBUS_ERROR_SERVICE_UNKNOWN)) {
            /* If the connection always asks for secrets, tolerate
             * keyring service not being present. */
            qWarning() << LOG_FLAG << "secret dbus failed";
            g_clear_error (&p_secretError);
        } else {
            QString errorStr = "read secret from keyring error msg: (" + QString(p_secretError->message) + ")";
            qWarning() << LOG_FLAG << errorStr;
            sendError(InternalError, errorStr);
            g_hash_table_unref (p_attributes);
            g_error_free (p_secretError);
            g_object_unref(p_cancellable);
            return secretMap;
        }
    }

    if ((flags & AllowInteraction) && g_list_length (p_secretList) == 0) {
        qDebug() << LOG_FLAG << "get secret list is empty";
        askForSecret(connection, settingName, connectType, secretMap, flags);
    } else {
        getSecretFromKeyring(p_secretList, settingName, connectType, secretMap);
        bool hintFind = hintIsFind(p_secretList, hints);
        if (flags) {
            if (!hintFind) {
                qDebug() << LOG_FLAG << "can not find hints int keyring";
                askForSecret(connection, settingName, connectType, secretMap, flags);
            } else if (flags & RequestNew) {
                qDebug() << LOG_FLAG <<"request new secret";
                askForSecret(connection, settingName, connectType, secretMap, flags);
            } else if ((flags & AllowInteraction)
                       && connectionIsAlwaysAsk (connection)) {
                qDebug() << LOG_FLAG << "secret is asked always";
                askForSecret(connection, settingName, connectType, secretMap, flags);
            }
        }
    }

    g_hash_table_unref (p_attributes);
    g_object_unref(p_cancellable);

    return secretMap;
}

void KylinSecretAgent::CancelGetSecrets(const QDBusObjectPath &connection_path, const QString &setting_name)
{
    //qDebug() << LOG_FLAG <<"CancelGetSecrets setting name" << setting_name;
    //qDebug() << LOG_FLAG <<"CancelGetSecrets path" << connection_path;

    return;
}

void KylinSecretAgent::saveOneSecret(QString &connectUuid,
                                 QString settingName,
                                 QString secretKey,
                                 QString &secretValue,
                                 QString &displayName)
{
    qDebug()<< LOG_FLAG << "save one secret"
        << "display name" << displayName
        << "uuid" << connectUuid
        << "setting name" << settingName
        << "psk key" << secretKey
        << "psk secret" << secretValue;

    GHashTable *attrs = NULL;
    GError *secretError = NULL;
    GCancellable *cancellable = g_cancellable_new();

    attrs = secret_attributes_build(&network_manager_secret_schema,
                            KEYRING_UUID_TAG, connectUuid.toLocal8Bit().data(),
                            KEYRING_SN_TAG, settingName.toLocal8Bit().data(),
                            KEYRING_SK_TAG, secretKey.toLocal8Bit().data(),
                            NULL);
    if (!attrs) {
        QString errorStr = "create secret attri failed";
        qWarning() << LOG_FLAG << errorStr;
        g_object_unref (cancellable);
        sendError(InternalError, errorStr);
        return;
    }


    gboolean ret = secret_password_storev_sync(&network_manager_secret_schema, attrs, NULL,
                            displayName.toLocal8Bit().data(),
                            secretValue.toLocal8Bit().data(),
                            cancellable, &secretError);
    if (!ret) {
        qWarning() << LOG_FLAG << "save secret failed";
        if (secretError) {
            QString errorStr = "secret password storev error msg: ("
                                            + QString(secretError->message) + ")";
            qWarning() << LOG_FLAG << errorStr;
            sendError(InternalError, errorStr);
            g_object_unref(secretError);
        }
    }

    g_object_unref (cancellable);
    g_hash_table_unref (attrs);

    return;
}

void KylinSecretAgent::saveConnectSecret(QVariantMap &securityMap,
                                              QString secretSettingName,
                                              QString &secretKey,
                                              QString &connectId,
                                              QString &connectUuid)
{
    //to do and test this case
    QString secretValue = "";

    if (!securityMap.contains(secretKey)) {
        qWarning() << LOG_FLAG << "the secret key" << secretKey
                   << "is not contain in security map.";
        return;
    }

    secretValue = securityMap.value(secretKey).toString();
    if (secretValue.isEmpty()) {
        qWarning() << LOG_FLAG << "the leap password is invalid.";
        return;
    }

    QString displayName = "Network secret for " + connectId
                             + "/" + secretSettingName + "/" + secretKey;
    saveOneSecret(connectUuid, secretSettingName,
                        secretKey, secretValue, displayName);

    return;
}

/*
* 802-11-wireless-security.wep-key-type:  passphrase  or key
* passphrase 是 WEP128位密码句
* key 是WEP40/128位秘钥（十六进制 或 ASCII）
* 两者对应的相同的mgmt类型：802-11-wireless-security.key-mgmt:      none
* 共用该函数，保存密码到秘钥环
* 涉及的字段
* 802-11-wireless-security.key-mgmt:      none
* 802-11-wireless-security.wep-tx-keyidx: 2
* 802-11-wireless-security.wep-key0:      --
* 802-11-wireless-security.wep-key1:      --
* 802-11-wireless-security.wep-key2:      --
* 802-11-wireless-security.wep-key3:      --
* 802-11-wireless-security.wep-key-flags: 1
* 802-11-wireless-security.wep-key-type:  passphrase
*/
void KylinSecretAgent::saveWepSecret(QVariantMap &wirelessSecurityMap,
                                     QString &connectId,
                                     QString &connectUuid)
{
    QString wepSecret = "";
    QString wepKey = "";

    uint wepKeyId = wirelessSecurityMap.value(WEP_ID_KEY).toUInt();

    wepKey = getWepKeyFromId(wepKeyId);
    if (wepKey.isEmpty()) {
        QString errorMsg = "save wep secret, but wep key is invalide.";
        qWarning() << LOG_FLAG << errorMsg;
        //sendError();
        return;
    }

    if (!wirelessSecurityMap.contains(wepKey)) {
        qWarning()<<LOG_FLAG << "wireless is not contain key" << wepKey;
        return;
    }

    wepSecret = wirelessSecurityMap.value(wepKey).toString();
    if (wepSecret.isEmpty()) {
        qDebug() << LOG_FLAG << "can not get wep key or secret";
        return;
    }

    QString displayName = "Network secret for " + connectId
                             + "/" + WIRELESS_SECURITY_NAME + "/" + wepKey;
    saveOneSecret(connectUuid, WIRELESS_SECURITY_NAME,
                                        wepKey, wepSecret, displayName);

    return;
}

/*
* 功能：保存普通wifi的密码，分为一下三类
*  1）
* * WPA/WPA2/WPA3个人 使用该函数保存密码到秘钥环
* 802-11-wireless-security.key-mgmt:      sae/wpa-psk
* wpa-psk 是WPA/WPA2个人加密方式
* sae  是WPA3个人加密方式
* 涉及的字段：
* 802-11-wireless-security.key-mgmt:      sae
* 802-11-wireless-security.psk:           asdasd
* 802-11-wireless-security.psk-flags:     0（无）
*
* 2）
* *802-11-wireless-security.key-mgmt:      ieee8021x
* 该类型对应的加密方式是leap，使用该函数保存秘钥环
* 涉及的字段
* 802-11-wireless-security.leap-username: sadasd
* 802-11-wireless-security.leap-password: sadasd
* 802-11-wireless-security.leap-password-flags:0
*
* 3）
* wep的见 函数saveWepSecret
*
*通过如下命令可以查看信息：
nmcli connection show [connection uuid] --show-secrets

通过打印日志，将SaveSecrets函数的入参connection打印出，也可以查看这些信息，
*/

void KylinSecretAgent::saveWirelessSecret(QVariantMap &wirelessSecurityMap,
                                          QString &connectId,
                                          QString &connectUuid)
{
    QMap<QString, QString>::iterator iter;

    iter = wirelessInfo.begin();
    while (iter != wirelessInfo.end()) {
        QString flagName = iter.key();
        QString secretName = iter.value();
        if (wirelessSecurityMap.contains(flagName)) {
            uint secretFlag = wirelessSecurityMap.value(flagName).toUInt();
            if (POLICY_AGENT_OWNED == secretFlag) {
                saveConnectSecret(wirelessSecurityMap, WIRELESS_SECURITY_NAME,
                                                secretName, connectId, connectUuid);
            }

            return;
        }

        iter ++;
    }

    if (wirelessSecurityMap.contains(WEP_FLAG_KEY)) {
        uint wepFLags = wirelessSecurityMap.value(WEP_FLAG_KEY).toUInt();
        if (POLICY_AGENT_OWNED == wepFLags) {
            saveWepSecret(wirelessSecurityMap, connectId, connectUuid);
        }
    }

    return;
}

/*
* 功能：保存企业级的wifi密码到keyring，涉及的主要字段：
* 802-1x.optional:                        否
802-1x.eap:                             peap
802-1x.identity:                        steve

802-1x.ca-cert-password:                --
802-1x.ca-cert-password-flags:          0（无）

802-1x.client-cert-password:            --
802-1x.client-cert-password-flags:      0（无）

802-1x.phase2-ca-cert-password:         --
802-1x.phase2-ca-cert-password-flags:   0（无）

802-1x.phase2-client-cert-password:     --
802-1x.phase2-client-cert-password-flags:0（无）

802-1x.password:                        testing
802-1x.password-flags:                  0（无）

802-1x.private-key-password:            --
802-1x.private-key-password-flags:      0（无）

802-1x.phase2-private-key-password:     --
802-1x.phase2-private-key-password-flags:0（无）

802-1x.pin:                             --
802-1x.pin-flags:                       0（无）

目前根据测试情况，企业wifi密码类型主要分为两类：
1）
802-11-wireless-security.key-mgmt:      wpa-eap/ieee8021x
wpa-eap  是wpa/wpa2企业
ieee8021x 是动态wep（802.1x）需要和普通wifi的leep作区分
802-1x.eap:                             ttls/pwd/leap/fast/peap
802-1x.identity:                        steve
802-1x.password:                        23445
802-1x.password-flags:                  1

2）
802-1x.eap:                             tls
802-1x.domain-suffix-match:
802-1x.private-key-password:            --
802-1x.private-key-password-flags:      0（无）
802-1x.identity:                        steve

对于这种密码类型nm-applet的agnet没有保存，不处理
802-1x.password-raw:                    --
802-1x.password-raw-flags:              0（无）

通过如下命令可以查看信息：
nmcli connection show [connection uuid] --show-secrets

通过打印日志，将SaveSecrets函数的入参connection打印出，也可以查看这些信息，
*/

void KylinSecretAgent::saveEnterpriceSecret(QVariantMap &enterpriceSecurityMap,
                                            QString &connectId,
                                            QString &connectUuid)
{
    QMap<QString, QString>::iterator iter;

    iter = enterpriceInfo.begin();
    while (iter != enterpriceInfo.end()) {
        QString flagName = iter.key();
        QString secretName = iter.value();
        if (enterpriceSecurityMap.contains(flagName)) {
            uint secretFlags = enterpriceSecurityMap.value(flagName).toUInt();
            if (POLICY_AGENT_OWNED == secretFlags) {
                saveConnectSecret(enterpriceSecurityMap,
                                  ENTERPRICE_NAME, secretName, connectId, connectUuid);
            }
        }

        iter++;
    }

    return;
}

/*
* 功能：将wifi的密码保存到keyring上面
* 先判断有没有企业级wifi的安全字段，有的话就从其获取要保存到keyring的密码
* 如果没有那就从普通wifi的安全字段获取要保存到keyring的密码
*
* 注意事项：
* 目前的测试情况：企业wifi和普通wifi的安全字段只有一个起作用，即互斥关系，
* 不能排除共存的情况，如果出现需要另做调整
*/

void KylinSecretAgent::saveSecretForWireless(const NMVariantMapMap &connection,
                                            QString &connectId,
                                            QString &connectUuid)
{
    QVariantMap enterpriceSecurityMap = connection.value(ENTERPRICE_NAME);
    if (!enterpriceSecurityMap.isEmpty()) {
        saveEnterpriceSecret(enterpriceSecurityMap, connectId, connectUuid);
    } else {
        QVariantMap wirelessSecurityMap = connection.value(WIRELESS_SECURITY_NAME);
        if (!wirelessSecurityMap.isEmpty()) {
            saveWirelessSecret(wirelessSecurityMap, connectId, connectUuid);
        } else {
            QString errorStr = "wireless is invalid, save secret failed.";
            sendError(InvalidConnection, errorStr);
            qWarning() << LOG_FLAG << errorStr;
        }
    }

    return;
}

QString KylinSecretAgent::getVpnDataItemByKey(QVariantMap &vpnMap, QString key)
{
    QString value = "";

    QString vpnData = vpnMap.value(VPN_DATA).toString();
    if (vpnData.isEmpty()) {
        qWarning() << LOG_FLAG << "vpn data is empty";
        return value;
    }

    QStringList vpnDataList = vpnData.split(",");
    
    // 遍历列表查找匹配的 key
    QString targetItem;
    for (const QString &item : vpnDataList) {
        QStringList keyList = item.split("=");
        if (keyList.size() >= 2) {
            // 去除 key 前后的空格
            QString currentKey = keyList[0].trimmed();
            if (currentKey == key) {
                targetItem = item;
                break;
            }
        }
    }
    
    if (targetItem.isEmpty()) {
        qWarning() << LOG_FLAG << "it can not find key" << key << "from vpn data";
        return value;
    }
    
    QStringList valueList = targetItem.split("=");
    if (valueList.size() >= 2) {
        // 去除 value 前后的空格
        value = valueList[1].trimmed();
    }

    return value;
}

QStringList KylinSecretAgent::getVpnPassword(QVariantMap &vpnMap)
{
    QStringList vpnPassword;
    QString vpnSecret;

    vpnSecret.clear();
    vpnPassword.clear();

    vpnSecret = vpnMap.value(VPN_SECRET).toString();
    if (vpnSecret.contains("=")) {
        vpnPassword = vpnSecret.split("=");
    }

    return vpnPassword;
}
/*
* 功能：将vpn的密码保存到keyring上面，vpn涉及的字段如下：

vpn.service-type:                       org.freedesktop.NetworkManager.pptp
vpn.user-name:                          --
vpn.data:                               gateway = 127.2.2.2, password-flags = 1, user = admin
vpn.secrets:                            password = 123456
vpn.persistent:                         否
vpn.timeout:                            0

其中主要的字段：
1）vpn.data中的password-flags，检查是否存于keyring
2）vpn.secrets中的password 是要存于keyring的密码，存密码时的key为password

注意事项：
vpn.secrets可能会有多个密码，目前没有遇到，等遇到的时候在调试,目前先按一个处理

通过如下命令可以查看信息：
nmcli connection show [connection uuid] --show-secrets

通过打印日志，将SaveSecrets函数的入参connection打印出，也可以查看这些信息，
***/
void KylinSecretAgent::saveSecretForVpn(QVariantMap &vpnSetingMap,
                                            QString &connectId,
                                            QString &connectUuid)
{
    if (vpnSetingMap.isEmpty()) {
        QString errorStr = "VPN map is empty";
        sendError(InvalidConnection, errorStr);
        qWarning() << LOG_FLAG << errorStr;
        return;
    }

    QString vpnFlag = getVpnDataItemByKey(vpnSetingMap, VPN_FLAG);
    if (vpnFlag.isEmpty() || POLICY_AGENT_OWNED != vpnFlag.toInt()){
        return;
    }

    QStringList vpnSecret = getVpnPassword(vpnSetingMap);
    if (vpnSecret.isEmpty() || vpnSecret.size() < 2) {
        QString errorStr = "the vpn secret is invalid";
        sendError(NoSecrets, errorStr);
        qWarning() << LOG_FLAG << errorStr;
        return;
    }

    QString secretKey = vpnSecret.at(0);
    QString secretValue= vpnSecret.at(1);

    QString vpnService = vpnSetingMap.value(VPN_SERVICE).toString();
    QString displayName = "VPN" + vpnSecret.at(0) + "secret for"
                                + connectId + vpnService + "/vpn";


    saveOneSecret(connectUuid, VPN_NAME, secretKey, secretValue, displayName);

    return;

}

void KylinSecretAgent::SaveSecrets(const NMVariantMapMap &connection,
                                   const QDBusObjectPath &connection_path)
{
    qDebug() << LOG_FLAG <<"SaveSecrets connection" << connection;

    //GError *secretError = NULL;
    GCancellable *cancellable = g_cancellable_new();

    QVariantMap connectMap = connection.value(CONNECTION_KEY);
    QString connectUuid = connectMap.value(UUID_KEY).toString();
    GError *p_secretError = NULL;

    secret_password_clear_sync(&network_manager_secret_schema,
                               cancellable, NULL,
                               KEYRING_UUID_TAG,
                               connectUuid.toLocal8Bit().data(), NULL);
    if (g_cancellable_is_cancelled(cancellable)) {
        qWarning() << LOG_FLAG << "save secret is canceled";
        g_object_unref(cancellable);
        return;
    }

    QString connectType = connectMap.value(TYPE_KEY).toString();
    QString connectId = connectMap.value(ID_KEY).toString();
    if (VPN_TYPE == connectType) {
        NMClient*client=nm_client_new(NULL,NULL);
        NMConnection *nmConnection=(NMConnection *)nm_client_get_connection_by_path(client,connection_path.path().toUtf8().constData());

        // 使用 NetworkManager::VpnSetting 解析 connection，正确处理 QDBusArgument
        NetworkManager::VpnSetting vpn;
        vpn.fromMap(connection.value(VPN_NAME));

        // 获取 VPN data 和 secrets（都是 QMap<QString, QString> 类型）
        QMap<QString, QString> vpnData = vpn.data();
        QMap<QString, QString> vpnSecrets = vpn.secrets();

        // 构造 a{sa{sv}} 结构：{"vpn": {"data": {...}, "secrets": {...}}}
        GVariantBuilder outerBuilder;
        g_variant_builder_init(&outerBuilder, G_VARIANT_TYPE("a{sa{sv}}"));

        // 构造内层 vpn 设置的 a{sv}
        GVariantBuilder vpnBuilder;
        g_variant_builder_init(&vpnBuilder, G_VARIANT_TYPE("a{sv}"));

			        // 添加 secrets 字段（a{ss} 格式）
        if (!vpnSecrets.isEmpty()) {
            GVariantBuilder secretsBuilder;
            g_variant_builder_init(&secretsBuilder, G_VARIANT_TYPE("a{ss}"));
            for (auto it = vpnSecrets.begin(); it != vpnSecrets.end(); ++it) {
                g_variant_builder_add(&secretsBuilder, "{ss}",
                    it.key().toUtf8().constData(),
                    it.value().toUtf8().constData());
            }

            GVariant *secretsVariant = g_variant_builder_end(&secretsBuilder);
            g_variant_builder_add(&vpnBuilder, "{sv}",
                VPN_SECRET,
                secretsVariant);
        }

        // 构造完整的 a{sa{sv}}
        GVariant *vpnVariant = g_variant_builder_end(&vpnBuilder);
        g_variant_builder_add(&outerBuilder, "{s@a{sv}}",
            VPN_NAME,
            vpnVariant);

        GVariant *vpnSecretsVariant = g_variant_builder_end(&outerBuilder);
        // 使用 nm_connection_update_secrets 将密钥更新到 nmConnection
        // 这样 kylinAgentVpnSave 就能获取到密钥并保存到 keyring
        qDebug() << LOG_FLAG << "mqtest update nm_connection_update_secrets";

        nm_connection_update_secrets(nmConnection,
                                      VPN_NAME,
                                      vpnSecretsVariant,
                                      &p_secretError);
        if(p_secretError){
            QString errorStr = "The SaveSecrets could not be completed ("
                                        + QString(p_secretError->message) + ")";
             qWarning() << LOG_FLAG << errorStr;
            g_error_free(p_secretError);
             return;
        }
       g_variant_unref(vpnSecretsVariant);
            

       kylinAgentVpnSave(nmConnection,connection_path.path().toUtf8().data(),cancellable);
       qDebug() << LOG_FLAG << "save vpn secret ";

    } else {
       saveSecretForWireless(connection, connectId, connectUuid);
    }

    return;
}

void KylinSecretAgent::DeleteSecrets(const NMVariantMapMap &connection,
                                     const QDBusObjectPath &connection_path)
{
    //qDebug() << LOG_FLAG <<"DeleteSecrets connection" << connection;

    GError *secretError = NULL;
    GCancellable *cancellable = g_cancellable_new();

    QVariantMap connectMap = connection.value(CONNECTION_KEY);
    QString uuid = connectMap.value(UUID_KEY).toString();

    secret_password_clear_sync(&network_manager_secret_schema,
                               cancellable,
                               &secretError,
                               KEYRING_UUID_TAG,
                               uuid.toLocal8Bit().data(),
                               NULL);

    if (g_cancellable_is_cancelled(cancellable)) {
        qWarning() << LOG_FLAG << "delete secret is canceled";
        g_object_unref(cancellable);
        return;
    }

    if (secretError != NULL) {
        QString errorStr = "The request could not be completed ("
                                    + QString(secretError->message) + ")";
        sendError(InternalError, errorStr);
        g_error_free(secretError);
    }

    g_object_unref(cancellable);
    return;
}


/**
 * @brief 将任意GVariant转换为对应的Qt QVariant类型
 * @param variant 输入的GVariant对象
 * @return 转换后的QVariant，类型不支持时返回无效QVariant
 */
QVariant gVariantToQVariant(GVariant *variant)
{
    if (!variant)
        return {};


    GVariantClass vclass = g_variant_classify(variant);
 qWarning() << "gVariantToQVariantclass:" << vclass;
    // 1. 基本数据类型
    switch (vclass) {
    case G_VARIANT_CLASS_BOOLEAN:
        return QVariant::fromValue(g_variant_get_boolean(variant));
    case G_VARIANT_CLASS_BYTE:
        return QVariant::fromValue(quint8(g_variant_get_byte(variant)));
    case G_VARIANT_CLASS_INT16:
        return QVariant::fromValue(qint16(g_variant_get_int16(variant)));
    case G_VARIANT_CLASS_UINT16:
        return QVariant::fromValue(quint16(g_variant_get_uint16(variant)));
    case G_VARIANT_CLASS_INT32:
        return QVariant::fromValue(qint32(g_variant_get_int32(variant)));
    case G_VARIANT_CLASS_UINT32:
        return QVariant::fromValue(quint32(g_variant_get_uint32(variant)));
    case G_VARIANT_CLASS_INT64:
        return QVariant::fromValue(qint64(g_variant_get_int64(variant)));
    case G_VARIANT_CLASS_UINT64:
        return QVariant::fromValue(quint64(g_variant_get_uint64(variant)));
    case G_VARIANT_CLASS_DOUBLE:
        return QVariant::fromValue(g_variant_get_double(variant));
    case G_VARIANT_CLASS_STRING:
    case G_VARIANT_CLASS_OBJECT_PATH:
    case G_VARIANT_CLASS_SIGNATURE:
        {
            const gchar *str = g_variant_get_string(variant, nullptr);
            return QVariant::fromValue(QString::fromUtf8(str));
        }
    case G_VARIANT_CLASS_VARIANT:
        {
            GVariant *nested = g_variant_get_variant(variant);
            QVariant res = gVariantToQVariant(nested);
            g_variant_unref(nested);
            return res;
        }
    case G_VARIANT_CLASS_ARRAY:
        {
            // 尝试按字典处理，支持 {sv} 和 {ss} 格式
            GVariantIter iter;
            g_variant_iter_init(&iter, variant);
            gchar *key = nullptr;
            GVariant *value = nullptr;
            const gchar *strValue = nullptr;

            // 先尝试 {sv} 格式
            if (g_variant_iter_next(&iter, "{sv}", &key, &value)) {
                g_free(key);
                g_variant_unref(value);
                // 确认为 {sv} 字典，重新完整遍历
                g_variant_iter_init(&iter, variant);
                QVariantMap map;
                while (g_variant_iter_next(&iter, "{sv}", &key, &value)) {
                    map.insert(QString::fromUtf8(key), gVariantToQVariant(value));
                    g_free(key);
                    g_variant_unref(value);
                }
                return map;
            }
        qWarning() << "gVariantToQVariantclass continue ss";
            // 再尝试 {ss} 格式（字符串到字符串的字典）
            g_variant_iter_init(&iter, variant);
            if (g_variant_iter_next(&iter, "{ss}", &key, &strValue)) {
                g_free(key);
                // 确认为 {ss} 字典，重新完整遍历
                g_variant_iter_init(&iter, variant);
                QVariantMap map;
                while (g_variant_iter_next(&iter, "{ss}", &key, &strValue)) {
                    map.insert(QString::fromUtf8(key), QString::fromUtf8(strValue));
                    g_free(key);
                }
                 qWarning() << "gVariantToQVariantclass continue ss"<<map;
                return map;
            }

            // 普通数组
            g_variant_iter_init(&iter, variant);
            QVariantList list;
            GVariant *child = nullptr;
            while ((child = g_variant_iter_next_value(&iter))) {
                list.append(gVariantToQVariant(child));
                g_variant_unref(child);
            }
            return list;
        }
    case G_VARIANT_CLASS_TUPLE:
        {
            // 元组也转为 QVariantList
            QVariantList list;
            GVariantIter iter;
            g_variant_iter_init(&iter, variant);
            GVariant *child = nullptr;
            while ((child = g_variant_iter_next_value(&iter))) {
                list.append(gVariantToQVariant(child));
                g_variant_unref(child);
            }
            return list;
        }
    case G_VARIANT_CLASS_MAYBE:
        {
            // 可能类型：提取内部值，若为空则返回无效 QVariant
            GVariant *inner = g_variant_get_maybe(variant);
            if (inner) {
                QVariant res = gVariantToQVariant(inner);
                g_variant_unref(inner);
                return res;
            }
            return QVariant();
        }
    default:
    {
        qWarning() << "Unhandled GVariant class:" << vclass;

        // 尝试按字典处理，支持 {sv} 和 {ss} 格式
        GVariantIter iter;
        g_variant_iter_init(&iter, variant);
        gchar *key = nullptr;
        GVariant *value = nullptr;
        const gchar *strValue = nullptr;

        // 先尝试 {sv} 格式
        if (g_variant_iter_next(&iter, "{sv}", &key, &value)) {
            g_free(key);
            g_variant_unref(value);
            // 确认为 {sv} 字典，重新完整遍历
            g_variant_iter_init(&iter, variant);
            QVariantMap map;
            while (g_variant_iter_next(&iter, "{sv}", &key, &value)) {
                map.insert(QString::fromUtf8(key), gVariantToQVariant(value));
                g_free(key);
                g_variant_unref(value);
            }
            return map;
        }

        // 再尝试 {ss} 格式（字符串到字符串的字典）
        g_variant_iter_init(&iter, variant);
        if (g_variant_iter_next(&iter, "{ss}", &key, &strValue)) {
            g_free(key);
            // 确认为 {ss} 字典，重新完整遍历
            g_variant_iter_init(&iter, variant);
            QVariantMap map;
            while (g_variant_iter_next(&iter, "{ss}", &key, &strValue)) {
                map.insert(QString::fromUtf8(key), QString::fromUtf8(strValue));
                g_free(key);
            }
            return map;
        }

        return {};
        }
    }
    /*
    GVariantIter iter;

    gchar *key = nullptr;
    GVariant *value = nullptr;
    const gchar *strValue = nullptr;
    // 再尝试 {ss} 格式（字符串到字符串的字典）
    g_variant_iter_init(&iter, variant);
    if (g_variant_iter_next(&iter, "{ss}", &key, &strValue)) {
        g_free(key);
        // 确认为 {ss} 字典，重新完整遍历
        g_variant_iter_init(&iter, variant);
        QVariantMap map;
        while (g_variant_iter_next(&iter, "{ss}", &key, &strValue)) {
            map.insert(QString::fromUtf8(key), QString::fromUtf8(strValue));
            g_free(key);
        }
        return map;
    }
    qWarning() << "Unhandled GVariant class" ;
    */
    return {};
}
/**
 * @brief 将NM标准连接配置(a{sa{sv}})转换为QMap<QString, QVariantMap>
 * @param connectionVariant 输入的GVariant连接配置，类型为NM_VARIANT_TYPE_CONNECTION
 * @return 转换后的Qt映射结构：key=设置名(如"vpn")，value=该设置的键值对
 */
NMVariantMapMap nmConnectionToQtMap(GVariant *connectionVariant)
{
    NMVariantMapMap result;
    if (!connectionVariant
        || !g_variant_is_of_type(connectionVariant, NM_VARIANT_TYPE_CONNECTION))
    {
        qWarning() << "Invalid NM connection variant type";
        return result;
    }

    GVariantIter iter;
    g_variant_iter_init(&iter, connectionVariant);
    const gchar *settingName = nullptr;
    GVariant *settingDict = nullptr;

    while (g_variant_iter_next(&iter, "{s@a{sv}}", &settingName, &settingDict))
    {
        QVariantMap settingMap = gVariantToQVariant(settingDict).toMap();
        
        if (g_strcmp0(settingName, VPN_NAME) == 0) {
            if (settingMap.contains(VPN_DATA)) {
                QVariant dataVar = settingMap.value(VPN_DATA);
                if (dataVar.type() != QMetaType::QString) {
                    QVariantMap dataMap = dataVar.toMap();
                    QMap<QString, QString> nmStringMap;
                    for (auto it = dataMap.begin(); it != dataMap.end(); ++it) {
                        nmStringMap.insert(it.key(), it.value().toString());
                    }
                    settingMap[VPN_DATA] = QVariant::fromValue(nmStringMap);
                }
            }
            if (settingMap.contains(VPN_SECRET)) {
                QVariant secretsVar = settingMap.value(VPN_SECRET);
                if (secretsVar.type() != QMetaType::QString) {
                    QVariantMap secretsMap = secretsVar.toMap();
                    QMap<QString, QString> nmStringMap;
                    for (auto it = secretsMap.begin(); it != secretsMap.end(); ++it) {
                        nmStringMap.insert(it.key(), it.value().toString());
                    }
                    settingMap[VPN_SECRET] = QVariant::fromValue(nmStringMap);
                }
            }
        }

        result.insert(QString::fromUtf8(settingName), settingMap);
        g_variant_unref(settingDict);
    }
    return result;
}

unsigned long long kylinGetSysRtime(void)
{
    unsigned long long runTimeMs=0;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC,&ts);
    runTimeMs=ts.tv_sec*1000+ts.tv_nsec/1000000;
    return runTimeMs;
}


/*阻塞等待vpn秘钥代理插件返回结果并设置等待超时时间*/
int KylinSecretAgent::blockWaitVpnRequestResult(unsigned int timeoutMs,NMVariantMapMap &secretMap)
{
    KylinVpnSecretsResult *pVpnSecretsResult=kylinVpnSecretsResultGet();
    unsigned long long lastTime=kylinGetSysRtime();
    int i=0;
    qDebug() << LOG_FLAG << "start blockWaitVpnRequestResult "<<timeoutMs;
    while(!pVpnSecretsResult->requestSta){

        printf("blockWaitVpnRequestResult wait %d \n",i++);
        QThread::usleep(2000);
        if((kylinGetSysRtime()-lastTime)>timeoutMs){
            qWarning() << LOG_FLAG << "blockWaitVpnRequestResult time out"<<timeoutMs;
            return -1;
        }
    }
    secretMap=nmConnectionToQtMap(pVpnSecretsResult->settings);
    //qDebug() << LOG_FLAG << "mqtest return  secretMap"<<secretMap;
    return 0;
}


