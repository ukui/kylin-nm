#ifndef APTPROXYDIALOG_H
#define APTPROXYDIALOG_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QGSettings>
#include <QLabel>
#include <QLineEdit>
#include <QDialog>
#include "fixlabel.h"

#define APT_PROXY_SCHEMA              "org.ukui.control-center.apt.proxy"
#define APT_PROXY_ENABLED            "enabled"
#define APT_PROXY_HOST_KEY         "host"
#define APT_PROXY_PORT_KEY         "port"

class AptProxyDialog : public QDialog
{
    Q_OBJECT
public:
    AptProxyDialog(QWidget *parent = nullptr);
    ~AptProxyDialog();

    void initUi();
private:
    QLineEdit *mHostEdit;
    QLineEdit *mPortEdit;

    QPushButton *mCancelBtn;
    QPushButton *mConfirmBtn;

private:
    void initConnect();
     void setupComponent();
};

#endif // APTPROXYDIALOG_H
