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
    WlanListItem(KyWirelessNetItem &wirelessNetItem, QString device, QWidget *parent = nullptr);
    WlanListItem(QWidget *parent = nullptr);
    ~WlanListItem();

public:
    QString getSsid();

    void setWlanSignal(const int &signal);
    void setWlanState(const int &state);
    void setExpanded(const bool &expanded);

    void connectStateIsChanging();
    void connectStateChanged();

    void updateConnectState(ConnectState state);
    void updateWirelessNetSecurity(QString ssid, QString securityType);
    void updateWirelessNetItem(KyWirelessNetItem &wirelessNetItem);

protected:
    void resizeEvent(QResizeEvent *event);
    void onRightButtonClicked();
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void itemHeightChanged(const bool isExpanded, const QString &ssid);

private:
    void initWlanUI();
    void refreshIcon();

private:
    KyWirelessNetItem m_wirelessNetItem;
    KyWirelessConnectOperation *m_wirelessConnectOperation = nullptr;

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

    bool m_focusIsOut = true;
    bool m_mouseIsOut = true;

protected slots:
    void onInfoButtonClicked();

private slots:
    void onNetButtonClicked();
    void onPwdEditorTextChanged();
    void onShowPwdButtonClicked();
    void onConnectButtonClicked();
    void onMenuTriggered(QAction *action);
};

#endif // WLANLISTITEM_H
