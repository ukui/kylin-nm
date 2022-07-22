/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include "tabpage.h"
#include <qsettings.h>
#include <QDBusInterface>
#include <QLabel>
#include <QApplication>
#include <QDBusReply>

TabPage::TabPage(QWidget *parent) : QWidget(parent)
{
    initUI();
    connect(qApp, &QApplication::paletteChanged, this, &TabPage::onPaletteChanged);
}

TabPage::~TabPage()
{
    delete m_titleDivider;
    delete m_activatedNetDivider;
    delete m_inactivatedNetDivider;
}

void TabPage::initUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_mainLayout->setSpacing(MAIN_LAYOUT_SPACING);
    this->setLayout(m_mainLayout);

    m_titleFrame = new QFrame(this);
    m_titleFrame->setFixedHeight(TITLE_FRAME_HEIGHT);
    m_titleLayout = new QHBoxLayout(m_titleFrame);
    m_titleLayout->setContentsMargins(TITLE_LAYOUT_MARGINS);
    m_titleLabel = new QLabel(m_titleFrame);
    m_netSwitch = new KSwitchButton(m_titleFrame);
    m_titleLayout->addWidget(m_titleLabel);
    m_titleLayout->addStretch();
    m_titleLayout->addWidget(m_netSwitch);
    m_titleDivider = new Divider(this);

    //临时增加的下拉框选择网卡区域
    m_deviceFrame = new QFrame(this);
    m_deviceFrame->setFixedHeight(TITLE_FRAME_HEIGHT);
    m_deviceLayout = new QHBoxLayout(m_deviceFrame);
    m_deviceLayout->setContentsMargins(DEVICE_LAYOUT_MARGINS);
    m_deviceFrame->setLayout(m_deviceLayout);
    m_deviceLabel = new QLabel(m_deviceFrame);
    m_deviceLabel->setText(tr("Current Device"));
    m_deviceComboBox = new QComboBox(m_deviceFrame);
    m_deviceComboBox->setFixedWidth(DEVICE_COMBOBOX_WIDTH);

    m_tipsLabel = new QLabel(m_deviceFrame);
    m_tipsLabel->setText(tr("Devices Closed!"));
    m_deviceLayout->addWidget(m_deviceLabel);
    m_deviceLayout->addStretch();
    m_deviceLayout->addWidget(m_deviceComboBox);
    m_deviceLayout->addWidget(m_tipsLabel);
    connect(m_deviceComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TabPage::onDeviceComboxIndexChanged);

    m_activatedNetFrame = new QFrame(this);
    m_activatedNetFrame->setMaximumHeight(ACTIVE_AREA_MAX_HEIGHT);
    m_activatedNetLayout = new QVBoxLayout(m_activatedNetFrame);
    m_activatedNetLayout->setContentsMargins(ACTIVE_NET_LAYOUT_MARGINS);
//    m_activatedNetLayout->setSpacing(NET_LAYOUT_SPACING);
    m_activatedNetLabel = new QLabel(m_activatedNetFrame);
    m_activatedNetLabel->setContentsMargins(TEXT_MARGINS);
    m_activatedNetLayout->addWidget(m_activatedNetLabel);
    m_activatedNetDivider = new Divider(this);

    m_inactivatedNetFrame = new QFrame(this);
    m_inactivatedNetFrame->setMinimumHeight(INACTIVE_AREA_MIN_HEIGHT);
    m_inactivatedNetLayout = new QVBoxLayout(m_inactivatedNetFrame);
    m_inactivatedNetLayout->setContentsMargins(NET_LAYOUT_MARGINS);
