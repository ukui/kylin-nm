#ifndef GRAYINFOBUTTON_H
#define GRAYINFOBUTTON_H

#include <QPushButton>
#include <QIcon>

class GrayInfoButton : public QPushButton
{
    Q_OBJECT
public:
    explicit GrayInfoButton(QWidget * parent = nullptr);
    ~GrayInfoButton() = default;
};

#endif // GRAYINFOBUTTON_H
