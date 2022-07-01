#ifndef WLANLISTITEM_H
#define WLANLISTITEM_H
#include "listitem.h"
#include "kywirelessnetitem.h"
#include "kywirelessnetresource.h"
#include "wlanpage.h"
#include "kywirelessconnectoperation.h"
#include <QCheckBox>
#include "kylinactiveconnectresource.h"
#include <QAction>
#include "enterprisewlandialog.h"

#include <networkmanagerqt/wirelesssecuritysetting.h>

#define PSK_SETTING_NAME "802-11-wireless-security"

#define NORMAL_HEIGHT 48
#define EXPANDED_HEIGHT 120
#define PWD_LENGTH_LIMIT 8

class WlanListItem : public ListItem
{
    Q_OBJECT
public:
    WlanListItem(KyWirelessNetItem &wirelessNetItem, QString device, bool isApMode, QWidget *parent = nullptr);
    WlanListItem(KyWirelessNetItem &wirelessNetItem, QString device, QWidget *parent = nullptr);
    WlanListItem(QWidget *parent = nullptr);
    ~WlanListItem();

public:
    QString getSsid();

    void setSignalStrength(const int &signal);
    int  getSignalStrength();

    bool isConfigured();

    void setWlanState(const int &state);
    void setExpanded(const bool &expanded);

    KyConnectState getConnectionState();
    void updateConnectState(KyConnectState state);

    void updateWirelessNetSecurity(QString ssid, QString securityType);
    void updateWirelessNetItem(KyWirelessNetItem &wirelessNetItem);

    void forgetPwd();

protected:
    void resizeEvent(QResizeEvent *event);
    void onRightButtonClicked();
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    void keyPressEvent(QKeyEvent *event);

Q_SIGNALS:
    void itemHeightChanged(const bool isExpanded, const QString &ssid);

private:
    void initWlanUI();
    void refreshIcon(bool isActivated);

private:
    KyWirelessNetItem m_wirelessNetItem;
    KyWirelessConnectOperation *m_wirelessConnectOperation = nullptr;
    EnterpriseWlanDialog *enterpriseWlanDialog = nullptr;

    bool m_hasPwd = true;
    QString m_wlanDevice;

    bool isEnterpriseWlanDialogShow = false;

    //密码输入区域的UI
    QFrame *m_pwdFrame = nullptr;
    QHBoxLayout *m_pwdFrameLyt = nullptr;

    QLineEdit * m_pwdLineEdit = nullptr;
    QHBoxLayout *m_pwdLineEditLyt = nullptr;
    QPushButton *m_showPwdButton = nullptr;
    QPushButton *m_connectButton = nullptr;

    //自动连接选择区域UI
    QFrame *m_autoConnectFrame = nullptr;
    QHBoxLayout *m_autoConnectFrameLyt = nullptr;
    QCheckBox *m_autoConnectCheckBox = nullptr;
    QLabel *m_autoConnectLabel = nullptr;

    bool m_focusIsOut = true;
    bool m_mouseIsOut = true;

    bool m_forgetConnection = false;
    bool m_isApMode = false;

protected Q_SLOTS:
    void onInfoButtonClicked();

private Q_SLOTS:
    void onNetButtonClicked();
    void onPwdEditorTextChanged();
    void onShowPwdButtonClicked();
    void onConnectButtonClicked();
    void onMenuTriggered(QAction *action);
    void onEnterpriseWlanDialogClose(bool isShow);
};

#endif // WLANLISTITEM_H