//    m_inactivatedNetLayout->setSpacing(NET_LAYOUT_SPACING);
    m_inactivatedNetFrame->setLayout(m_inactivatedNetLayout);

    m_inactivatedNetLabel = new QLabel(m_inactivatedNetFrame);
    m_inactivatedNetLabel->setContentsMargins(TEXT_MARGINS);

    m_inactivatedNetListArea = new QScrollArea(m_inactivatedNetFrame);
    m_inactivatedNetListArea->setFrameShape(QFrame::Shape::NoFrame);
    m_inactivatedNetListArea->setWidgetResizable(true);
    m_inactivatedNetListArea->setBackgroundRole(QPalette::Base);
    m_inactivatedNetListArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_inactivatedNetListArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_inactivatedAreaLayout = new QVBoxLayout(m_inactivatedNetListArea);
    m_inactivatedAreaLayout->setSpacing(MAIN_LAYOUT_SPACING);
    m_inactivatedAreaLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivatedNetListArea->setLayout(m_inactivatedAreaLayout);

    m_inactivatedNetLayout->addWidget(m_inactivatedNetLabel);
    m_inactivatedNetLayout->addWidget(m_inactivatedNetListArea);

    m_inactivatedNetDivider = new Divider(this);
    m_settingsFrame = new QFrame(this);
    m_settingsFrame->setFixedHeight(TITLE_FRAME_HEIGHT);

    m_settingsLayout = new QHBoxLayout(m_settingsFrame);
    m_settingsLayout->setContentsMargins(SETTINGS_LAYOUT_MARGINS);

    m_settingsBtn = new KBorderlessButton(m_settingsFrame);
    m_settingsBtn->setText(tr("Settings"));

    m_settingsLayout->addWidget(m_settingsBtn);
    m_settingsLayout->addStretch();
    m_settingsFrame->setLayout(m_settingsLayout);

    m_mainLayout->addWidget(m_titleFrame);
    //临时增加的设备选择区域
    m_mainLayout->addWidget(m_deviceFrame);
    m_mainLayout->addWidget(m_titleDivider);
    m_mainLayout->addWidget(m_activatedNetFrame);
    m_mainLayout->addWidget(m_activatedNetDivider);
    m_mainLayout->addWidget(m_inactivatedNetFrame);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_inactivatedNetDivider);
    m_mainLayout->addWidget(m_settingsFrame);

    QPalette pal = m_inactivatedNetListArea->palette();
    pal.setBrush(QPalette::Base, QColor(0,0,0,0));     //背景透明
    m_inactivatedNetListArea->setPalette(pal);

    onPaletteChanged();
}

void TabPage::onPaletteChanged()
{
    QPalette labPal = m_activatedNetLabel->palette();
    QColor color = qApp->palette().color(QPalette::PlaceholderText);
    labPal.setColor(QPalette::WindowText, color);
    m_activatedNetLabel->setPalette(labPal);
    m_inactivatedNetLabel->setPalette(labPal);

    if (m_deviceComboBox->view()) {
        QPalette view_pal = m_deviceComboBox->view()->palette();
        QColor view_color = qApp->palette().color(QPalette::Active, QPalette::Button);
        view_pal.setColor(QPalette::Base, view_color);
        m_deviceComboBox->setPalette(view_pal);
        m_deviceComboBox->view()->setPalette(view_pal);
    }
}

void TabPage::showDesktopNotify(const QString &message, QString soundName)
{
    QDBusInterface iface("org.freedesktop.Notifications",
                         "/org/freedesktop/Notifications",
                         "org.freedesktop.Notifications",
                         QDBusConnection::sessionBus());
    QStringList actions;  //跳转动作
    actions.append("kylin-nm");
    actions.append("default");          //默认动作：点击消息体时打开麒麟录音
    QMap<QString, QVariant> hints;
    if (!soundName.isEmpty()) {
        hints.insert("sound-name",soundName); //添加声音
    }
    QList<QVariant> args;
    args<<(tr("Kylin NM"))
       <<((unsigned int) 0)
       <<QString("gnome-dev-ethernet")
       <<tr("kylin network applet desktop message") //显示的是什么类型的信息
       <<message //显示的具体信息
       <<actions
       <<hints
       <<(int)-1;
    iface.callWithArgumentList(QDBus::AutoDetect,"Notify",args);
}

