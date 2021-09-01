#ifndef INFOBUTTON_H
#define INFOBUTTON_H
#include <QPushButton>
#include <QIcon>

class InfoButton : public QPushButton
{
    Q_OBJECT
public:
    explicit InfoButton(QWidget * parent = nullptr);
    ~InfoButton() = default;

private:
    void initUI();
    QIcon info_img;
    QIcon info_imgHover;
    QIcon info_imgPressed;

protected:
    bool event(QEvent *event);
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    void paintEvent(QPaintEvent *event);
};

#endif // INFOBUTTON_H
