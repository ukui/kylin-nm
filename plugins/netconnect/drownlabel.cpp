#include "drownlabel.h"
#include "deviceframe.h"

#define ICONSIZE 12,12
DrownLabel::DrownLabel(QString devName, QWidget * parent) : QLabel(parent)
{
    m_devName = devName;
    setFixedSize(36,36);
    loadPixmap(isChecked);
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
        setPixmap(QIcon::fromTheme("ukui-up-symbolic", QIcon(":/img/plugins/netconnect/up.svg")).pixmap(ICONSIZE));
    } else {
        setPixmap(QIcon::fromTheme("ukui-down-symbolic", QIcon(":/img/plugins/netconnect/down.svg")).pixmap(ICONSIZE));
    }
}

void DrownLabel::mouseReleaseEvent(QMouseEvent *event)
{
    emit labelClicked();
    QWidget::mouseReleaseEvent(event);
}
