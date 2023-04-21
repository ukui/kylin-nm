#ifndef VPNADDPAGE_H
#define VPNADDPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QFormLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>

#include "kyvpnconnectoperation.h"

class vpnAddPage : public QWidget
{
    Q_OBJECT
public:
    explicit vpnAddPage(QWidget *parent = nullptr);
    void centerToScreen();
    ~vpnAddPage();

protected:
    void paintEvent(QPaintEvent *event);

private:
    void initWindow();
    void initUI();
    void initVpnTypeFrame();
    void initVpnNameFrame();
    void initVpnServerFrame();
    void initButtonFrame();

    void initConnection();

    bool checkConfimBtnIsEnabled();
    bool createVpnConnect();

    QFrame *m_vpnTypeFrame = nullptr;
    QFrame *m_vpnNameFrame = nullptr;
    QFrame *m_vpnServerFrame = nullptr;
    QFrame *m_buttonFrame = nullptr;

    QComboBox *m_vpnTypeComboBox = nullptr;
    QLineEdit *m_vpnNameLineEdit = nullptr;
    QLineEdit *m_vpnServerLineEdit = nullptr;

    QPushButton *m_confimBtn = nullptr;
    QPushButton *m_cancelBtn = nullptr;
    QCheckBox *m_autoConnectBox = nullptr;

    KyVpnConnectOperation *m_vpnConnOperation = nullptr;

private Q_SLOTS:
    void onConfimBtnClicked();
    void onSetConfimBtnEnable();

Q_SIGNALS:
    void closed();
};

#endif // VPNADDPAGE_H
