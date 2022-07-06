#include "grayinfobutton.h"

#include <QVariant>

#define BUTTON_SIZE 36,36
#define ICON_SIZE 16,16

GrayInfoButton::GrayInfoButton(QWidget *parent): QPushButton(parent)
{
    this->setFixedSize(BUTTON_SIZE);
    this->setIcon(QIcon::fromTheme("preferences-system-details-symbolic"));
    this->setProperty("useButtonPalette", true);
    this->setFlat(true);
}
