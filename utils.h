#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QDebug>
#include <QStyle>
#include <QProxyStyle>
#include <QStyleFactory>
#include <QWidget>
#include <QPainter>
#include <QStyleOptionSlider>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/*!
 * \brief The NetworkSpeed class
 * \details
 * 该类通过获取前后时间的流量差，计算单位时间的网络速度
 */
class NetworkSpeed : public QObject
{
    Q_OBJECT
public:
    explicit NetworkSpeed(QObject *parent = nullptr);

    int getCurrentDownloadRates(char *netname,long int * save_rate,long int * tx_rate); //获取当前的流量，参数为将获取到的流量保
};




/*!
 * \brief The CustomStyle class
 * \details
 * 自定义QStyle
 * 基于QProxyStyle，默认使用QProxyStyle的实例绘制控件，你需要针对某一个控件重新实现若干对应的接口。
 * QProxyStyle可以从现有的qt style实例化，我们只需要知道这个style的名字即可。
 * 这种做法带来了不错的扩展性和自由度，因为我们不需要将某个style的代码直接引入我们的项目中，
 * 也能够“继承”这个style类进行二次开发。
 *
 * 下面的方法展现了QStyle的所有的接口，使用QStyle进行控件的绘制使得qt应用能够进行风格的切换，
 * 从而达到不修改项目源码却对应用外观产生巨大影响的效果。
 *
 * \note
 * 需要注意QStyle与QSS并不兼容，因为QSS本身其实上也是QStyle的一种实现，对一个控件而言，本身理论上只能
 * 在同一时间调用唯一一个QStyle进行绘制。
 */
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
