#include "deviceframe.h"

#define LAYOUT_MARGINS 18,0,24,0
#define FRAME_HEIGHT 58

DeviceFrame::DeviceFrame(QString devName, QWidget *parent) : QFrame(parent)
{
    this->setFrameShape(QFrame::Box);
    this->setFixedHeight(FRAME_HEIGHT);
    QHBoxLayout *deviceLayout = new QHBoxLayout(this);
    deviceLayout->setContentsMargins(LAYOUT_MARGINS);
    setLayout(deviceLayout);

    deviceLabel = new QLabel(this);
    dropDownLabel = new DrownLabel(devName, this);
    deviceSwitch = new SwitchButton(this);

    deviceLayout->addWidget(deviceLabel);
    deviceLayout->addStretch();
    deviceLayout->addWidget(dropDownLabel);
    deviceLayout->addWidget(deviceSwitch);
}

DeviceFrame::~DeviceFrame()
{

}

