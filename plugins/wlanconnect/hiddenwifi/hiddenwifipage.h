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

using namespace kdk;

enum KySecuType {
    NONE = 0,
    WPA_AND_WPA2_PERSONAL,
    WPA_AND_WPA2_ENTERPRISE,
    WPA3_PERSONAL,
};

class HiddenWiFiPage : public QWidget
{
    Q_OBJECT

public:
    HiddenWiFiPage(QString interface, QString name, QString uuid, bool isLockScreen, QWidget *parent = nullptr);
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
    KBorderlessButton *m_showListBtn;
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
    QString      m_name;
    QString      m_uuid;

private Q_SLOTS:
    void on_btnJoin_clicked();
    void onSecuTypeComboxIndexChanged();

};
#endif // HIDDENWIFIPAGE_H
