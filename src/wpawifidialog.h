/*
 * Copyright (C) 2020 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */
#ifndef WPAWIFIDIALOG_H
#define WPAWIFIDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include <QThread>
#include <QSettings>
#include <QSharedPointer>
#include <QTableView>
#include <QListView>
#include <QModelIndex>
#include <QList>

#define WINDOW_WIDTH 360
#define WINDOW_HEIGHT_PEAP 540
#define WINDOW_HEIGHT_TLP 670
#define WINDOW_HEIGHT_ELSE 360

class MainWindow;

typedef QList<QObject*> QObjectList;

namespace Ui {
class WpaWifiDialog;
}

class UpConnThread : public QThread
{
    Q_OBJECT

public:
    explicit UpConnThread(const QString &, const QString &);
    ~UpConnThread();

public:
    void run();
    QString conn_name = 0;
    QString m_user = 0;
    QString m_pwd = 0;

Q_SIGNALS:
    void connRes(int respond);
};

/**
 * @brief The wifiProperty struct
 * 目前只写了802-1x,企业wifi的配置，可增加通用属性配置
 */
struct WifiConfig
{
    QString connectName;

    /*802-1x属性*/
    QString eap;
    QString identity;
    QString anonymousIdentity;
    QString caCert;//ca证书配置路径
    QString clientCert;//用户证书配置路径
    QString privateKey;//用户私钥配置路径
};

class WpaWifiDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WpaWifiDialog(QWidget *parent = nullptr, MainWindow *mw = 0, QString conname = 0);
    explicit WpaWifiDialog(QWidget *parent = nullptr, MainWindow *mw = 0, WifiConfig *wifiConfig = nullptr);
    ~WpaWifiDialog();

private:
    Ui::WpaWifiDialog *ui;
    void initUI(); //初始化UI界面
    void initCombox(); //初始化所有下拉框
    void initConnect(); //初始化连接
    void getPwdFlag(); //获取是否每次询问密码
    bool setPwdFlag(const int&); //设置是否每次询问密码

    /*初始化CA证书的配置界面*/
    void initCAConfigUI();

    /*初始化用户证书、私钥和密码的配置界面*/
    void initUserConfig4TLSUI();

    /*清空布局 delete all item of layout*/
    void deleteAllItemOfLayout(QLayout *layout);

protected:
    void paintEvent(QPaintEvent *event);

private:
    int pwd_flag = 2; //是否每次询问密码，0保存密码，2询问，默认不保存密码

    QString connectionName;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

    bool isPress;
    QPoint winPos;
    QPoint dragPos;

    QStringList wifi_info;
    bool has_config;
    QStringList user_list;
    QString key_mgmt = "wpa-eap";

    MainWindow *mw;
    QObjectList m_frameList;

    void setEditorEnable(bool is_checking); //设置是否禁用输入
    void activateConnection();
    QStringList getWifiInfo(QString wifiName);
    bool appendWifiInfo(QString name, QString eap, QString inner, QString user, bool ask);
    bool appendWifiUser(QString name, QString user);

