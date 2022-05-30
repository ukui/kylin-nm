#include "deviceframe.h"

#define LAYOUT_MARGINS 18,0,24,0
#define FRAME_HEIGHT 58
#define RADIUS 6.0

DeviceFrame::DeviceFrame(QString devName, QWidget *parent) : QFrame(parent)
{
    this->setFrameShape(QFrame::Box);
    this->setFixedHeight(FRAME_HEIGHT);
    QHBoxLayout *deviceLayout = new QHBoxLayout(this);
    deviceLayout->setContentsMargins(LAYOUT_MARGINS);
    setLayout(deviceLayout);

    deviceLabel = new QLabel(this);
    dropDownLabel = new DrownLabel(devName, this);
    deviceSwitch = new KSwitchButton(this);

    deviceLayout->addWidget(deviceLabel);
    deviceLayout->addStretch();
    deviceLayout->addWidget(dropDownLabel);
    deviceLayout->addWidget(deviceSwitch);
}

DeviceFrame::~DeviceFrame()
{

}

void DeviceFrame::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();

    QPainter painter(this);
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(pal.color(QPalette::Base));

    QRect rect = this->rect();
    QPainterPath path;
    path.addRoundedRect (rect, RADIUS, RADIUS);
    QRect temp_rect(rect.left(), rect.top() + rect.height()/2, rect.width(), rect.height()/2);
    path.addRect(temp_rect);
    painter.drawPath(path);
    QFrame::paintEvent(event);
}
