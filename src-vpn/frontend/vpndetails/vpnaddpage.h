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

#include "kyvpnconnectoperation.h"

#define VPNADDPAGE_MAINLAYOUT_MARGINS 0, 12, 0, 12
#define VPNADDPAGE_ITEM_MARGINS 24, 0, 24, 0
#define VPNADDPAGE_NULLMAGINS 0, 0, 0, 0

#define VPNADDPAGE_SIZE 520,312
#define VPNADDPAGE_FRAME_FIXEDSIZE 520, 60
#define VPNADDPAGE_BUTTON_FIXEDSIZE 96, 36

#define VPNADDPAGE_NULLSPACE 0
#define VPNADDPAGE_BUTTON_SPACE 16
#define VPNADDPAGE_NAME_MAX_LENGTH 32
#define VPNADDPAGE_LABLE_FIXEDWIDTH 112
#define VPNADDPAGE_COMBOBOX_FIXEDWIDTH 360
#define VPNADDPAGE_INPUTBOX_FIXEDWIDTH 360

#define VPNDETAILPAGE_ITEM_MARGINS 24, 0, 24, 0
#define VPNDETAILPAGE_NULLMAGINS 0, 0, 0, 0

#define VPNDETAILPAGE_FRAME_FIXEDSIZE 520, 60
#define VPNDETAILPAGE_BUTTON_FIXEDSIZE 96, 36

#define VPNDETAILPAGE_NULLSPACE 0
#define VPNDETAILPAGE_BUTTON_SPACE 16



class vpnAddPage : public QWidget
{
    Q_OBJECT
public:
    explicit vpnAddPage(QWidget *parent = nullptr);
    void centerToScreen();
    ~vpnAddPage();

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

private:
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
