#include "wlanmoreitem.h"

WlanMoreItem::WlanMoreItem(QWidget *parent) : ListItem(parent)
{
    setObjectName(WMI_OB_NAME);
    m_netButton->setVisible(false);
    m_infoButton->setVisible(false);
    m_nameLabel->setText(tr("Add Others..."));
}

WlanMoreItem::~WlanMoreItem() {

}

void WlanMoreItem::onNetButtonClicked() {
    emit hiddenWlanClicked();
}

void WlanMoreItem::onRightButtonClicked() {
    // Todo
}

void WlanMoreItem::onMenuTriggered(QAction *action) {

}
