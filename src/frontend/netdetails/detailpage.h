#ifndef DETAILPAGE_H
#define DETAILPAGE_H
#include <QWidget>
#include <QLabel>
#include <QFormLayout>
#include <QPainter>
#include <QCheckBox>
#include <QSpacerItem>
#include <QDebug>
class DetailPage : public QFrame
{
    Q_OBJECT
public:
    DetailPage(bool isWlan, QWidget *parent = nullptr);

    void setSSID(const QString &ssid);
    void setProtocol(const QString &protocol);
    void setSecType(const QString &secType);
    void setHz(const QString &hz);
    void setChan(const QString &chan);
    void setBandWidth(const QString &brandWidth);
    void setIpv4(const QString &ipv4);
    void setIpv4Dns(const QString &ipv4Dns);
    void setIpv6(const QString &ipv6);
    void setMac(const QString &mac);

private:
    void initUI();
    void initComponent();

public:
    QLabel *mSSID;
    QLabel *mProtocol;
    QLabel *mSecType;
    QLabel *mHz;
    QLabel *mChan;
    QLabel *mBandWidth;
    QLabel *mIPV4;
    QLabel *mIPV4Dns;
    QLabel *mIPV6;
    QLabel *mMac;
    QLabel *autoConnect;

//    QWidget *autoFrame;
private:
    QVBoxLayout *layout;
    QFormLayout *mDetailLayout;
    QHBoxLayout *mAutoLayout;
    QCheckBox *forgetNetBox;
    bool mIsWlan;
private slots:
    void setNetStatus(bool checked);

};

#endif // DETAILPAGE_H
