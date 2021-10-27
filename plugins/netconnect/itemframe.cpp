#include "itemframe.h"
#define LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_MARGINS 0,0,0,8
ItemFrame::ItemFrame(QString devName, QWidget *parent) : QFrame(parent)
{
    deviceLanLayout = new QVBoxLayout(this);
    deviceLanLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    lanItemFrame = new QFrame(this);
    lanItemFrame->setFrameShape(QFrame::Shape::NoFrame);

    lanItemLayout = new QVBoxLayout(this);
    lanItemLayout->setContentsMargins(LAYOUT_MARGINS);
    lanItemLayout->setSpacing(1);
    addLanWidget = new AddNetBtn(this);

    deviceLanLayout->setSpacing(1);
    setLayout(deviceLanLayout);
    lanItemFrame->setLayout(lanItemLayout);

    deviceFrame = new DeviceFrame(devName, this);
    deviceLanLayout->addWidget(deviceFrame);
    deviceLanLayout->addWidget(lanItemFrame);
    deviceLanLayout->addWidget(addLanWidget);

    //下拉按钮
    connect(deviceFrame->dropDownLabel, &DrownLabel::labelClicked, this, &ItemFrame::onDrownLabelClicked);
}

ItemFrame::~ItemFrame()
{

}

void ItemFrame::onDrownLabelClicked()
{
    if (!deviceFrame->dropDownLabel->isChecked) {
        lanItemFrame->show();
        deviceFrame->dropDownLabel->setDropDownStatus(true);
    } else {
        lanItemFrame->hide();
        deviceFrame->dropDownLabel->setDropDownStatus(false);
    }
}
