#ifndef CUSTOMSTYLE_H
#define CUSTOMSTYLE_H

#include <QStyle>
#include <QProxyStyle>
#include <QStyleFactory>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionSlider>

class CustomStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit CustomStyle(const QString &proxyStyleName = "windows");
    ~CustomStyle() = default;
    virtual QSize sizeFromContents(QStyle::ContentsType type, const QStyleOption *option, const QSize &contentsSize, const QWidget *widget = nullptr) const;
    virtual void drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
//    virtual void drawComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget = nullptr) const;

//    virtual void drawItemPixmap(QPainter *painter, const QRect &rectangle, int alignment, const QPixmap &pixmap) const;
//    virtual void drawItemText(QPainter *painter, const QRect &rectangle, int alignment, const QPalette &palette, bool enabled, const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const;

//    virtual void drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const;
//    virtual QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *option) const;
//    virtual QStyle::SubControl hitTestComplexControl(QStyle::ComplexControl control, const QStyleOptionComplex *option, const QPoint &position, const QWidget *widget = nullptr) const;
//    virtual QRect itemPixmapRect(const QRect &rectangle, int alignment, const QPixmap &pixmap) const;
//    virtual QRect itemTextRect(const QFontMetrics &metrics, const QRect &rectangle, int alignment, bool enabled, const QString &text) const;
//    //virtual int layoutSpacing(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation, const QStyleOption *option, const QWidget *widget);
//    virtual int pixelMetric(QStyle::PixelMetric metric, const QStyleOption *option = nullptr, const QWidget *widget = nullptr) const;

//    virtual void polish(QWidget *widget);
//    virtual void polish(QApplication *application);
//    virtual void polish(QPalette &palette);
//    virtual void unpolish(QWidget *widget);
//    virtual void unpolish(QApplication *application);

//    virtual QIcon standardIcon(QStyle::StandardPixmap standardIcon, const QStyleOption *option, const QWidget *widget) const;
//    virtual QPalette standardPalette() const;

//    virtual int	styleHint(QStyle::StyleHint hint, const QStyleOption *option = nullptr, const QWidget *widget = nullptr, QStyleHintReturn *returnData = nullptr) const;
//    virtual QRect subControlRect(QStyle::ComplexControl control, const QStyleOptionComplex *option, QStyle::SubControl subControl, const QWidget *widget = nullptr) const;
//    virtual QRect subElementRect(QStyle::SubElement element, const QStyleOption *option, const QWidget *widget = nullptr) const;
};

#endif // CUSTOMSTYLE_H
