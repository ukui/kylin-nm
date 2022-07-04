#include "connectdevpage.h"
#include <QDebug>

#define CONTENTS_MARGINS 0, 0, 0, 0
#define FRAME_MIN_SIZE 550, 60
#define FRAME_MAX_SIZE 16777215, 16777215
#define LINE_MAX_SIZE 16777215, 1
#define LINE_MIN_SIZE 0, 1

ConnectdevPage::ConnectdevPage(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *Vlayout = new QVBoxLayout(this);
    Vlayout->setContentsMargins(CONTENTS_MARGINS);
    Vlayout->setSpacing(0);

    QFrame *staistFrame = new QFrame(this);
    staistFrame->setMinimumSize(FRAME_MIN_SIZE);
    staistFrame->setMaximumSize(FRAME_MAX_SIZE);
    staistFrame->setFrameShape(QFrame::Box);

    m_staListLayout = new QVBoxLayout(staistFrame);
    m_staListLayout->setContentsMargins(0, 0, 0, 0);
    m_staListLayout->setSpacing(0);

    m_titleLabel = new TitleLabel(this);
    m_titleLabel->setText(tr("Connect device"));

    Vlayout->addWidget(m_titleLabel);
    Vlayout->addSpacing(8);
    Vlayout->addWidget(staistFrame);
}

ConnectdevPage::~ConnectdevPage()
{
}

void ConnectdevPage::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

QFrame* ConnectdevPage::myLine()
{
    QFrame *line = new QFrame(this);
    line->setMinimumSize(QSize(LINE_MIN_SIZE));
    line->setMaximumSize(QSize(LINE_MAX_SIZE));
    line->setLineWidth(0);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    return line;
}

void ConnectdevPage::getConnectStaDevice(QMap<QString, QString> &staMap)
{
    staMap.clear();
    if (m_avtivePathInterface == nullptr || !m_avtivePathInterface->isValid()) {
        qDebug() << "dbus interface m_avtivePathInterface is invaild";
        return ;
    }

    QDBusMessage reply = m_avtivePathInterface->call("Getstainfo");
    if(reply.type() == QDBusMessage::ErrorMessage)
    {
        qWarning() << "[mobilehotspot]Getstainfo error:" << reply.errorMessage();
        return;
    }
    if (reply.arguments().isEmpty()
        || reply.arguments().at(0).toString() == ""
        || reply.arguments().at(0).toString() == "[Invalid UTF-8]"
        || reply.arguments().at(1).toString() == "") {
        qDebug() << "Dbus interface call Getstainfo return is empty!";
        return ;
    }

    QStringList macList = reply.arguments().at(0).toString().split(";");
    QStringList hostNameList = reply.arguments().at(1).toString().split(";");
    for (int index = 0; index < macList.count() && macList.at(index) != nullptr; index ++) {
        if (!staMap.contains(macList.at(index))) {
            staMap[macList.at(index)] = hostNameList.at(index);
        }
    }
}

void ConnectdevPage::initStaDev()
{
    QMap<QString, QString>::const_iterator item = m_staMap.cbegin();
    while (item != m_staMap.cend()) {
        addStaDevFrame(item.key(), item.value());
        item ++;
        if (item != m_staMap.cend()) {
            m_staListLayout->addWidget(myLine());
        }
    }
}

void ConnectdevPage::addStaDevFrame(QString staMac, QString staName)
{
    ConnectDevListItem *itemFrame = new ConnectDevListItem(staMac, staName, m_staListLayout->widget());
    m_staListLayout->addWidget(itemFrame);
    connect(itemFrame, &ConnectDevListItem::onBtnClicked, this, &ConnectdevPage::onDropIntoBlacklistBtnClicked);
}

void ConnectdevPage::clearStaListLayout()
{
    m_staMap.clear();
    if (m_staListLayout->layout() != NULL) {
        QLayoutItem* layoutItem;
        while ((layoutItem = m_staListLayout->layout()->takeAt(0)) != NULL) {
            delete layoutItem->widget();
            delete layoutItem;
        }
    }
}

void ConnectdevPage::onStaDevAdded(bool istrue, QString staMac, QString staName)
{
    if (!m_staMap.keys().contains(staMac)) {
        qDebug()<< Q_FUNC_INFO << __LINE__ << istrue << staMac << staName;
        onStaDevChanged(istrue, staMac, staName);
    }
}

void ConnectdevPage::onStaDevRemoved(bool istrue, QString staMac, QString staName)
{
    if (m_staMap.keys().contains(staMac)) {
        qDebug()<< Q_FUNC_INFO << __LINE__ << istrue << staMac << staName;
        onStaDevChanged(istrue, staMac, staName);
    }
}

void ConnectdevPage::onStaDevChanged(bool istrue, QString staMac, QString staName)
{
    if (istrue) {
        refreshStalist();
    }
}

void ConnectdevPage::refreshStalist()
{
    clearStaListLayout();
    getConnectStaDevice(m_staMap);
    initStaDev();
    int height = 0;
    for (int i = 0; i < m_staListLayout->count(); i ++) {
        QWidget *w = m_staListLayout->itemAt(i)->widget();
        if (w != nullptr) {
            height += w->height();
        }
    }
    this->setFixedHeight(height + m_titleLabel->height() + 8);

    if (m_staMap.isEmpty()) {
        this->hide();
    } else {
        this->show();
    }
    this->update();
}

void ConnectdevPage::onDropIntoBlacklistBtnClicked(QString staMac)
{
    if (staMac.isNull() || staMac.isEmpty()) {
        return;
    }

    emit setStaIntoBlacklist(staMac);
}
