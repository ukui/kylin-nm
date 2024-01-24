#ifndef ENTERPRISEWLANPAGE_H
#define ENTERPRISEWLANPAGE_H

#include <QWidget>
#include <QDesktopWidget>
#include <QApplication>

#include <kylinnetworkmanager.h>
#include "entsecuritywidget.h"

class EnterpriseWlanPage : public QWidget
{
    Q_OBJECT

public:
    EnterpriseWlanPage(QString ssid, QString device, bool isLockScreen, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);

private:
    void initUI();
    void initConnections();
    void centerToScreen();

private:
    QString m_deviceName;
    QString m_ssid;

    QLabel *m_ssidTitleLabel;
    QLabel *m_ssidLabel;
    EntSecurityWidget *m_entSecurityWidget = nullptr;
    QPushButton *m_cancelBtn;
    QPushButton *m_connectBtn;
    QWidget *m_ssidWidget;
    QWidget *m_btnWidget;

    QVBoxLayout  *m_mainLayout;

private Q_SLOTS:
    void onBtnConnectClicked();
    void onEapTypeChanged(const KyEapMethodType &type);

Q_SIGNALS:
    void connectPeapConnect(KyEapMethodPeapInfo info, KyWirelessConnectSetting connSettingInfo);
    void connectTtlsConnect(KyEapMethodTtlsInfo info, KyWirelessConnectSetting connSettingInfo);

};

#endif // ENTERPRISEWLANPAGE_H
