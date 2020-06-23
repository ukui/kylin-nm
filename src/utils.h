/*
 * Copyright (C) 2020 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <QObject>
#include <QDebug>
#include <QString>

#include <QStyle>
#include <QProxyStyle>
#include <QStyleFactory>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionSlider>

#include <QFile>
#include <QApplication>


///////////////////////////////////////////////////////////////////////////////
// The Utils class, used to do some assist function

class Utils
{
public:
    Utils();

    static int m_system(char *cmd);

};


///////////////////////////////////////////////////////////////////////////////
// The UseQssFile class, set control style by using .qss file

class UseQssFile
{
public:
    static void setStyle(const QString &style)
    {
        QString styleName = ":/qss/" + style;

        QFile qss(styleName);
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
};


///////////////////////////////////////////////////////////////////////////////
// The NetworkSpeed class, get the network upload and download speed

class NetworkSpeed : public QObject
{
    Q_OBJECT
public:
    explicit NetworkSpeed(QObject *parent = nullptr);

    int getCurrentDownloadRates(char *netname,long int * save_rate,long int * tx_rate); //获取当前的流量，参数为将获取到的流量保
};




///////////////////////////////////////////////////////////////////////////////
// The CustomStyle class, inherit from class QProxyStyle, to change control style customize

class CustomStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit CustomStyle(const QString &proxyStyleName = "windows", QObject *parent = nullptr);


    virtual void drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = nullptr) const;

    virtual void drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
    virtual void drawItemPixmap(QPainter *painter, const QRect &rectangle, int alignment, const QPixmap &pixmap) const;
    virtual void drawItemText(QPainter *painter, const QRect &rectangle, int alignment, const QPalette &palette, bool enabled, const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const;

    virtual void drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
    virtual QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *option) const;
    virtual QStyle::SubControl hitTestComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, const QPoint &position, const QWidget *widget = nullptr) const;
    virtual QRect itemPixmapRect(const QRect &rectangle, int alignment, const QPixmap &pixmap) const;
    virtual QRect itemTextRect(const QFontMetrics &metrics, const QRect &rectangle, int alignment, bool enabled, const QString &text) const;
    //virtual int layoutSpacing(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation, const QStyleOption *option, const QWidget *widget);
    virtual int pixelMetric(QStyle::PixelMetric metric, const QStyleOption *option = nullptr, const QWidget *widget = nullptr) const;

    virtual void polish(QWidget *widget);
    virtual void polish(QApplication *application);
    virtual void polish(QPalette &palette);
    virtual void unpolish(QWidget *widget);
    virtual void unpolish(QApplication *application);

    virtual QSize sizeFromContents(QStyle::ContentsType type, const QStyleOption *option, const QSize &contentsSize, const QWidget *widget = nullptr) const;
    virtual QIcon standardIcon(QStyle::StandardPixmap standardIcon, const QStyleOption *option, const QWidget *widget) const;
    virtual QPalette standardPalette() const;

    virtual int	styleHint(QStyle::StyleHint hint, const QStyleOption *option = nullptr, const QWidget *widget = nullptr, QStyleHintReturn *returnData = nullptr) const;

    virtual QRect subControlRect(QStyle::ComplexControl control, const QStyleOptionComplex *option, QStyle::SubControl subControl, const QWidget *widget = nullptr) const;

    virtual QRect subElementRect(QStyle::SubElement element, const QStyleOption *option, const QWidget *widget = nullptr) const;

signals:

public slots:
};

#endif // UTILS_H
