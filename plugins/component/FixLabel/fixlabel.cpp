#include "fixlabel.h"
#include <QGSettings>
#include <QApplication>

FixLabel::FixLabel(QWidget *parent) :
    QLabel(parent)
{
    const QByteArray id("org.ukui.style");
    QGSettings * fontSetting = new QGSettings(id, QByteArray(), this);
    connect(fontSetting, &QGSettings::changed,[=](QString key) {
        if ("systemFont" == key || "systemFontSize" ==key) {
            changedLabelSlot();
        }
    });
}


void FixLabel::setLabelText(QString text) {

    mStr = text;
    changedLabelSlot();
}

QString FixLabel::getText(){
    return mStr;
}

void FixLabel::changedLabelSlot() {
    QFontMetrics  fontMetrics(this->font());
    int fontSize = fontMetrics.width(mStr);
    if (fontSize > this->width()) {
        setText(fontMetrics.elidedText(mStr, Qt::ElideRight, this->width()));
        setToolTip(mStr);
    } else {
        setText(mStr);
        setToolTip("");
    }
}

#define FOREGROUND_COLOR_NORMAL qApp->palette().text().color()
QColor mixColor(const QColor &c1, const QColor &c2, qreal bias);

KyLable::KyLable(QWidget *parent) : QLabel(parent)
{
    connect(qApp, &QApplication::paletteChanged, this, &KyLable::onPaletteChanged);
    onPaletteChanged();
}

void KyLable::onPaletteChanged()
{
    m_foregroundColor = FOREGROUND_COLOR_NORMAL;
    this->repaint();
}

void KyLable::setPressColor()
{
    QColor hightlight = this->palette().color(QPalette::Active,QPalette::Highlight);
    QColor mix = this->palette().color(QPalette::Active,QPalette::BrightText);
    m_foregroundColor = mixColor(hightlight, mix, 0.2);
}

void KyLable::setHoverColor()
{
    m_foregroundColor = this->palette().color(QPalette::Active,QPalette::Highlight);
}

void KyLable::setNormalColor()
{
    m_foregroundColor = FOREGROUND_COLOR_NORMAL;
}

void KyLable::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::WindowText, m_foregroundColor);
    this->setPalette(pal);
    return QLabel::paintEvent(event);
}

void KyLable::enterEvent(QEvent *event)
{
    setHoverColor();
    this->update();
}

void KyLable::leaveEvent(QEvent *event)
{
    setNormalColor();
    this->update();
}

void KyLable::mousePressEvent(QMouseEvent *event)
{
    setPressColor();
    this->update();
    return QLabel::mousePressEvent(event);
}

void KyLable::mouseReleaseEvent(QMouseEvent *event)
{
    setHoverColor();
    this->update();
    return QLabel::mouseReleaseEvent(event);
}
