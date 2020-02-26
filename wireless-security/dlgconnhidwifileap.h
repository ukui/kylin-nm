#ifndef DLGCONNHIDWIFILEAP_H
#define DLGCONNHIDWIFILEAP_H

#include <QDialog>
#include <QMouseEvent>

namespace Ui {
class DlgConnHidWifiLeap;
}

class DlgConnHidWifiLeap : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConnHidWifiLeap(QWidget *parent = 0);
    ~DlgConnHidWifiLeap();

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void changeDialog();

private slots:
    void on_btnCancel_clicked();

    void on_btnConnect_clicked();

    void on_checkBoxPwd_stateChanged(int arg1);

    void on_leNetName_textEdited(const QString &arg1);

    void on_leUserName_textEdited(const QString &arg1);

    void on_lePassword_textEdited(const QString &arg1);

private:
    Ui::DlgConnHidWifiLeap *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    bool isPress;
    QPoint winPos;
    QPoint dragPos;
};

#endif // DLGCONNHIDWIFILEAP_H
