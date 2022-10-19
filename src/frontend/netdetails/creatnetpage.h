#ifndef CREATNETPAGE_H
#define CREATNETPAGE_H

#include <QWidget>
#include <QLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPainter>
#include <QScrollArea>
#include <QSpacerItem>
#include <QComboBox>
#include <QLineEdit>
#include <QDebug>

#include "coninfo.h"
#include "multiplednswidget.h"

class CreatNetPage : public QFrame
{
    Q_OBJECT
public:
    CreatNetPage(QWidget *parent = nullptr);

    void constructIpv4Info(KyConnectSetting &setting);
private:
    LineEdit *connNameEdit;
    QComboBox *ipv4ConfigCombox;
    LineEdit *ipv4addressEdit;
    LineEdit *netMaskEdit;
    LineEdit *gateWayEdit;
//    LineEdit *firstDnsEdit;
//    LineEdit *secondDnsEdit;

    QFormLayout *m_detailLayout;
    QVBoxLayout *mvBoxLayout;
    QLabel *m_connNameLabel;
    QLabel *m_configLabel;
    QLabel *m_addressLabel;
    QLabel *m_maskLabel;
    QLabel *m_gateWayLabel;
//    QLabel *m_dnsLabel;
//    QLabel *m_secDnsLabel;
    MultipleDnsWidget *m_dnsWidget = nullptr;
private:
    void initUI();
    void initComponent();
    void setLineEnabled(bool check);
    void configSave();
    bool getTextEditState(QString text);
    bool checkConnectBtnIsEnabled();

    bool netMaskIsValide(QString text);
    QString getNetMaskText(QString text);

private slots:
    void setEnableOfSaveBtn();
    void configChanged(int index);
Q_SIGNALS:
    void setCreatePageState(bool);

};

#endif // CREATNETPAGE_H