QString getDefaultDeviceName(KyDeviceType deviceType)
{
    QString defaultDevice = "";

    QString key;
    switch (deviceType) {
    case WIRED:
        key = "wired";
        break;
    case WIRELESS:
        key = "wireless";
        break;
    default:
        return defaultDevice;
    }

    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("DEFAULTCARD");
    defaultDevice = m_settings->value(key).toString();
    m_settings->endGroup();

    delete m_settings;
    m_settings = nullptr;

    return defaultDevice;
}

void setDefaultDevice(KyDeviceType deviceType, QString deviceName)
{
    QString key;
    switch (deviceType) {
    case WIRED:
        key = "wired";
        break;
    case WIRELESS:
        key = "wireless";
        break;
    default:
        return;
    }

    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("DEFAULTCARD");
    m_settings->setValue(key, deviceName);
    m_settings->endGroup();
    m_settings->sync();

    delete m_settings;
    m_settings = nullptr;

    return;
}

bool checkDeviceExist(KyDeviceType deviceType, QString deviceName)
{
    NetworkManager::Device::Type type;
    switch (deviceType) {
    case WIRED:
        type = NetworkManager::Device::Type::Ethernet;
        break;
    case WIRELESS:
        type = NetworkManager::Device::Type::Wifi;
        break;
    default:
        return false;
        break;
    }

    KyNetworkDeviceResourse * kdr = new KyNetworkDeviceResourse();
    QStringList devList;
    devList.clear();

    kdr->getNetworkDeviceList(type, devList);
    delete kdr;
    return devList.contains(deviceName);
}

void saveDeviceEnableState(QString deviceName, bool enable)
{
    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("CARDEABLE");
    m_settings->setValue(deviceName, enable);
    m_settings->endGroup();
    m_settings->sync();
    delete m_settings;
    m_settings = nullptr;
    return;
}

void deleteDeviceEnableState(QString deviceName)
{
    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("CARDEABLE");
    m_settings->remove(deviceName);
    m_settings->endGroup();
    m_settings->sync();
    delete m_settings;
    m_settings = nullptr;
    return;
}

void getDeviceEnableState(int type, QMap<QString, bool> &map)
{
    map.clear();
    if (!QFile::exists(CONFIG_FILE_PATH)) {
        qDebug() << "CONFIG_FILE_PATH not exist";
        return;
    }
    if (type != WIRED && type != WIRELESS) {
        qDebug() << "getDeviceEnableState but wrong type";
        return;
    }

    KyNetworkDeviceResourse * kdr = new KyNetworkDeviceResourse();
    QStringList wiredDevList,wirelessDevList;
    wiredDevList.clear();
    wirelessDevList.clear();

    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    m_settings->beginGroup("CARDEABLE");

    if (type == WIRED) {
        kdr->getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, wiredDevList);
        if (!wiredDevList.isEmpty()) {
            for (int i = 0; i < wiredDevList.size(); ++i) {
                if (!m_settings->contains(wiredDevList.at(i))) {
                    saveDeviceEnableState(wiredDevList.at(i),true);
                }
                bool enable = m_settings->value(wiredDevList.at(i), true).toBool();
                map.insert(wiredDevList.at(i), enable);
            }
        }
    } else if (type == WIRELESS) {
        kdr->getNetworkDeviceList(NetworkManager::Device::Type::Wifi, wirelessDevList);
        if (!wirelessDevList.isEmpty()) {
            for (int i = 0; i < wirelessDevList.size(); ++i) {
                bool enable = m_settings->value(wirelessDevList.at(i), true).toBool();
                map.insert(wirelessDevList.at(i), enable);
            }
        }
    }

    m_settings->endGroup();
    delete m_settings;
    m_settings = nullptr;
    delete kdr;
    kdr = nullptr;
    return;
}
