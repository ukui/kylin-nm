#ifndef WIFICONFIGDIALOG_H
#define WIFICONFIGDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QPoint>
#include <QGSettings/QGSettings>

namespace Ui {
class WiFiConfigDialog;
}

class WiFiConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WiFiConfigDialog(QWidget *parent = nullptr);
    ~WiFiConfigDialog();

    void initTransparentState();
    double getTransparentData();
    void toConfigWifi();
    void setEnableOfBtn();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void on_checkBoxPwd_stateChanged(int arg1);

    void on_leWifiId_textEdited(const QString &arg1);

    void on_leWiFiName_textEdited(const QString &arg1);

    void on_leWifiPassword_textEdited(const QString &arg1);

private:
    Ui::WiFiConfigDialog *ui;
    QGSettings *m_transparency_gsettings = nullptr;
    QString     checkBoxQss = "QCheckBox {border:none;background:transparent;}"
                              "QCheckBox::indicator {width: 18px; height: 9px;}"
                              "QCheckBox::indicator:checked {image: url(:/res/h/show-pwd.png);}"
                              "QCheckBox::indicator:unchecked {image: url(:/res/h/hide-pwd.png);}";
};

#endif // WIFICONFIGDIALOG_H
