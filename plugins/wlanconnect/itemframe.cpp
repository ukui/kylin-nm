#include "itemframe.h"
#include <QPainter>
#include <kwindowsystem.h>
#include <kwindowsystem_export.h>

#define LAYOUT_MARGINS 8,0,8,0
#define MAIN_LAYOUT_MARGINS 0,0,0,0
#define RADIUS 6.0

AddNetItem::AddNetItem(QWidget *parent) : QFrame(parent)
{
    this->setFixedSize(404, 48);
    this->setFixedHeight(48);
    this->setMinimumWidth(MIN_ITEM_WIDTH);
    this->setMaximumWidth(MAX_ITEM_WIDTH);
    QHBoxLayout *m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(0,0,0,0);

    titleLabel = new QLabel(this);
    titleLabel->setText(tr("Add Others..."));

    m_mainLayout->addSpacing(16);
    m_mainLayout->addWidget(titleLabel);
    m_mainLayout->addStretch();

    this->setLayout(m_mainLayout);
}

void AddNetItem::mouseReleaseEvent(QMouseEvent *event)
{
    Q_EMIT itemClick();
    return QFrame::mouseReleaseEvent(event);
}

void AddNetItem::enterEvent(QEvent *event)
{
    m_isIn = true;
    update();
    return QFrame::enterEvent(event);
}
void AddNetItem::leaveEvent(QEvent *event)
{
    m_isIn = false;
    update();
    return QFrame::leaveEvent(event);
}
void AddNetItem::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();

    QPainter painter(this);
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);
    if (m_isIn) {
        QColor color = qApp->palette().brightText().color();
        color.setAlphaF(0.05);
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

ItemFrame::ItemFrame(QString devName, QWidget *parent) : QFrame(parent)
{
    deviceLanLayout = new QVBoxLayout(this);
    deviceLanLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    lanItemFrame = new QFrame(this);
    lanItemFrame->setFrameShape(QFrame::Shape::NoFrame);

    lanItemLayout = new QVBoxLayout(this);
    lanItemLayout->setContentsMargins(LAYOUT_MARGINS);
    lanItemLayout->setSpacing(0);

    deviceLanLayout->setSpacing(0);
    setLayout(deviceLanLayout);
    lanItemFrame->setLayout(lanItemLayout);

    deviceFrame = new DeviceFrame(devName, this);
    m_divider = new Divider(this);
    addNetItem = new AddNetItem(this);

    QWidget *addWidget = new QWidget(this);
    QHBoxLayout *hLayout = new QHBoxLayout(addWidget);
    hLayout->setContentsMargins(8,0,8,0);
    hLayout->addWidget(addNetItem);

    deviceLanLayout->addWidget(m_divider);
    deviceLanLayout->addWidget(deviceFrame);
    deviceLanLayout->addWidget(lanItemFrame);
    deviceLanLayout->addWidget(addWidget);

    connect(addNetItem, &AddNetItem::itemClick, this, &ItemFrame::addNetItemClick);
}

void ItemFrame::showJoinPage(bool isSimple, QWidget *widget)
{
    if (nullptr != joinPage) {
//        joinPage->show();
        KWindowSystem::raiseWindow(joinPage->winId());
    } else {
        joinPage = new HiddenWiFiPage(deviceFrame->deviceLabel->text(), isSimple, widget);
        connect(joinPage, &HiddenWiFiPage::destroyed, [=](){
            joinPage->disconnect(this);
            joinPage = nullptr;
        });
        connect(joinPage, &HiddenWiFiPage::connectHideNormalConnect, this, &ItemFrame::connectHideNormalConnect);
        connect(joinPage, &HiddenWiFiPage::connectHideTtlsConnect, this, &ItemFrame::connectHideTtlsConnect);
        connect(joinPage, &HiddenWiFiPage::connectHidePeapConnect, this, &ItemFrame::connectHidePeapConnect);
        joinPage->show();
    }
}
