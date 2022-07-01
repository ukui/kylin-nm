#ifndef KYLINPASSWORDDIALOG_H
#define KYLINPASSWORDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
class KyPasswordDialog : public QDialog
{
    Q_OBJECT
public:
    KyPasswordDialog(QWidget *parent = 0);
    ~KyPasswordDialog();
private:
    QLabel *usrLabel;
    QLabel *pwdLabel;
    QLineEdit *usrLineEdit;
    QLineEdit *pwdLineEdit;
    QPushButton *LoginBtn;
    QPushButton *exitBtn;

public:
    QString getPassword();
    void setPassword(QString password);

    QString getUserName();
    void setUserName(QString name);

private Q_SLOTS:
    void login();
    void close();

Q_SIGNALS:
    void signal_username(QString);
};

#endif // KYLINPASSWORDDIALOG_H
