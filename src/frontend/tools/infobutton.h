#ifndef INFOBUTTON_H
#define INFOBUTTON_H
#include <QPushButton>

class InfoButton : public QPushButton
{
    Q_OBJECT
public:
    explicit InfoButton(QWidget * parent = nullptr);
    ~InfoButton() = default;

private:
    void initUI();

protected:
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
};

#endif // INFOBUTTON_H
