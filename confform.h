#ifndef CONFFORM_H
#define CONFFORM_H

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QPoint>

namespace Ui {
class ConfForm;
}

class ConfForm : public QWidget
{
    Q_OBJECT

public:
    explicit ConfForm(QWidget *parent = 0);
    ~ConfForm();

    void setProp(QString connName, QString v4method, QString addr, QString mask, QString gateway, QString dns, bool isActConf);

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
    void cbTypeChanged(int index);

private:
    Ui::ConfForm *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    bool isPress;
    QPoint winPos;
    QPoint dragPos;
    bool isActConf;
};

#endif // CONFFORM_H
