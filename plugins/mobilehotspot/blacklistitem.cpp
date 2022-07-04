#include "blacklistitem.h"
#include <QDebug>

#define MAIN_LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_SPACING 0
#define ITEM_FRAME_MARGINS 0,0,0,0
#define ITEM_FRAME_SPACING 10
#define FRAME_WIDTH 395
#define INFO_ICON_WIDTH 16
#define INFO_ICON_HEIGHT 16
#define LIGHT_HOVER_COLOR QColor(240,240,240,255)
#define DARK_HOVER_COLOR QColor(15,15,15,255)

#define FRAME_MIN_SIZE 550, 60
#define CONTECT_FRAME_MAX_SIZE 16777215, 60
#define LABLE_MIN_WIDTH 188
#define ITEM_MARGINS 16, 0, 16, 0

BlacklistItem::BlacklistItem(QString staMac, QString staName, QWidget *parent) :
    QFrame(parent),
    m_mac(staMac),
    m_hostName(staName)
{
    qDebug()<<"---CXC---"<<m_mac<<Q_FUNC_INFO<<__LINE__;
    this->setMinimumHeight(60);
    this->setFixedHeight(60);

    QHBoxLayout *hItemLayout = new QHBoxLayout(this);
    hItemLayout->setContentsMargins(ITEM_MARGINS);
    QLabel *nameLabel = new QLabel(m_hostName, this);
    m_removeFromBlacklistBtn = new KBorderlessButton(this);
    m_removeFromBlacklistBtn->setText(tr("Remove"));
    hItemLayout->setSpacing(0);
    hItemLayout->addWidget(nameLabel, Qt::AlignLeft);
    hItemLayout->addStretch();
    hItemLayout->addWidget(m_removeFromBlacklistBtn, Qt::AlignRight);

    m_removeFromBlacklistBtn->installEventFilter(this);
    this->setLayout(hItemLayout);
}

BlacklistItem::~BlacklistItem()
{

}

bool BlacklistItem::eventFilter(QObject *w, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonRelease) {
        if (w == m_removeFromBlacklistBtn) {
            qDebug() << "--cxc--" << Q_FUNC_INFO << __LINE__ << m_mac;
            emit onBtnClicked(m_mac);
            return true;
        }
    }
    return QWidget::eventFilter(w,e);
}

