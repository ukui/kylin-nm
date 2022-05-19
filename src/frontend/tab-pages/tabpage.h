#ifndef TABPAGE_H
#define TABPAGE_H

#include "divider.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QListWidget>
#include <QDir>
#include <QComboBox>
#include <QEvent>
#include <QProcess>
#include <QDebug>
#include "kylinnetworkdeviceresource.h"
#include "kwidget.h"
#include "kswitchbutton.h"
#include "kborderlessbutton.h"

using namespace kdk;

#define MAIN_LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_SPACING 0
#define TITLE_FRAME_HEIGHT 50     //TabWidget的tab和widget有间隙，和设计稿看起来一致就不能设为设计稿里的高度
#define TITLE_LAYOUT_MARGINS 24,0,24,0
#define DEVICE_LAYOUT_MARGINS 24,0,24,8
#define DEVICE_COMBOBOX_WIDTH 180
#define ACTIVE_NET_LAYOUT_MARGINS 8,8,8,8
#define NET_LAYOUT_MARGINS 8,8,0,3
#define NET_LAYOUT_SPACING 8
#define NET_LIST_SPACING 0
#define TEXT_MARGINS 16,0,0,0
//#define SCROLL_AREA_HEIGHT 200
#define SETTINGS_LAYOUT_MARGINS 24,0,24,0
#define TRANSPARENT_COLOR QColor(0,0,0,0)
#define INACTIVE_AREA_MIN_HEIGHT 170
#define ACTIVE_AREA_MAX_HEIGHT 92

#define MAX_ITEMS 4
#define MAX_WIDTH 400
#define MIN_WIDTH 383

#define SCROLL_STEP 4

enum KyDeviceType
{
    WIRED,
    WIRELESS
};

const QString CONFIG_FILE_PATH   =  QDir::homePath() + "/.config/ukui/kylin-nm.conf";
bool checkDeviceExist(KyDeviceType deviceType, QString deviceName);
QString getDefaultDeviceName(KyDeviceType deviceType);
void setDefaultDevice(KyDeviceType deviceType, QString deviceName);
void saveDeviceEnableState(QString deviceName, bool enable);
void deleteDeviceEnableState(QString deviceName);
void getDeviceEnableState(int type, QMap<QString, bool> &map);

class TabPage : public QWidget
{
    Q_OBJECT
public:
    explicit TabPage(QWidget *parent = nullptr);
    ~TabPage();

//    void updateDefaultDevice(QString &deviceName);
//    QString getDefaultDevice();
    static void showDesktopNotify(const QString &message, QString soundName);

    void hideSetting() {
        if (nullptr != m_settingsFrame) {
            m_settingsFrame->hide();
            m_inactivatedNetDivider->hide();
            m_inactivatedNetFrame->setMinimumHeight(INACTIVE_AREA_MIN_HEIGHT + 100);
        }
    }
    void showSetting() {
        if (nullptr != m_settingsFrame) {
            m_inactivatedNetFrame->setMinimumHeight(INACTIVE_AREA_MIN_HEIGHT);
            m_settingsFrame->show();
            m_inactivatedNetDivider->show();
        }
    }

signals:
    void deviceStatusChanged();
    void deviceNameChanged(QString oldName, QString newName, int type);
    void activateFailed(QString errorMessage);
    void deactivateFailed(QString errorMessage);

protected:
    void initUI();
//    virtual void initDevice() = 0;//初始化默认设备
    virtual void initDeviceCombox() = 0;//初始化设备选择下拉框
    QVBoxLayout * m_mainLayout = nullptr;
    QFrame * m_titleFrame = nullptr;
    QHBoxLayout * m_titleLayout = nullptr;
    QLabel * m_titleLabel = nullptr;
    KSwitchButton * m_netSwitch = nullptr;
    Divider * m_titleDivider = nullptr;

    QFrame * m_activatedNetFrame = nullptr;
    QVBoxLayout * m_activatedNetLayout = nullptr;
    QLabel * m_activatedNetLabel = nullptr;
    Divider * m_activatedNetDivider = nullptr;

    QFrame * m_inactivatedNetFrame = nullptr;
    QVBoxLayout * m_inactivatedNetLayout = nullptr;
    QLabel * m_inactivatedNetLabel = nullptr;
    QScrollArea * m_inactivatedNetListArea = nullptr;
    QVBoxLayout * m_inactivatedAreaLayout = nullptr;

    Divider * m_inactivatedNetDivider = nullptr;

    QFrame * m_settingsFrame = nullptr;
    QHBoxLayout * m_settingsLayout = nullptr;
    KBorderlessButton *m_settingsBtn = nullptr;

    //临时增加的下拉框选择网卡区域
    QFrame * m_deviceFrame = nullptr;
    QHBoxLayout * m_deviceLayout = nullptr;
    QLabel * m_deviceLabel = nullptr;
    QComboBox * m_deviceComboBox = nullptr;
    QLabel * m_tipsLabel = nullptr;

public slots:
    virtual void onDeviceComboxIndexChanged(int currentIndex) = 0;
    void onPaletteChanged();

};

#endif // TABPAGE_H
