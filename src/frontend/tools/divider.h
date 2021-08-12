#ifndef DIVIDER_H
#define DIVIDER_H
#include <QFrame>

class Divider : public QFrame
{
public:
    Divider(QWidget * parent = nullptr);
    ~Divider() = default;

protected:
    void paintEvent(QPaintEvent *);
};

#endif // DIVIDER_H
