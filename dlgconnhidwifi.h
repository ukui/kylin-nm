#ifndef DLGCONNHIDWIFI_H
#define DLGCONNHIDWIFI_H

#include <QDialog>
#include <QMouseEvent>
#include <QThread>
#include <QTimer>
#include <QFile>

class MainWindow;

namespace Ui {
class DlgConnHidWifi;
}

class DlgConnHidWifi : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConnHidWifi(int type, MainWindow *mw = 0, QWidget *parent = 0);
    ~DlgConnHidWifi();

public slots:
    void changeDialog();
    void changeWindow();
    void emitSignal();
    void on_execSecConn();
    void slotStartLoading();

private slots:
    void on_btnCancel_clicked();

    void on_btnConnect_clicked();

    void on_leNetName_textEdited(const QString &arg1);

signals:
    void reSetWifiList();
    void sendMessage();
    void execSecConn();
    void stopSignal();

private:
    Ui::DlgConnHidWifi *ui;
    int isUsed;//=0 current wifi not used before; >=1 used
    MainWindow *mw;
    QString strWifiname;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    bool isPress;
    QPoint winPos;
    QPoint dragPos;
};

#endif // DLGCONNHIDWIFI_H
