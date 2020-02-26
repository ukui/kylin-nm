#ifndef DLGCONNHIDWIFISECLEAP_H
#define DLGCONNHIDWIFISECLEAP_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class DlgConnHidWifiSecLeap;
}

class DlgConnHidWifiSecLeap : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConnHidWifiSecLeap(int type, QWidget *parent = 0);
    ~DlgConnHidWifiSecLeap();

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
    Ui::DlgConnHidWifiSecLeap *ui;
    int WepOrWpa = 0;//0 WEP;1WPA

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool isPress;
    QPoint winPos;
    QPoint dragPos;
};

#endif // DLGCONNHIDWIFISECLEAP_H
