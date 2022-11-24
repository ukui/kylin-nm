#include "lanitem.h"
#define FRAME_SPEED 150
#define LIMIT_TIME 60*1000
#define TOTAL_PAGE 8
#define RADIUS 6.0

LanItem::LanItem(bool isSimple, QWidget *parent)
    : QFrame(parent),isSimple(isSimple)
{
    this->setFixedHeight(48);
    this->setMinimumWidth(MIN_ITEM_WIDTH);
    this->setMaximumWidth(MAX_ITEM_WIDTH);
    QHBoxLayout *mLanLyt = new QHBoxLayout(this);
    mLanLyt->setContentsMargins(16,0,0,0);
    mLanLyt->setSpacing(0);
    radioBtn = new RadioItemButton(this);
    titileLabel = new FixLabel(this);
    titileLabel->setFixedWidth(282);

    mLanLyt->setAlignment(Qt::AlignLeft);
    mLanLyt->addWidget(radioBtn);
    mLanLyt->addSpacing(10);
    mLanLyt->addWidget(titileLabel);
    if (!isSimple) {
        infoLabel = new InfoButton(this);
        mLanLyt->addSpacing(8);
        mLanLyt->addWidget(infoLabel);
        connect(infoLabel, &InfoButton::released, this, &LanItem::infoButtonClick);
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
    m_menu->setWindowFlag(Qt::X11BypassWindowManagerHint);
    connect(m_menu, &QMenu::triggered, this, &LanItem::itemClick);
}

void LanItem::updateIcon()
{
    if (currentIconIndex > 6) {
        currentIconIndex = 0;
    }
    radioBtn->setButtonIcon(loadIcons.at(currentIconIndex));
    currentIconIndex ++;
}

void LanItem::startLoading()
{
    waitTimer->start(FRAME_SPEED);
    m_loading = true;
}

void LanItem::stopLoading(){
    waitTimer->stop();
    m_loading = false;
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
        if (m_isAcitve || m_loading) {
            m_menu->addAction(new QAction(tr("Disconnect"), this));
        } else if (!m_isAcitve && !m_loading) {
            m_menu->addAction(new QAction(tr("Connect"), this));
        }
        m_menu->move(cursor().pos());
        m_menu->show();

    } else {
        if (!m_isAcitve && !m_loading) {
           Q_EMIT itemClick();
        }
    }
    return QFrame::mouseReleaseEvent(event);
}
void LanItem::enterEvent(QEvent *event)
{
    m_isIn = true;
    update();
    return QFrame::enterEvent(event);
}
void LanItem::leaveEvent(QEvent *event)
{
    m_isIn = false;
    update();
    return QFrame::leaveEvent(event);
}
void LanItem::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();

    QPainter painter(this);
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);
    if (m_isIn) {
        QColor color(240, 240, 240);
        color.setAlphaF(0.39);
        painter.setBrush(color);
    }
    else
        painter.setBrush(pal.color(QPalette::Base));

    QRect rect = this->rect();
    QPainterPath path;
    path.addRoundedRect(rect, RADIUS, RADIUS);
    painter.drawPath(path);
    return QFrame::paintEvent(event);
}
