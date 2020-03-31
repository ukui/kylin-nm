#ifndef NOTIFYSEND_H
#define NOTIFYSEND_H

#include <QWidget>
#include <QTimer>
#include <QScreen>

namespace Ui {
class NotifySend;
}

class NotifySend : public QWidget
{
    Q_OBJECT

public:
    explicit NotifySend(QWidget *parent = nullptr);
    ~NotifySend();

    void execNotifySend(QString text);

private:
    Ui::NotifySend *ui;

private slots:
    void closeNotifyDialog();

    void on_btnClose_clicked();

signals:
    void timeout();
};

#endif // NOTIFYSEND_H
