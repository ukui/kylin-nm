#ifndef ENTERPRISEWLANPAGE_H
#define ENTERPRISEWLANPAGE_H

#include <QWidget>
#include <QDesktopWidget>
#include <QApplication>

#include <kylin-nm/kylinnetworkmanager.h>
#include "entsecuritywidget.h"

class EnterpriseWlanPage : public QWidget
{
    Q_OBJECT

public:
    EnterpriseWlanPage(KyWirelessNetItem &wirelessNetItem, QString device, bool isLockScreen, QWidget *parent = nullptr);
    ~EnterpriseWlanPage();
protected:
    void paintEvent(QPaintEvent *event);

private:
    void initUI();
    void initConnections();
    void initData();
    void centerToScreen();

private:
    KyWirelessNetItem m_wirelessNetItem;
//    ConInfo m_info;
    QString m_deviceName;
    KyWirelessConnectOperation *m_connectOperation = nullptr;
    KyWirelessNetResource *m_resource = nullptr;

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

};

#endif // ENTERPRISEWLANPAGE_H
