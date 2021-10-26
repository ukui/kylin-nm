#include "wlanmoreitem.h"

WlanMoreItem::WlanMoreItem(QWidget *parent) : ListItem(parent)
{
    setObjectName(WMI_OB_NAME);
    m_netButton->setVisible(false);
    m_infoButton->setVisible(false);
    m_nameLabel->setText(tr("More..."));
}

WlanMoreItem::~WlanMoreItem() {

}

void WlanMoreItem::onNetButtonClicked() {
    emit hiddenWlanClicked();
}

void WlanMoreItem::onRightButtonClicked() {
    // Todo
}
