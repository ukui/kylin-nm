#ifndef DLGHOTSPOTCREATE_H
#define DLGHOTSPOTCREATE_H

#include <sys/syslog.h>

#include <QDialog>
#include <QMouseEvent>
#include <QDebug>

namespace Ui {
class DlgHotspotCreate;
}

class DlgHotspotCreate : public QDialog
{
    Q_OBJECT

public:
    explicit DlgHotspotCreate(QString wiFiCardName, QWidget *parent = nullptr);
    ~DlgHotspotCreate();

public slots:
    void changeDialog();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void on_checkBoxPwd_stateChanged(int arg1);

    void on_leNetName_textEdited(const QString &arg1);

    void on_lePassword_textEdited(const QString &arg1);

private:
    Ui::DlgHotspotCreate *ui;

    QString wirelessCardName;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool isPress;
    QPoint winPos;
    QPoint dragPos;

signals:
    void updateHotspotList();
    void btnHotspotState();
};

#endif // DLGHOTSPOTCREATE_H
