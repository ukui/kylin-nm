#ifndef DLGCONNHIDWIFISECPEAP_H
#define DLGCONNHIDWIFISECPEAP_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class DlgConnHidWifiSecPeap;
}

class DlgConnHidWifiSecPeap : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConnHidWifiSecPeap(int type, QWidget *parent = 0);
    ~DlgConnHidWifiSecPeap();

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void changeDialogSecu();
    void changeDialogAuth();

private slots:
    void on_btnCancel_clicked();

    void on_btnConnect_clicked();

    void on_cbxCA_currentIndexChanged(const QString &arg1);

    void on_checkBoxPwd_stateChanged(int arg1);

    void on_checkBoxCA_stateChanged(int arg1);

    void on_checkBoxPwdSec_stateChanged(int arg1);

    void on_leNetName_textEdited(const QString &arg1);

    void on_leAnonyId_textEdited(const QString &arg1);

    void on_leDomain_textEdited(const QString &arg1);

    void on_leCaPwd_textEdited(const QString &arg1);

    void on_leUserName_textEdited(const QString &arg1);

    void on_lePassword_textEdited(const QString &arg1);

private:
    Ui::DlgConnHidWifiSecPeap *ui;
    int WepOrWpa = 0;//0 WEP;1WPA

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

//    QString labelQss, cbxQss, leQss, btnConnQss, btnCancelQss, lineQss, checkBoxQss, checkBoxCAQss;

    bool isPress;
    QPoint winPos;
    QPoint dragPos;
};

#endif // DLGCONNHIDWIFISECPEAP_H
