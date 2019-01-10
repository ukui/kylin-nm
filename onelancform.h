#ifndef ONELANCFORM_H
#define ONELANCFORM_H

#include <QWidget>
#include <QScreen>
#include <QThread>

#include "confform.h"
#include "kylin-network-interface.h"
#include "backthread.h"
#include "ksimplenm.h"

class MainWindow;

namespace Ui {
class OneLancForm;
}

class OneLancForm : public QWidget
{
    Q_OBJECT

public:
    explicit OneLancForm(QWidget *parent = 0, MainWindow *mw = 0, ConfForm *confForm = 0, KSimpleNM *ksnm = 0);
    ~OneLancForm();

    void setName(QString name);
    void setIcon(bool isOn);
    void setBandWidth(QString bandWidth);

    void setSelected(bool isSelected);
    void setAct(bool isAct);

    void setConnedString(QString str);
    void setShowPoint(bool flag);

    bool isSelected;
    bool isActive;
    QString lanName;

signals:
    void selectedOneLanForm(QString lanName);
    void connDone(int connFlag);
    void disconnActiveLan();

    void sigConnLan(QString);

protected:
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void on_btnConf_clicked();
    void on_btnConn_clicked();

    void on_btnDisConn_clicked();

    void slotConnLan();

private:
    Ui::OneLancForm *ui;
    MainWindow *mw;
    ConfForm *cf;
    KSimpleNM *ks;
};

#endif // ONELANCFORM_H
