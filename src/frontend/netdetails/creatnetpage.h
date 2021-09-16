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

class CreatNetPage : public QFrame
{
    Q_OBJECT
public:
    CreatNetPage(QWidget *parent = nullptr);

    void constructIpv4Info(KyConnectSetting &setting);
private:
    QLineEdit *connNameEdit;
    QComboBox *ipv4ConfigCombox;
    QLineEdit *ipv4addressEdit;
    QLineEdit *netMaskEdit;
    QLineEdit *gateWayEdit;
    QLineEdit *firstDnsEdit;
    QLineEdit *secondDnsEdit;

private:
    QFormLayout *m_detailLayout;
    QVBoxLayout *mvBoxLayout;
    QLabel *m_connNameLabel;
    QLabel *m_configLabel;
    QLabel *m_addressLabel;
    QLabel *m_maskLabel;
    QLabel *m_gateWayLabel;
    QLabel *m_dnsLabel;
    QLabel *m_secDnsLabel;
private:
    void initUI();
    void initComponent();
    void setLineEnabled(bool check);
    void configSave();
    bool getTextEditState(QString text);
    bool checkConnectBtnIsEnabled();

private slots:
    void setEnableOfSaveBtn();
    void configChanged(int index);
Q_SIGNALS:
    void setCreatePageState(bool);

};

#endif // CREATNETPAGE_H
