#include "deviceframe.h"

#define LAYOUT_MARGINS 24,0,0,0
#define FRAME_HEIGHT 36

DeviceFrame::DeviceFrame(QString devName, QWidget *parent) : QFrame(parent)
{
    this->setFixedHeight(FRAME_HEIGHT);
    QHBoxLayout *deviceLayout = new QHBoxLayout(this);
    deviceLayout->setContentsMargins(LAYOUT_MARGINS);
    setLayout(deviceLayout);

    deviceLabel = new QLabel(this);
    deviceLabel->setText(devName);
    deviceLabel->setDisabled(true);

    deviceLayout->addWidget(deviceLabel);
    deviceLayout->addStretch();
}