private:
    QWidget * mainWidget = nullptr;
    QVBoxLayout * mainLyt = nullptr; //弹窗整体布局

    QFrame * titleFrame = nullptr; //标题栏
    QHBoxLayout * titleLyt = nullptr;
    QLabel * titleLabel = nullptr;

    QFrame * nameFrame = nullptr; //网络名称
    QHBoxLayout * nameLyt = nullptr;
    QLabel * nameLabel = nullptr;
    QLineEdit * nameEditor = nullptr;

    QFrame * securityFrame = nullptr; //网络安全性
    QHBoxLayout * securityLyt = nullptr;
    QLabel * securityLabel = nullptr;
    QComboBox * securityCombox = nullptr;

    QFrame * hLine = nullptr; //分割线

    QFrame * eapFrame = nullptr; //EAP方法
    QHBoxLayout * eapLyt = nullptr;
    QLabel * eapLabel = nullptr;
    QComboBox * eapCombox = nullptr;

    QFrame * innerFrame = nullptr; //阶段2身份认证
    QHBoxLayout * innerLyt = nullptr;
    QLabel * innerLabel = nullptr;
    QComboBox * innerCombox = nullptr;

    QFrame * userFrame = nullptr; //用户名
    QHBoxLayout * userLyt = nullptr;
    QLabel * userLabel = nullptr;
    QLineEdit * userEditor = nullptr;

    QFrame * pwdFrame = nullptr; //密码
    QVBoxLayout * pwdLyt = nullptr;
    QFrame * pwdEditFrame = nullptr; //输入密码
    QHBoxLayout * pwdEditLyt = nullptr;
    QLabel * pwdLabel = nullptr;
    QLineEdit * pwdEditor = nullptr;
    QFrame * pwdShowFrame = nullptr; //显示密码
    QHBoxLayout * pwdShowLyt = nullptr;
    QCheckBox * pwdShowBtn = nullptr;
    QLabel * pwdShowLabel = nullptr;
    QFrame * askPwdFrame = nullptr; //每次询问密码
    QHBoxLayout * askPwdLyt = nullptr;
    QCheckBox * askPwdBtn = nullptr;
    QLabel * askPwdlabel = nullptr;

    QFrame * buttonFrame = nullptr; //按钮
    QHBoxLayout * buttonLyt = nullptr;
    QPushButton * cancelBtn = nullptr; //取消
    QPushButton * connectBtn = nullptr; //连接

    /**************** 802-1x layout *****************
      认证 EAP方法 TTS    QLabel QComboBox
      身份               QLabel QLineEdit
      域                 QLabel QLineEdit
      CA证书             QLabel QComboBox V
                                         无
                                         请选择文件路径
      显示密码           QCheckBox
      不需要CA证书       QCheckBox

      用户证书           QComboBox
      用户私钥           QComboBox
      用户私钥密码        QLineEdit
      显示密码           QCheckBox
      **/
    enum EapType {
        TLS = 0,
        LEAP,
        PWD,
        FAST,
        TTLS,
        PEAP
    };

    WifiConfig    *m_wifiConfig              = nullptr;

    /*身份 identity*/
    QFrame        *m_identityFrame           = nullptr;
    QHBoxLayout   *m_identityLyt             = nullptr;
    QLabel        *m_identityLabel           = nullptr;
    QLineEdit     *m_identityEditor          = nullptr;

    /*匿名身份 anonymous identity*/
    QFrame        *m_anonymousIdentityFrame  = nullptr;
    QHBoxLayout   *m_anonymousIdentityLyt    = nullptr;
    QLabel        *m_anonymousIdentityLabel  = nullptr;
    QLineEdit     *m_anonymousIdentityEditor = nullptr;

    /*域 domain*/
    QFrame        *m_domainFrame             = nullptr;
    QHBoxLayout   *m_domainLyt               = nullptr;
    QLabel        *m_domainLabel             = nullptr;
    QLineEdit     *m_domainEditor            = nullptr;

    /*CA证书 certificate*/
    QFrame        *m_CAFrame                 = nullptr;
    QHBoxLayout   *m_CALyt                   = nullptr;
    QLabel        *m_CALabel                 = nullptr;
    QComboBox     *m_CAComboBox              = nullptr;// 默认为(无)、QFileDialog

    /*不需要CA证书  no need for CA certificate*/
    QFrame        *m_isCANeededFrame         = nullptr;
    QHBoxLayout   *m_isCANeededShowLyt       = nullptr;
    QCheckBox     *m_isCANeededShowBtn       = nullptr;
    QLabel        *m_isCANeededShowLabel     = nullptr;

    /*用户证书 user certificate*/
    QFrame        *m_UserCertificateFrame    = nullptr;
    QHBoxLayout   *m_UserCertificateLyt      = nullptr;
    QLabel        *m_UserCertificateLabel    = nullptr;
    QComboBox     *m_UserCertificateComboBox = nullptr;// 默认为(无)、QFileDialog

    /*用户私钥 user private key*/
    QFrame        *m_UserPrivateKeyFrame     = nullptr;
    QHBoxLayout   *m_UserPrivateKeyLyt       = nullptr;
    QLabel        *m_UserPrivateKeyLabel     = nullptr;
    QComboBox     *m_UserPrivateKeyComboBox  = nullptr;// 默认为(无)、QFileDialog

    /*私钥密码 password for private key*/
    QFrame        *m_pwd4PrivateKeyPWDFrame     = nullptr;
    QHBoxLayout   *m_pwd4PrivateKeyPWDLyt       = nullptr;
    QLabel        *m_pwd4PrivateKeyPWDLabel     = nullptr;
    QLineEdit     *m_pwd4PrivateKeyPWDEditor    = nullptr;// 未添加用户私钥时，该编辑栏不可用
Q_SIGNALS:
    void conn_done();
    void conn_failed();

private slots:
    void slot_on_connectBtn_clicked();
    void slot_line_edit_changed();
    void changeDialog();
    void eapTypeChange();//
    void checkConnectBtnIsEnabled();
};

#endif // WPAWIFIDIALOG_H
