#ifndef DLGCONNHIDWIFIWPA_H
#define DLGCONNHIDWIFIWPA_H

#include <QDialog>
#include <QMouseEvent>
#include <QThread>

class MainWindow;

namespace Ui {
class DlgConnHidWifiWpa;
}

class DlgConnHidWifiWpa : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConnHidWifiWpa(int type, MainWindow *mw = 0, QWidget *parent = 0);
    ~DlgConnHidWifiWpa();

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void changeDialog();
    void changeWindow();
    void emitSignal();
    void on_execSecConn();
    void slotStartLoading();

private slots:
    void on_btnCancel_clicked();

    void on_btnConnect_clicked();

    void on_checkBoxPwd_stateChanged(int arg1);

    void on_leNetName_textEdited(const QString &arg1);

    void on_lePassword_textEdited(const QString &arg1);

signals:
    void reSetWifiList();
    void sendMessage();
    void execSecConn();
    void stopSignal();

private:
    Ui::DlgConnHidWifiWpa *ui;
    int isUsed;//=0 current wifi not used before; >=1 used
    MainWindow *mw;
    QString strWifiname;
    QString strWifiPassword;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool isPress;
    QPoint winPos;
    QPoint dragPos;
};

#endif // DLGCONNHIDWIFIWPA_H
