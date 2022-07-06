#ifndef JOINHIDDENWIFIPAGE_H
#define JOINHIDDENWIFIPAGE_H

#include <QScrollArea>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "coninfo.h"
#include "kywirelessconnectoperation.h"
#include "securitypage.h"
#include "divider.h"
#include "kwidget.h"
#include "kdialog.h"
#include "kborderlessbutton.h"

using namespace kdk;

class JoinHiddenWiFiPage : public KDialog
{
    Q_OBJECT

public:
    JoinHiddenWiFiPage(QString devName, KDialog *parent = nullptr);
    ~JoinHiddenWiFiPage();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void initUI();
    void initComponent();

    void setJoinBtnEnable();

private:
    KyWirelessConnectOperation *m_wirelessConnOpration = nullptr;
    QString m_devName;

    QWidget *m_topWidget;
    QWidget *m_centerWidget;
    QWidget *m_bottomWidget;
    SecurityPage *m_secuWidget;

    QLabel *m_descriptionLabel;
    QLabel *m_nameLabel;
    LineEdit *m_nameEdit;

    QLabel *m_emptyLabel;
    QLabel *m_checkLabel;
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

    bool m_isJoinBtnEnable = false;
    bool m_isSecuOk = false;
    ConInfo      m_info;

private slots:
    void onBtnJoinClicked();
    void onBtnShowListClicked();
    void onSecuTypeChanged(const KySecuType &type);
    void onEapTypeChanged(const KyEapMethodType &type);

signals:
    void hiddenWiFiPageClose(QString);
    void showWlanList(int type);
};

#endif // JOINHIDDENWIFIPAGE_H
