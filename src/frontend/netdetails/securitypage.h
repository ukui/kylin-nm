#ifndef SECURITYWIDGET_H
#define SECURITYWIDGET_H

#include <QWidget>
#include <QFormLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>

class SecurityPage : public QFrame
{
public:
    SecurityPage(QWidget *parent = nullptr);
private:
    QFormLayout *mDetailLayout;

private:
    void initUI();

};

#endif // SECURITYWIDGET_H
