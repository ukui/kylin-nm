#ifndef DLGCONNHIDWIFIWEP_H
#define DLGCONNHIDWIFIWEP_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class DlgConnHidWifiWep;
}

class DlgConnHidWifiWep : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConnHidWifiWep(int type, QWidget *parent = 0);
    ~DlgConnHidWifiWep();

public slots:
    void changeDialog();

private slots:
    void on_btnCancel_clicked();

    void on_btnConnect_clicked();

    void on_checkBoxPwd_stateChanged(int arg1);

    void on_leKey_textEdited(const QString &arg1);

    void on_leNetName_textEdited(const QString &arg1);

private:
    Ui::DlgConnHidWifiWep *ui;
    int WepPwdOrCode = 0; //0 WEP password;1 WEP Code Sentence

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool isPress;
    QPoint winPos;
    QPoint dragPos;
};

#endif // DLGCONNHIDWIFIWEP_H
