#ifndef HIDDENWIFIPAGE_H
#define HIDDENWIFIPAGE_H

#include <QWidget>
#include <QDesktopWidget>
#include <QApplication>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QScrollArea>

//#include "coninfo.h"
#include "../component/Divider/divider.h"
#include "kwidget.h"
#include "kpasswordedit.h"
#include "kborderlessbutton.h"
#include "entsecuritywidget.h"
#include "kylin-nm/depend/kywirelessconnectoperation.h"

using namespace kdk;


class HiddenWiFiPage : public QWidget
{
    Q_OBJECT

public:
    HiddenWiFiPage(QString interface, bool isLockScreen, QWidget *parent = nullptr);
    ~HiddenWiFiPage();

    void getSecuType(KySecuType &secuType);

protected:
    void paintEvent(QPaintEvent *event);
    bool eventFilter(QObject *w, QEvent *event);

private:
    void initUI();
    void centerToScreen();
    void initComponent();

    void showNone();
    void showPsk();
    void showEnt();

    void setBtnEnable(bool on);
    void setJoinBtnEnable();
    void setWindowWidth(KyEapMethodType eapType);

private:
    QWidget *m_topWidget;
    QWidget *m_centerWidget;
    QWidget *m_bottomWidget;
    EntSecurityWidget *m_secuWidget;

    QLabel *m_descriptionLabel;
    QLabel *m_nameLabel;
    QLabel *m_secuTypeLabel;
    QLabel *m_pwdLabel;
    QLabel *m_emptyLabel;
    QLabel *m_checkLabel;

    LineEdit *m_nameEdit;
    QComboBox *m_secuTypeCombox;
    KPasswordEdit *m_pwdEdit = nullptr;

    QCheckBox *m_rememberCheckBox = nullptr;

    Divider *m_bottomDivider = nullptr;
//    KBorderlessButton *m_showListBtn;
    QPushButton *m_cancelBtn;
    QPushButton *m_joinBtn;

    QScrollArea *m_scrollArea;
    QVBoxLayout *m_pageLayout;
    QHBoxLayout *m_topLayout;
    QVBoxLayout *m_centerVBoxLayout;
    QHBoxLayout *m_bottomLayout;

    bool m_isJoinBtnEnable;
    bool m_isSecuOk = false;

    QString      m_deviceName;

private Q_SLOTS:
    void on_btnJoin_clicked();
    void onSecuTypeComboxIndexChanged();

Q_SIGNALS:
    void connectHidePeapConnect(KyEapMethodPeapInfo info, KyWirelessConnectSetting connSettingInfo);
    void connectHideTtlsConnect(KyEapMethodTtlsInfo info, KyWirelessConnectSetting connSettingInfo);
    void connectHideNormalConnect(KyWirelessConnectSetting connSettingInfo, KySecuType type);
};
#endif // HIDDENWIFIPAGE_H
