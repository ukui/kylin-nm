#include "deviceframe.h"

#define LAYOUT_MARGINS 18,0,8,0
#define FRAME_HEIGHT 58
#define LAYOUT_SPACING 16

DeviceFrame::DeviceFrame(QString devName, QWidget *parent) : QFrame(parent)
{
    this->setFrameShape(QFrame::Box);
    this->setFixedHeight(FRAME_HEIGHT);
    QHBoxLayout *deviceLayout = new QHBoxLayout(this);
    deviceLayout->setContentsMargins(LAYOUT_MARGINS);
    setLayout(deviceLayout);
    deviceLayout->setSpacing(LAYOUT_SPACING);

    deviceLabel = new QLabel(this);
    dropDownLabel = new DrownLabel(devName, this);

    deviceLayout->addWidget(deviceLabel);
    deviceLayout->addStretch();
    deviceLayout->addWidget(dropDownLabel);
}

DeviceFrame::~DeviceFrame()
{

}

