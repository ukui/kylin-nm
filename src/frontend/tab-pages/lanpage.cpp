#include "lanpage.h"

#define MAIN_LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_SPACING 0
#define TITLE_FRAME_HEIGHT 52
#define TITLE_LAYOUT_MARGINS 24,0,0,0
#define LAN_LAYOUT_MARGINS 8,8,8,8
#define LAN_LAYOUT_SPACING 8
#define LAN_LIST_SPACING 2
#define TEXT_MARGINS 16,0,0,0
//#define SCROLL_AREA_HEIGHT 200
#define SETTINGS_LAYOUT_MARGINS 24,16,24,16

LanPage::LanPage(QWidget *parent) : QWidget(parent)
{
    initUI();
}

LanPage::~LanPage()
{
    delete m_titleDivider;
    delete m_activatedLanDivider;
    delete m_inactivatedLanDivider;
}

void LanPage::initUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_mainLayout->setSpacing(MAIN_LAYOUT_SPACING);
    this->setLayout(m_mainLayout);

    m_titleFrame = new QFrame(this);
    m_titleFrame->setFixedHeight(TITLE_FRAME_HEIGHT);
    m_titleLayout = new QHBoxLayout(m_titleFrame);
    m_titleLayout->setContentsMargins(TITLE_LAYOUT_MARGINS);
    m_titleLabel = new QLabel(m_titleFrame);
    m_titleLabel->setText(tr("LAN"));
    m_lanSwitch = new SwitchButton(m_titleFrame);
    m_titleLayout->addWidget(m_titleLabel);
    m_titleLayout->addStretch();
    m_titleLayout->addWidget(m_lanSwitch);
    m_titleDivider = new Divider(this);

    m_activatedLanFrame = new QFrame(this);
    m_activatedLanLayout = new QVBoxLayout(m_activatedLanFrame);
    m_activatedLanLayout->setContentsMargins(LAN_LAYOUT_MARGINS);
    m_activatedLanLayout->setSpacing(LAN_LAYOUT_SPACING);
    m_activatedLanLabel = new QLabel(m_activatedLanFrame);
    m_activatedLanLabel->setContentsMargins(TEXT_MARGINS);
    m_activatedLanLabel->setText(tr("Activated LAN"));
    m_activatedLanListWidget = new QListWidget(m_activatedLanFrame);
    m_activatedLanListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_activatedLanListWidget->setSpacing(LAN_LIST_SPACING);
    m_activatedLanLayout->addWidget(m_activatedLanLabel);
    m_activatedLanLayout->addWidget(m_activatedLanListWidget);
    m_activatedLanDivider = new Divider(this);

    m_inactivatedLanFrame = new QFrame(this);
    m_inactivatedLanLayout = new QVBoxLayout(m_inactivatedLanFrame);
    m_inactivatedLanLayout->setContentsMargins(LAN_LAYOUT_MARGINS);
    m_inactivatedLanLayout->setSpacing(LAN_LAYOUT_SPACING);
    m_inactivatedLanFrame->setLayout(m_inactivatedLanLayout);
    m_inactivatedLanLabel = new QLabel(m_inactivatedLanFrame);
    m_inactivatedLanLabel->setContentsMargins(TEXT_MARGINS);
    m_inactivatedLanLabel->setText(tr("Inactivated LAN"));
    m_inactivatedLanListArea = new QScrollArea(m_inactivatedLanFrame);
    m_inactivatedLanListWidget = new QListWidget(m_inactivatedLanListArea);
    m_inactivatedLanListArea->setWidget(m_inactivatedLanListWidget);
    m_inactivatedLanListArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_inactivatedLanListArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_inactivatedLanListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivatedLanListWidget->setSpacing(LAN_LIST_SPACING);
//    m_inactivated_lan_list_area->setFixedHeight(SCROLL_AREA_HEIGHT);
    m_inactivatedLanLayout->addWidget(m_inactivatedLanLabel);
    m_inactivatedLanLayout->addWidget(m_inactivatedLanListArea);
    m_inactivatedLanDivider = new Divider(this);

    m_settingsFrame = new QFrame(this);
    m_settingsLayout = new QHBoxLayout(m_settingsFrame);
    m_settingsLayout->setContentsMargins(SETTINGS_LAYOUT_MARGINS);
    m_settingsLabel = new QLabel(m_settingsFrame);
    m_settingsLabel->setText(tr("Settings"));
    m_settingsLayout->addWidget(m_settingsLabel);
    m_settingsFrame->setLayout(m_settingsLayout);

    m_mainLayout->addWidget(m_titleFrame);
    m_mainLayout->addWidget(m_titleDivider);
    m_mainLayout->addWidget(m_activatedLanFrame);
    m_mainLayout->addWidget(m_activatedLanDivider);
    m_mainLayout->addWidget(m_inactivatedLanFrame);
    m_mainLayout->addWidget(m_inactivatedLanDivider);
    m_mainLayout->addWidget(m_settingsFrame);
}
