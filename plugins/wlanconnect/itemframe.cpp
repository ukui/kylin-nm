#include "itemframe.h"
#include <QPainter>

#define LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_MARGINS 0,0,0,0
ItemFrame::ItemFrame(QString devName, QWidget *parent)
{
    deviceLanLayout = new QVBoxLayout(this);
    deviceLanLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    lanItemFrame = new QFrame(this);
    lanItemFrame->setFrameShape(QFrame::Shape::NoFrame);
    lanItemFrame->setContentsMargins(LAYOUT_MARGINS);

    lanItemLayout = new QVBoxLayout(this);
    lanItemLayout->setContentsMargins(LAYOUT_MARGINS);
    lanItemLayout->setSpacing(1);
    addWlanWidget = new AddNetBtn(true, this);

    deviceLanLayout->setSpacing(1);
    setLayout(deviceLanLayout);
    lanItemFrame->setLayout(lanItemLayout);

    deviceFrame = new DeviceFrame(devName, this);
    deviceLanLayout->addWidget(deviceFrame);
    deviceLanLayout->addWidget(lanItemFrame);
    deviceLanLayout->addWidget(addWlanWidget);

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

void ItemFrame::filletStyleChange()
{
    if (lanItemLayout->isEmpty()) {
        return;
    }

    for (int i = 0; i < lanItemLayout->count(); ++i) {
        QLayoutItem *it = lanItemLayout->itemAt(i);
        WlanItem *itemFrame = (WlanItem*)(it->widget());
        if (i != lanItemLayout->count()-1) {
            itemFrame->setHalfFillet(false);
        } else {
            itemFrame->setHalfFillet(true);
        }
    }
}
