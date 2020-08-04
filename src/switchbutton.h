#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QObject>
#include <QTimer>
#include <QWidget>
#include <QPainter>

class SwitchButton : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchButton(QWidget *parent = nullptr);
    void     setSwitchStatus(bool check);

private:
    int             m_bIsOn = 1;
    QTimer          *m_cTimer;
    float           m_fWidth;
    float           m_fHeight;
    float           m_fCurrentValue;
    void            paintEvent(QPaintEvent *event);
    void            mousePressEvent(QMouseEvent *event);

Q_SIGNALS:
    void clicked(int check);
private Q_SLOTS:
    void startAnimation();

};

#endif // SWITCHBUTTON_H
