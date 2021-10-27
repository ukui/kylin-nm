#include "addnetbtn.h"
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QVariant>

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
