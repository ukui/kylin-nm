#ifndef DLGCONNHIDWIFISECPWD_H
#define DLGCONNHIDWIFISECPWD_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class DlgConnHidWifiSecPwd;
}

class DlgConnHidWifiSecPwd : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConnHidWifiSecPwd(int type, QWidget *parent = 0);
    ~DlgConnHidWifiSecPwd();

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void changeDialogSecu();
    void changeDialogAuth();

private slots:
    void on_btnCancel_clicked();

    void on_btnConnect_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_leNetName_textEdited(const QString &arg1);

    void on_leUserName_textEdited(const QString &arg1);

    void on_lePassword_textEdited(const QString &arg1);

private:
    Ui::DlgConnHidWifiSecPwd *ui;
    int WepOrWpa = 0;//0 WEP;1WPA

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    bool isPress;
    QPoint winPos;
    QPoint dragPos;
};

#endif // DLGCONNHIDWIFISECPWD_H
