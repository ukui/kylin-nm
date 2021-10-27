#ifndef ADDNETBTN_H
#define ADDNETBTN_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QTranslator>
#include <QApplication>

class AddNetBtn : public QPushButton
{
    Q_OBJECT
public:
    AddNetBtn(QWidget *parent = nullptr);
    ~AddNetBtn();

protected:
    virtual void leaveEvent(QEvent * event);
    virtual void enterEvent(QEvent * event);

Q_SIGNALS:
    void enterWidget();
    void leaveWidget();
};

#endif // ADDNETBTN_H
