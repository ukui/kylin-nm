#ifndef ENTERPRISEWLANDIALOG_H
#define ENTERPRISEWLANDIALOG_H
#include <QDialog>
#include <QPushButton>
#include "securitypage.h"
#include "kywirelessnetitem.h"
#include "coninfo.h"

class EnterpriseWlanDialog : public QWidget
{
    Q_OBJECT
public:
    EnterpriseWlanDialog(KyWirelessNetItem &wirelessNetItem, QString device, QWidget *parent = nullptr);
    ~EnterpriseWlanDialog();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void initUI(); //初始化UI界面
    void centerToScreen();
    void initConnections();
    void initData();

private:
    KyWirelessNetItem m_wirelessNetItem;
    ConInfo m_info;
    QString m_deviceName;
    KyWirelessConnectOperation *m_connectOperation = nullptr;
    KyWirelessNetResource *m_resource = nullptr;

    /* 弹窗布局
     * Connect Enterprise WLAN············X
     * SSID··························[SSID]
     * -----------SecurityPage-------------
     * |                                  |
     * ------------------------------------
     * ····················CANCEL···CONNECT
     */
    QVBoxLayout  *m_mainLayout = nullptr;

    QHBoxLayout  *m_titleLayout = nullptr;
    QLabel       *m_titleLabel = nullptr;
    QPushButton  *m_closeBtn = nullptr;

    QHBoxLayout  *m_ssidLayout = nullptr;
    QLabel       *m_ssidTitleLabel = nullptr;
    QLabel       *m_ssidLabel = nullptr;

    SecurityPage *m_securityPage = nullptr;

    QHBoxLayout  *m_btnLayout = nullptr;
    QPushButton  *m_cancelBtn = nullptr;
    QPushButton  *m_connectBtn = nullptr;

private slots:
    void onBtnConnectClicked();
    void onEapTypeChanged(const KyEapMethodType &type);

signals:
    void enterpriseWlanDialogClose(bool);
};

#endif // ENTERPRISEWLANDIALOG_H
