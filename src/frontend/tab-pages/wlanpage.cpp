#include "wlanpage.h"
#include <QEvent>

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

WlanPage::WlanPage(QWidget *parent) : QWidget(parent)
{
    initUI();
}

WlanPage::~WlanPage()
{
    delete m_titleDivider;
    delete m_activatedWlanDivider;
    delete m_inactivatedWlanDivider;
}

bool WlanPage::eventFilter(QObject *w, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress) {
        if (w == m_hiddenWlanLabel) {
            //ZJP_TODO 打开隐藏WiFi添加弹窗
        } else if (w == m_settingsLabel) {
            //ZJP_TODO 打开控制面板
        }
    }
    return QWidget::eventFilter(w,e);
}

void WlanPage::initUI()
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
    m_titleLabel->setText(tr("WLAN"));
    m_lanSwitch = new SwitchButton(m_titleFrame);
    m_titleLayout->addWidget(m_titleLabel);
    m_titleLayout->addStretch();
    m_titleLayout->addWidget(m_lanSwitch);
    m_titleDivider = new Divider(this);

    m_activatedWlanFrame = new QFrame(this);
    m_activatedWlanLayout = new QVBoxLayout(m_activatedWlanFrame);
    m_activatedWlanLayout->setContentsMargins(LAN_LAYOUT_MARGINS);
    m_activatedWlanLayout->setSpacing(LAN_LAYOUT_SPACING);
    m_activatedWlanLabel = new QLabel(m_activatedWlanFrame);
    m_activatedWlanLabel->setContentsMargins(TEXT_MARGINS);
    m_activatedWlanLabel->setText(tr("Activated WLAN"));
    m_activatedWlanListWidget = new QListWidget(m_activatedWlanFrame);
    m_activatedWlanListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_activatedWlanListWidget->setSpacing(LAN_LIST_SPACING);
    m_activatedWlanLayout->addWidget(m_activatedWlanLabel);
    m_activatedWlanLayout->addWidget(m_activatedWlanListWidget);
    m_activatedWlanDivider = new Divider(this);

    m_inactivatedWlanFrame = new QFrame(this);
    m_inactivatedWlanLayout = new QVBoxLayout(m_inactivatedWlanFrame);
    m_inactivatedWlanLayout->setContentsMargins(LAN_LAYOUT_MARGINS);
    m_inactivatedWlanLayout->setSpacing(LAN_LAYOUT_SPACING);
    m_inactivatedWlanFrame->setLayout(m_inactivatedWlanLayout);
    m_inactivatedWlanLabel = new QLabel(m_inactivatedWlanFrame);
    m_inactivatedWlanLabel->setContentsMargins(TEXT_MARGINS);
    m_inactivatedWlanLabel->setText(tr("Inactivated WLAN"));
    m_inactivatedWlanListArea = new QScrollArea(m_inactivatedWlanFrame);
    m_inactivatedWlanListArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_inactivatedWlanListArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_inactivatedWlanListAreaCentralWidget = new QFrame(m_inactivatedWlanListArea);
    m_inactivatedWlanListArea->setWidget(m_inactivatedWlanListAreaCentralWidget);
    m_inactivatedWlanListAreaLayout = new QVBoxLayout(m_inactivatedWlanListAreaCentralWidget);
    m_inactivatedWlanListAreaCentralWidget->setLayout(m_inactivatedWlanListAreaLayout);
    m_inactivatedWlanListAreaLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivatedWlanListWidget = new QListWidget(m_inactivatedWlanListAreaCentralWidget);
    m_inactivatedWlanListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivatedWlanListWidget->setSpacing(LAN_LIST_SPACING);
    m_hiddenWlanWidget = new QFrame(m_inactivatedWlanListAreaCentralWidget);
    m_hiddenWlanLayout = new QHBoxLayout(m_hiddenWlanWidget);
    m_hiddenWlanLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_hiddenWlanLabel = new QLabel(m_hiddenWlanWidget);
    m_hiddenWlanLabel->setText(tr("More..."));
    m_hiddenWlanLayout->addWidget(m_hiddenWlanLabel);
    m_hiddenWlanLayout->addStretch();
    m_inactivatedWlanListAreaLayout->addWidget(m_inactivatedWlanListWidget);
    m_inactivatedWlanListAreaLayout->addWidget(m_hiddenWlanWidget);

//    m_inactivated_lan_list_area->setFixedHeight(SCROLL_AREA_HEIGHT);
    m_inactivatedWlanLayout->addWidget(m_inactivatedWlanLabel);
    m_inactivatedWlanLayout->addWidget(m_inactivatedWlanListArea);
    m_inactivatedWlanDivider = new Divider(this);

    m_settingsFrame = new QFrame(this);
    m_settingsLayout = new QHBoxLayout(m_settingsFrame);
    m_settingsLayout->setContentsMargins(SETTINGS_LAYOUT_MARGINS);
    m_settingsLabel = new QLabel(m_settingsFrame);
    m_settingsLabel->setText(tr("Settings"));
    m_settingsLayout->addWidget(m_settingsLabel);
    m_settingsFrame->setLayout(m_settingsLayout);

    m_mainLayout->addWidget(m_titleFrame);
    m_mainLayout->addWidget(m_titleDivider);
    m_mainLayout->addWidget(m_activatedWlanFrame);
    m_mainLayout->addWidget(m_activatedWlanDivider);
    m_mainLayout->addWidget(m_inactivatedWlanFrame);
    m_mainLayout->addWidget(m_inactivatedWlanDivider);
    m_mainLayout->addWidget(m_settingsFrame);
}
