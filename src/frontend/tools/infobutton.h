#ifndef INFOBUTTON_H
#define INFOBUTTON_H
#include <QPushButton>
#include <QLabel>

class InfoButton : public QPushButton
{
    Q_OBJECT
public:
    explicit InfoButton(QWidget * parent = nullptr);
    ~InfoButton() = default;

signals:
    void clicked();

private:
    void initUI();

private:
    QLabel *m_iconLabel = nullptr;

protected:
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
    void paintEvent(QPaintEvent *event);
};

#endif // INFOBUTTON_H
