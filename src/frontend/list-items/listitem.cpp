#include "listitem.h"

#define MAIN_LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_SPACING 0
#define ITEM_FRAME_MARGINS 16,6,16,6
#define ITEM_FRAME_SPACING 10

ListItem::ListItem(QWidget *parent) : QFrame(parent)
{
    initUI();
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

void ListItem::initUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_mainLayout->setSpacing(MAIN_LAYOUT_SPACING);
    this->setLayout(m_mainLayout);

    m_itemFrame = new QFrame(this);

    m_hItemLayout = new QHBoxLayout(m_itemFrame);
    m_hItemLayout->setContentsMargins(ITEM_FRAME_MARGINS);
    m_hItemLayout->setSpacing(ITEM_FRAME_SPACING);

    m_netButton = new NetButton(m_itemFrame);
    m_nameLabel = new QLabel(m_itemFrame);
    m_infoButton = new InfoButton(m_itemFrame);

    m_hItemLayout->addWidget(m_netButton);
    m_hItemLayout->addWidget(m_nameLabel);
    m_hItemLayout->addStretch();
    m_hItemLayout->addWidget(m_infoButton);

    m_mainLayout->addWidget(m_itemFrame);
}
