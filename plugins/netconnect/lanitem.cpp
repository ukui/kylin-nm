#include "lanitem.h"
#define FRAME_SPEED 150
#define LIMIT_TIME 60*1000
#define TOTAL_PAGE 8

LanItem::LanItem(bool isSimple, QWidget *parent)
    : QFrame(parent),isSimple(isSimple)
{
    this->setFixedSize(420, 48);
    QHBoxLayout *mLanLyt = new QHBoxLayout(this);
    mLanLyt->setContentsMargins(24,0,24,0);
    mLanLyt->setSpacing(0);
    iconLabel = new QLabel(this);
    iconLabel->setProperty("useIconHighlightEffect", 0x2);
    titileLabel = new QLabel(this);

    mLanLyt->addWidget(iconLabel);
    mLanLyt->addSpacing(10);
    mLanLyt->addWidget(titileLabel,Qt::AlignLeft);
    mLanLyt->addStretch();
    if (!isSimple) {
        infoLabel = new InfoButton(this);
        mLanLyt->addSpacing(8);
        mLanLyt->addWidget(infoLabel);
    }

    loadIcons.append(QIcon::fromTheme("ukui-loading-1-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-2-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-3-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-4-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-5-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-6-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-7-symbolic"));
    waitTimer = new QTimer(this);
    connect(waitTimer, &QTimer::timeout, this, &LanItem::updateIcon);

    m_menu = new QMenu(this);//右键菜单
    connect(m_menu, &QMenu::triggered, this, &LanItem::itemClick);
}

void LanItem::updateIcon()
{
    if (currentIconIndex > 6) {
        currentIconIndex = 0;
    }
    iconLabel->setPixmap(loadIcons.at(currentIconIndex).pixmap(16,16));
    currentIconIndex ++;
}

void LanItem::startLoading()
{
    waitTimer->start(FRAME_SPEED);
    loading = true;
}

void LanItem::stopLoading(){
    waitTimer->stop();
    loading = false;
}

void LanItem::mousePressEvent(QMouseEvent *event)
{
    return QFrame::mousePressEvent(event);
}
void LanItem::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton) {
        if (!m_menu) {
            return QFrame::mouseReleaseEvent(event);
        }

        m_menu->clear();
        if (isAcitve || loading) {
            m_menu->addAction(new QAction(tr("Disconnect"), this));
        } else if (!isAcitve && !loading) {
            m_menu->addAction(new QAction(tr("Connect"), this));
        }
        m_menu->move(cursor().pos());
        m_menu->show();

    } else {
        if (!isAcitve && !loading) {
           Q_EMIT itemClick();
        }
    }
    return QFrame::mouseReleaseEvent(event);
}
void LanItem::enterEvent(QEvent *event)
{
    return QFrame::enterEvent(event);
}
void LanItem::leaveEvent(QEvent *event)
{
    return QFrame::leaveEvent(event);
}
void LanItem::paintEvent(QPaintEvent *event)
{
    return QFrame::paintEvent(event);
}
