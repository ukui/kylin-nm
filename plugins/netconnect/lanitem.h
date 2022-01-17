#ifndef LANITEM_H
#define LANITEM_H
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QVariantMap>
#include <QTimer>
#include <QGSettings>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include "fixlabel.h"
#include "../component/InfoButton/infobutton.h"

class LanItem : public QPushButton
{
public:
    LanItem(bool isAcitve, QWidget *parent = nullptr);
    ~LanItem();
public:
    QLabel * iconLabel = nullptr;
    InfoButton * infoLabel = nullptr;
    FixLabel * titileLabel = nullptr;
    QLabel * statusLabel = nullptr;

public:
    void startLoading();
    void stopLoading();

    bool loading = false;
    bool isAcitve = false;

    QString uuid;
    QString dbusPath;

protected:
    void paintEvent(QPaintEvent *);

private:
    QTimer *waitTimer = nullptr;
    QGSettings *themeGsettings = nullptr;
    QList<QIcon> loadIcons;
        int currentIconIndex=0;

private slots:
        void updateIcon();

};

#endif // LANITEM_H
