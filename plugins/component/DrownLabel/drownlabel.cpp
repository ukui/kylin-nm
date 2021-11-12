#include "drownlabel.h"
#include "deviceframe.h"

#define ICONSIZE 12,12
DrownLabel::DrownLabel(QString devName, QWidget * parent) : QLabel(parent)
{
    m_devName = devName;
    setFixedSize(36,36);
    loadPixmap(isChecked);
    this->setProperty("useIconHighlightEffect", 0x2);
}

DrownLabel::~DrownLabel()
{

}
void DrownLabel::setDropDownStatus(bool status)
{
    isChecked = status;
    loadPixmap(isChecked);
}

void DrownLabel::loadPixmap(bool isChecked)
{
    if (isChecked) {
        setPixmap(QIcon::fromTheme("ukui-up-symbolic").pixmap(ICONSIZE));
    } else {
        setPixmap(QIcon::fromTheme("ukui-down-symbolic").pixmap(ICONSIZE));
    }
}

void DrownLabel::mouseReleaseEvent(QMouseEvent *event)
{
    emit labelClicked();
    QWidget::mouseReleaseEvent(event);
}
