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
}
