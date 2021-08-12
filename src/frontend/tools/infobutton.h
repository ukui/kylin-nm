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
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
    bool eventFilter(QObject *, QEvent *);
};

#endif // INFOBUTTON_H
