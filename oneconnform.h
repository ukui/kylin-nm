#ifndef ONECONNFORM_H
#define ONECONNFORM_H

#include <QWidget>
#include <QScreen>
#include <QThread>

#include "confform.h"
#include "kylin-network-interface.h"
#include "backthread.h"
#include "ksimplenm.h"

class MainWindow;

namespace Ui {
class OneConnForm;
}

class OneConnForm : public QWidget
{
    Q_OBJECT

public:
    explicit OneConnForm(QWidget *parent = 0, MainWindow *mw = 0, ConfForm *confForm = 0, KSimpleNM *ksnm = 0);
    ~OneConnForm();

    void setSignal(QString lv);
    void setName(QString name);
    void setSafe(QString safe);
    void setRate(QString rate);

    void setSelected(bool isSelected);
    void setAct(bool isAct);

    void setSafeString(QString str);
    void setConnedString(QString str);
    void setShowPoint(bool flag);

    QString wifiName;
    bool isSelected;
    bool isActive;
    int signalLv;

signals:
    void selectedOneWifiForm(QString wifiName);
    void connDone(int connFlag);
    void disconnActiveWifi();

    void sigConnWifi(QString);
    void sigConnWifiPWD(QString, QString);

protected:
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_btnConf_clicked();
    void on_btnConn_clicked();

    void on_btnDisConn_clicked();

    void slotConnWifi();
    void slotConnWifiPWD();

    void slotConnDone(int connFlag);

    void on_btnConnPWD_clicked();

private:
    Ui::OneConnForm *ui;
    MainWindow *mw;
    ConfForm *cf;
    KSimpleNM *ks;
    bool isSafe;
};

#endif // ONECONNFORM_H
