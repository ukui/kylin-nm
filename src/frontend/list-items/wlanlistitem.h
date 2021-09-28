#ifndef WLANLISTITEM_H
#define WLANLISTITEM_H
#include "listitem.h"
#include "kywirelessnetitem.h"
#include "kywirelessnetresource.h"
#include "wlanpage.h"
#include "kywirelessconnectoperation.h"
#include <QCheckBox>
#include "kylinactiveconnectresource.h"
#include <QMenu>
#include <QAction>

#include <networkmanagerqt/wirelesssecuritysetting.h>

#define PSK_SETTING_NAME "802-11-wireless-security"

#define NORMAL_HEIGHT 48
#define EXPANDED_HEIGHT 120
#define PWD_LENGTH_LIMIT 8

class WlanListItem : public ListItem
{
    Q_OBJECT
public:
    WlanListItem(KyWirelessNetResource *resource, KyWirelessNetItem *data, QString device, QWidget *parent = nullptr);
    WlanListItem(QWidget *parent = nullptr);
    ~WlanListItem();

public:
    void setWlanSignal(const int &signal);
    void setWlanState(const int &state);
    void setExpanded(const bool &expanded);
    QString getSsid();
    KyWirelessNetItem *m_data = nullptr;
protected:
    void resizeEvent(QResizeEvent *event);
    void onRightButtonClicked();
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

signals:
    void itemHeightChanged(const QString &ssid);
    void connectButtonClicked(KyWirelessConnectSetting &connSettingInfo, const bool &isHidden);

private:
    void initWlanUI();
    void initWlanConnection();
    void refreshIcon();

private:
    KyWirelessNetResource *m_resource = nullptr;

    KyWirelessConnectOperation *m_connoperation = nullptr;
    KyActiveConnectResourse *m_connectResource = nullptr;
    bool m_hasPwd = true;
    QString m_wlanDevice;

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

    QMenu *m_menu = nullptr;

    NetworkManager::ActiveConnection::State m_state;

//    QVBoxLayout * m_mainLayout = nullptr;
//    QFrame * m_itemFrame = nullptr;
//    QHBoxLayout * m_hItemLayout = nullptr;
//    QLineEdit * m_lineEdit = nullptr;
protected slots:
    void onInfoButtonClicked();

private slots:
//    void onInfoButtonClicked();
    void onNetButtonClicked();
//    void onNameLableClicked();
    void onSecurityChanged(QString interface, QString ssid, QString securityType);
    void onPwdEditorTextChanged();
    void onShowPwdButtonClicked();
    void onConnectButtonClicked();
    void onConnectionAdd(QString deviceName, QString ssid);
    void onConnectionRemove(QString deviceName, QString ssid);
    void onWlanStatusChange(QString uuid,
                          NetworkManager::ActiveConnection::State state,
                          NetworkManager::ActiveConnection::Reason reason);
    void onMenuTriggered(QAction *action);
};

#endif // WLANLISTITEM_H
