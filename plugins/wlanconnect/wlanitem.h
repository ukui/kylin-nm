#ifndef WLANITEM_H
#define WLANITEM_H
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QVariantMap>
#include <QTimer>
#include <QDebug>
#include <QGSettings>
#include <QImage>
#include "fixlabel.h"
#include "../component/InfoButton/infobutton.h"

class WlanItem : public QPushButton
{
public:
    WlanItem(bool bAcitve, bool isLock, QWidget *parent = nullptr);
    ~WlanItem();
public:
    QLabel * iconLabel = nullptr;
    InfoButton * infoLabel = nullptr;
    FixLabel * titileLabel = nullptr;
    QLabel * statusLabel = nullptr;
    int waitPage;
    int countCurrentTime;
    QString uuid = "";
public:
    void startLoading();
    void setWaitPage(int waitPage);
    void setCountCurrentTime(int countCurrentTime);
    void waitAnimStep();
    void stopLoading();
    bool isAcitve = false;
    bool loading = false;
    bool isLock = false;
private:
    QTimer *waitTimer = nullptr;
    QGSettings *themeGsettings = nullptr;

};

#endif // WLANITEM_H
