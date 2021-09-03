#include "listitem.h"
#include <QDebug>

#define MAIN_LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_SPACING 0
#define ITEM_FRAME_MARGINS 16,6,16,6
#define ITEM_FRAME_SPACING 10
#define FRAME_WIDTH 395
#define INFO_ICON_WIDTH 16
#define INFO_ICON_HEIGHT 16

ListItem::ListItem(QWidget *parent) : QFrame(parent)
{
    initUI();
    initConnection();
    m_itemFrame->installEventFilter(this);
}

ListItem::~ListItem()
{
    delete m_netButton;
    m_netButton = NULL;

    delete m_infoButton;
    m_infoButton = NULL;
}

void ListItem::setName(const QString &name)
{
    m_nameLabel->setText(name);
}

//仅无线调用，有线自己获取
void ListItem::setActive(const bool &isActive)
{
    m_netButton->setActive(isActive);
    m_isActive = isActive;
}

bool ListItem::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_itemFrame) {
        if (event->type() == QEvent::MouseButtonPress) {
            onNetButtonClicked();
        }
    }
    return QFrame::eventFilter(watched, event);
}

void ListItem::initUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_mainLayout->setSpacing(MAIN_LAYOUT_SPACING);
    this->setLayout(m_mainLayout);

    m_itemFrame = new QFrame(this);
    m_itemFrame->setFixedWidth(FRAME_WIDTH);

    m_hItemLayout = new QHBoxLayout(m_itemFrame);
    m_hItemLayout->setContentsMargins(ITEM_FRAME_MARGINS);
//    m_hItemLayout->setSpacing(ITEM_FRAME_SPACING);

    m_netButton = new RadioItemButton(m_itemFrame);
    m_nameLabel = new QLabel(m_itemFrame);
    m_infoButton = new InfoButton(m_itemFrame);
    m_infoButton->setIconSize(QSize(INFO_ICON_WIDTH,INFO_ICON_HEIGHT));

    m_hItemLayout->addWidget(m_netButton);
    m_hItemLayout->addWidget(m_nameLabel);
    m_hItemLayout->addStretch();
    m_hItemLayout->addWidget(m_infoButton);

    m_mainLayout->addWidget(m_itemFrame);
}


void ListItem::initConnection()
{
    connect(this->m_netButton, &RadioItemButton::clicked, this, &ListItem::onNetButtonClicked);
    connect(this->m_infoButton, &InfoButton::clicked, this, &ListItem::onInfoButtonClicked);
}

void ListItem::onInfoButtonClicked()
{

}

void ListItem::onNetButtonClicked()
{

}
