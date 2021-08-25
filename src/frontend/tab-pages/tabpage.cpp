#include "tabpage.h"

TabPage::TabPage(QWidget *parent) : QWidget(parent)
{
    initUI();
}

TabPage::~TabPage()
{
    delete m_titleDivider;
    delete m_activatedNetDivider;
    delete m_inactivatedNetDivider;
}

void TabPage::initUI()
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
    m_netSwitch = new SwitchButton(m_titleFrame);
    m_titleLayout->addWidget(m_titleLabel);
    m_titleLayout->addStretch();
    m_titleLayout->addWidget(m_netSwitch);
    m_titleDivider = new Divider(this);

    m_activatedNetFrame = new QFrame(this);
    m_activatedNetLayout = new QVBoxLayout(m_activatedNetFrame);
    m_activatedNetLayout->setContentsMargins(NET_LAYOUT_MARGINS);
    m_activatedNetLayout->setSpacing(NET_LAYOUT_SPACING);
    m_activatedNetLabel = new QLabel(m_activatedNetFrame);
    m_activatedNetLabel->setContentsMargins(TEXT_MARGINS);
    m_activatedNetListWidget = new QListWidget(m_activatedNetFrame);
    m_activatedNetListWidget->setFrameShape(QFrame::Shape::NoFrame);
    m_activatedNetListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_activatedNetListWidget->setSpacing(NET_LIST_SPACING);
    m_activatedNetLayout->addWidget(m_activatedNetLabel);
    m_activatedNetLayout->addWidget(m_activatedNetListWidget);
    m_activatedNetDivider = new Divider(this);

    m_inactivatedNetFrame = new QFrame(this);
    m_inactivatedNetLayout = new QVBoxLayout(m_inactivatedNetFrame);
    m_inactivatedNetLayout->setContentsMargins(NET_LAYOUT_MARGINS);
    m_inactivatedNetLayout->setSpacing(NET_LAYOUT_SPACING);
    m_inactivatedNetFrame->setLayout(m_inactivatedNetLayout);
    m_inactivatedNetLabel = new QLabel(m_inactivatedNetFrame);
    m_inactivatedNetLabel->setContentsMargins(TEXT_MARGINS);
    m_inactivatedNetListArea = new QScrollArea(m_inactivatedNetFrame);
    m_inactivatedNetListArea->setFrameShape(QFrame::Shape::NoFrame);
    m_inactivatedNetListArea->setWidgetResizable(true);
    m_inactivatedNetLayout->addWidget(m_inactivatedNetLabel);
    m_inactivatedNetLayout->addWidget(m_inactivatedNetListArea);
    m_inactivatedNetDivider = new Divider(this);

    m_settingsFrame = new QFrame(this);
    m_settingsLayout = new QHBoxLayout(m_settingsFrame);
    m_settingsLayout->setContentsMargins(SETTINGS_LAYOUT_MARGINS);
    m_settingsLabel = new QLabel(m_settingsFrame);
    m_settingsLabel->setText(tr("Settings"));
    m_settingsLayout->addWidget(m_settingsLabel);
    m_settingsFrame->setLayout(m_settingsLayout);

    m_mainLayout->addWidget(m_titleFrame);
    m_mainLayout->addWidget(m_titleDivider);
    m_mainLayout->addWidget(m_activatedNetFrame);
    m_mainLayout->addWidget(m_activatedNetDivider);
    m_mainLayout->addWidget(m_inactivatedNetFrame);
    m_mainLayout->addWidget(m_inactivatedNetDivider);
    m_mainLayout->addWidget(m_settingsFrame);
}

void TabPage::updateDefaultDevice(QString &deviceName)
{
    qDebug() << "updateDefaultDevice" << deviceName;
    defaultDevice = deviceName;
}

QString TabPage::getDefaultDevice()
{
    qDebug() << "getDefaultDevice" << defaultDevice;
    return defaultDevice;
}
