#include "infobutton.h"
#include <QEvent>
#include <QPainter>
#include <QApplication>
#include <QDebug>

#define BUTTON_SIZE 36,36
#define ICON_SIZE 16,16
#define BACKGROUND_COLOR QColor(0,0,0,0)
#define FOREGROUND_COLOR_NORMAL qApp->palette().text().color()
#define FOREGROUND_COLOR_BRIGHTTEXT qApp->palette().brightText().color()
#define FOREGROUND_COLOR_HIGHLIGHT qApp->palette().highlight().color()
#define OUTER_PATH 10,10,16,16
#define INNER_PATH 11,11,14,14
#define TEXT_POS 16,7,16,16,0

#define BUTTON_SIZE 36,36

#define THEME_SCHAME "org.ukui.style"
#define COLOR_THEME "styleName"

static inline qreal mixQreal(qreal a, qreal b, qreal bias)
{
    return a + (b - a) * bias;
}


QColor mixColor(const QColor &c1, const QColor &c2, qreal bias)
{
    if (bias <= 0.0) {
        return c1;
    }
    if (bias >= 1.0) {
        return c2;
    }
    if (qIsNaN(bias)) {
        return c1;
    }

    qreal r = mixQreal(c1.redF(),   c2.redF(),   bias);
    qreal g = mixQreal(c1.greenF(), c2.greenF(), bias);
    qreal b = mixQreal(c1.blueF(),  c2.blueF(),  bias);
    qreal a = mixQreal(c1.alphaF(), c2.alphaF(), bias);

    return QColor::fromRgbF(r, g, b, a);
}
InfoButton::InfoButton(QWidget *parent) : QPushButton(parent)
{
    initUI();
    const QByteArray style_id(THEME_SCHAME);
    if (QGSettings::isSchemaInstalled(style_id)) {
        m_styleGsettings = new QGSettings(style_id);
        connect(m_styleGsettings, &QGSettings::changed, this, &InfoButton::onGSettingChaned);
    } else {
        qDebug() << "Gsettings interface \"org.ukui.style\" is not exist!";
    }
}

void InfoButton::initUI()
{
    this->setFixedSize(BUTTON_SIZE);
    m_backgroundColor = BACKGROUND_COLOR;
    m_foregroundColor = FOREGROUND_COLOR_NORMAL;
}

void InfoButton::onGSettingChaned(const QString &key)
{
    if (key == COLOR_THEME) {
        m_foregroundColor = FOREGROUND_COLOR_NORMAL;
        this->repaint();
    }
}

void InfoButton::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::Base, m_backgroundColor);
    pal.setColor(QPalette::Text, m_foregroundColor);

    QPainterPath cPath;
    cPath.addRect(0, 0, ICON_SIZE);
    cPath.addEllipse(0, 0, ICON_SIZE);

    QPainterPath outerPath;
    outerPath.addEllipse(OUTER_PATH);

    QPainterPath innerPath;
    innerPath.addEllipse(INNER_PATH);
    outerPath -= innerPath;

    QPainter painter(this);
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);

    painter.setBrush(pal.color(QPalette::Base));
    painter.drawPath(cPath);

    painter.fillPath(outerPath, pal.color(QPalette::Text));
    painter.setPen(m_foregroundColor);
    QFont font("Noto Sans CJK SC", 11, QFont::Normal, false);
    painter.setFont(font);
    painter.drawText(TEXT_POS, "i");
}

void InfoButton::enterEvent(QEvent *event)
{
    m_foregroundColor = FOREGROUND_COLOR_HIGHLIGHT;
    this->repaint();
}

void InfoButton::leaveEvent(QEvent *event)
{
    m_foregroundColor = FOREGROUND_COLOR_NORMAL;
    this->repaint();
}

void InfoButton::mousePressEvent(QMouseEvent *event)
{
    m_foregroundColor = mixColor(FOREGROUND_COLOR_HIGHLIGHT, FOREGROUND_COLOR_BRIGHTTEXT, 0.2);
    this->repaint();
    return QPushButton::mousePressEvent(event);
}

void InfoButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_foregroundColor = mixColor(FOREGROUND_COLOR_HIGHLIGHT, FOREGROUND_COLOR_BRIGHTTEXT, 0.2);
    this->repaint();
    return QPushButton::mouseReleaseEvent(event);
}
