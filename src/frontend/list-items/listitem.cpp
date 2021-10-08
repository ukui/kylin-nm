#include "listitem.h"
#include <QDebug>

#define MAIN_LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_SPACING 0
#define ITEM_FRAME_MARGINS 16,6,16,6
#define ITEM_FRAME_SPACING 10
#define FRAME_WIDTH 395
#define INFO_ICON_WIDTH 16
#define INFO_ICON_HEIGHT 16
#define LIGHT_HOVER_COLOR QColor(240,240,240,255)
#define DARK_HOVER_COLOR QColor(15,15,15,255)

ListItem::ListItem(QWidget *parent) : QFrame(parent)
{
    initUI();
    initConnection();
    connect(qApp, &QApplication::paletteChanged, this, &ListItem::onPaletteChanged);
//    m_itemFrame->installEventFilter(this);
}

ListItem::~ListItem()
{
    if (nullptr != m_netButton) {
        delete m_netButton;
        m_netButton = nullptr;
    }

    if (nullptr != m_infoButton) {
        delete m_infoButton;
        m_infoButton = nullptr;
    }

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

void ListItem::showDesktopNotify(const QString &message)
{
    QDBusInterface iface("org.freedesktop.Notifications",
                         "/org/freedesktop/Notifications",
                         "org.freedesktop.Notifications",
                         QDBusConnection::sessionBus());
    QList<QVariant> args;
    args<<(tr("Kylin NM"))
       <<((unsigned int) 0)
      <<QString("/usr/share/icons/ukui-icon-theme-default/24x24/devices/gnome-dev-ethernet.png")
     <<tr("kylin network applet desktop message") //显示的是什么类型的信息
    <<message //显示的具体信息
    <<QStringList()
    <<QVariantMap()
    <<(int)-1;
    iface.callWithArgumentList(QDBus::AutoDetect,"Notify",args);
}

void ListItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        onNetButtonClicked();
    } else if (event->button() == Qt::RightButton) {
        onRightButtonClicked();
    }
    return QFrame::mousePressEvent(event);
}

void ListItem::enterEvent(QEvent *event)
{
    QPalette pal = qApp->palette();
    QColor baseColor = qApp->palette().base().color();
    if (baseColor.red() > MIDDLE_COLOR) {
        pal.setColor(QPalette::Window, LIGHT_HOVER_COLOR);
    } else {
        pal.setColor(QPalette::Window, DARK_HOVER_COLOR);
    }
    this->setPalette(pal);
    return QFrame::enterEvent(event);
}

void ListItem::leaveEvent(QEvent *event)
{
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window, qApp->palette().base().color());
    this->setPalette(pal);
    return QFrame::leaveEvent(event);
}

void ListItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); //反锯齿
    painter.setBrush(this->palette().brush(QPalette::Window));
    painter.setPen(Qt::transparent);
    painter.drawRoundedRect(this->rect(), 8, 8);
    return QFrame::paintEvent(event);
}

//bool ListItem::eventFilter(QObject *watched, QEvent *event)
//{
//    if (watched == m_itemFrame) {
//        if (event->type() == QEvent::MouseButtonPress) {
//            onNetButtonClicked();
//        }
//    }
//    return QFrame::eventFilter(watched, event);
//}

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

    this->setAutoFillBackground(true);
    this->setBackgroundRole(QPalette::Base);
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window, qApp->palette().base().color());
    this->setPalette(pal);
}


void ListItem::initConnection()
{
    connect(this->m_netButton, &RadioItemButton::clicked, this, &ListItem::onNetButtonClicked);
    //    connect(this->m_infoButton, &InfoButton::clicked, this, &ListItem::onInfoButtonClicked);
}

void ListItem::onPaletteChanged()
{
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window, qApp->palette().base().color());
    this->setPalette(pal);
}
