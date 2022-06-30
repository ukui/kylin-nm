#include "fixlabel.h"
#include <QGSettings>


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

