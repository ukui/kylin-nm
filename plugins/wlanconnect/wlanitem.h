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
#include "infobutton.h"

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
    QString uuid = "";

    void setHalfFillet(bool flag) {useHalfFillet = flag; repaint();}
public:
    void startLoading();
    void stopLoading();
    bool isAcitve = false;
    bool loading = false;
    bool isLock = false;



protected:
    void paintEvent(QPaintEvent *event);

private:
    QTimer *waitTimer = nullptr;
    QGSettings *themeGsettings = nullptr;
    bool useHalfFillet = false;
    QList<QIcon> loadIcons;
    int currentIconIndex=0;

private slots:
    void updateIcon();

};

#endif // WLANITEM_H
