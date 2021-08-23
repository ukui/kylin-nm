#include "lanpage.h"

LanPage::LanPage(QWidget *parent) : TabPage(parent)
{
    initLanUI();
}

void LanPage::initLanUI()
{
    m_titleLabel->setText(tr("LAN"));
    m_activatedNetLabel->setText(tr("Activated LAN"));
    m_inactivatedNetLabel->setText(tr("Inactivated LAN"));

    m_inactivatedNetListWidget = new QListWidget(m_inactivatedNetListArea);
    m_inactivatedNetListArea->setWidget(m_inactivatedNetListWidget);
    m_inactivatedNetListArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_inactivatedNetListArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_inactivatedNetListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivatedNetListWidget->setSpacing(NET_LIST_SPACING);
    m_inactivatedNetListWidget->setFrameShape(QFrame::Shape::NoFrame);
}
