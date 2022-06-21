#ifndef MOBILEHOTSPOTWIDGET_H
#define MOBILEHOTSPOTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>

#include <QTranslator>
#include <QApplication>


#include <QDBusMessage>
#include <QDBusObjectPath>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMetaType>

#include "titlelabel.h"
#include "kwidget.h"
#include "kswitchbutton.h"
#include "kpasswordedit.h"

using namespace kdk;

class MobileHotspotWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MobileHotspotWidget(QWidget *parent = nullptr);
    ~MobileHotspotWidget();

private:
    QFrame *m_switchFrame = nullptr;          //开关
    QFrame *m_ApNameFrame = nullptr;          //wifi名称
    QFrame *m_passwordFrame = nullptr;        //密码
    QFrame *m_freqBandFrame = nullptr;        //频带
    QFrame *m_interfaceFrame = nullptr;       //网卡

    KSwitchButton *m_switchBtn;

    TitleLabel *m_hotspotTitleLabel;
    QLabel *m_switchLabel;
    QLabel *m_apNameLabel;
    QLabel *m_pwdLabel;
    QLabel *m_freqBandLabel;
    QLabel *m_interfaceLabel;

    QFrame *switchAndApNameLine;
    QFrame *apNameAndPwdLine;
    QFrame *pwdAndfreqBandLine;
    QFrame *freqBandAndInterfaceLine;

    QVBoxLayout *mVlayout;

    QLineEdit *m_apNameLine;
    KPasswordEdit *m_pwdNameLine;

    QComboBox *m_freqBandComboBox;
    QComboBox *m_interfaceComboBox;

    QDBusInterface  *m_interface = nullptr;

    QString m_interfaceName = "";

    QGSettings *m_switchGsettings = nullptr;

    QString m_uuid = "";
    QString m_hostName = "";

    void initUI();
    void initDbusConnect();
    void initInterfaceInfo();
    void getApInfo();
    bool getApInfoBySsid(QString devName, QString ssid, QStringList &info);
    void setSwitchFrame();
    void setApNameFrame();
    void setPasswordFrame();
    void setFreqBandFrame();
    void setInterFaceFrame();

    void setUiEnabled(bool enable);
    void setWidgetHidden(bool isHidden);

    void showDesktopNotify(const QString &message);

    void updateBandCombox();

    QString getHostName();

    bool eventFilter(QObject *watched, QEvent *event);
    void paintEvent(QPaintEvent *event);

    QFrame* myLine();

signals:

private slots:
    void onActivateFailed(QString errorMessage);
    void onDeactivateFailed(QString errorMessage);
    //设备插拔
    void onDeviceStatusChanged();
    void onDeviceNameChanged(QString oldName, QString newName, int type);
    //热点断开
    void onHotspotDeactivated(QString devName, QString ssid);
    //热点连接
    void onHotspotActivated(QString devName, QString ssid, QString uuid);

    void onGsettingChanged(const QString &key);

    void onActiveConnectionChanged(QString deviceName, QString ssid, QString uuid, int status);

    void onApLineEditTextEdit(QString text);

};

#endif // MOBILEHOTSPOTWIDGET_H
