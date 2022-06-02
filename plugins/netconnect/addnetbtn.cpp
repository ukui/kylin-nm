#include "addnetbtn.h"
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QVariant>
#include <QPainter>
#include <QPainterPath>

#define RADIUS 6.0

AddNetBtn::AddNetBtn(QWidget *parent) : QPushButton(parent)
{
    this->setObjectName("this");
    this->setMinimumSize(QSize(580, 60));
    this->setMaximumSize(QSize(16777215, 60));
    this->setStyleSheet("QPushButton:!checked{background-color: palette(base)}");
    this->setProperty("useButtonPalette", true);

    QHBoxLayout *addLyt = new QHBoxLayout;

    QLabel *iconLabel = new QLabel();
    QLabel *textLabel = new QLabel(tr("Add WiredNetork"));

    QIcon mAddIcon = QIcon::fromTheme("list-add-symbolic");
    iconLabel->setPixmap(mAddIcon.pixmap(mAddIcon.actualSize(QSize(24, 24))));
    iconLabel->setProperty("useIconHighlightEffect", true);
    iconLabel->setProperty("iconHighlightEffectMode", 1);

    addLyt->addStretch();
    addLyt->addWidget(iconLabel);
    addLyt->addWidget(textLabel);
    addLyt->addStretch();
    this->setLayout(addLyt);

}

AddNetBtn::~AddNetBtn()
{

}

void AddNetBtn::enterEvent(QEvent *event){
    Q_EMIT enterWidget();

    QPushButton::enterEvent(event);
}

void AddNetBtn::leaveEvent(QEvent *event){
    Q_EMIT leaveWidget();

    QPushButton::leaveEvent(event);
}

void AddNetBtn::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();

    QPainter painter(this);
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(pal.color(QPalette::Base));

    QRect rect = this->rect();
    QPainterPath path;
    path.addRoundedRect (rect, RADIUS, RADIUS);
    QRect temp_rect(rect.left(), rect.top(), rect.width(), rect.height()/2);
    path.addRect(temp_rect);
    painter.drawPath(path);
    QPushButton::paintEvent(event);
}
