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
#include <QListWidget>
#include <QListWidgetItem>
#include <QScreen>

#include"listitem.h"

#define LOG_FLAG "[tabPage]"
#define PANEL_SETTINGS "org.ukui.panel.settings"
#define PANEL_SIZE_KEY "panelsize"
#define PANEL_POSITION_KEY "panelposition"
#define ICONSIZE 16,16

TabPage::TabPage(QWidget *parent) : QWidget(parent)
{
    initUI();
    initPanelGSettings();
    connect(qApp, &QApplication::paletteChanged, this, &TabPage::onPaletteChanged);
}

TabPage::~TabPage()
{
    delete m_titleDivider;
    delete m_activatedNetDivider;
    delete m_inactivatedNetDivider;
}

bool TabPage::checkTimeIsOut(qint64 msTimeOut) {
    static qint64 lastTime=0;
    static QElapsedTimer systemUptime;
    if(!systemUptime.isValid()) {
        systemUptime.start();
        return true;
    }
    if ((systemUptime.elapsed()-lastTime)>=msTimeOut) {
        lastTime=systemUptime.elapsed();
        return true;
    }
    return false;
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
    m_netSwitch->setTranslucent(true);
    m_titleLayout->addWidget(m_titleLabel);
    m_titleLayout->addStretch();
    m_titleLayout->addWidget(m_netSwitch);
    m_titleDivider = new Divider(true, this);

    m_loadIcons.append(QIcon::fromTheme("ukui-loading-1-symbolic"));
    m_loadIcons.append(QIcon::fromTheme("ukui-loading-2-symbolic"));
    m_loadIcons.append(QIcon::fromTheme("ukui-loading-3-symbolic"));
    m_loadIcons.append(QIcon::fromTheme("ukui-loading-4-symbolic"));
    m_loadIcons.append(QIcon::fromTheme("ukui-loading-5-symbolic"));
    m_loadIcons.append(QIcon::fromTheme("ukui-loading-6-symbolic"));
    m_loadIcons.append(QIcon::fromTheme("ukui-loading-7-symbolic"));
    m_waitTimer = new QTimer(this);
    connect(m_waitTimer, &QTimer::timeout, this, &TabPage::updateLoadingIcon);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setProperty("useIconHighlightEffect", 0x2);
    m_statusLabel->hide();
    m_titleLayout->addWidget(m_statusLabel);

    //临时增加的下拉框选择网卡区域
    m_deviceFrame = new QFrame(this);
    m_deviceFrame->setFixedHeight(TITLE_FRAME_HEIGHT);
    m_deviceLayout = new QHBoxLayout(m_deviceFrame);
    m_deviceLayout->setContentsMargins(DEVICE_LAYOUT_MARGINS);
    m_deviceFrame->setLayout(m_deviceLayout);
    m_deviceLabel = new QLabel(m_deviceFrame);
    m_deviceLabel->setText(tr("Current Device"));
    m_deviceComboBox = new QComboBox(m_deviceFrame);
    m_deviceComboBox->setMinimumWidth(DEVICE_COMBOBOX_WIDTH);
    m_deviceComboBox->setMaximumWidth(DEVICE_COMBOBOX_WIDTH_MAX);
    m_deviceComboBox->setSizeAdjustPolicy(QComboBox::SizeAdjustPolicy::AdjustToContents);

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
    m_activatedNetLabel->setFixedHeight(TEXT_HEIGHT);
    m_activatedNetLayout->addWidget(m_activatedNetLabel);
    m_activatedNetDivider = new Divider(true, this);

    m_inactivatedNetFrame = new QFrame(this);
    m_inactivatedNetFrame->setMinimumHeight(INACTIVE_AREA_MIN_HEIGHT);
    m_inactivatedNetLayout = new QVBoxLayout(m_inactivatedNetFrame);
    m_inactivatedNetLayout->setContentsMargins(NET_LAYOUT_MARGINS);
//    m_inactivatedNetLayout->setSpacing(NET_LAYOUT_SPACING);
    m_inactivatedNetFrame->setLayout(m_inactivatedNetLayout);

    m_inactivatedNetLabel = new QLabel(m_inactivatedNetFrame);
    m_inactivatedNetLabel->setContentsMargins(TEXT_MARGINS);
    m_inactivatedNetLabel->setFixedHeight(TEXT_HEIGHT);

//    m_inactivatedNetListArea = new QScrollArea(m_inactivatedNetFrame);
//    m_inactivatedNetListArea->setFrameShape(QFrame::Shape::NoFrame);
//    m_inactivatedNetListArea->setWidgetResizable(true);
//    m_inactivatedNetListArea->setBackgroundRole(QPalette::Base);
//    m_inactivatedNetListArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    m_inactivatedNetListArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_inactivatedNetListArea = new QWidget(m_inactivatedNetFrame);
    m_inactivatedAreaLayout = new QVBoxLayout(m_inactivatedNetListArea);
    m_inactivatedAreaLayout->setSpacing(MAIN_LAYOUT_SPACING);
    m_inactivatedAreaLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
//    m_inactivatedNetListArea->setLayout(m_inactivatedAreaLayout);

    m_inactivatedNetLayout->addWidget(m_inactivatedNetLabel);
    m_inactivatedNetLayout->addWidget(m_inactivatedNetListArea);

    m_inactivatedNetDivider = new Divider(true, this);
    m_settingsFrame = new QFrame(this);
    m_settingsFrame->setFixedHeight(TITLE_FRAME_HEIGHT);

    m_settingsLayout = new QHBoxLayout(m_settingsFrame);
    m_settingsLayout->setContentsMargins(SETTINGS_LAYOUT_MARGINS);

//    m_settingsBtn = new KBorderlessButton(m_settingsFrame);
//    m_settingsBtn->setText(tr("Settings"));
//    m_settingsLayout->addWidget(m_settingsBtn);

    m_settingsLabel = new KyLable(m_settingsFrame);
    m_settingsLabel->setCursor(Qt::PointingHandCursor);
    m_settingsLabel->setText(tr("Settings"));
    m_settingsLabel->setScaledContents(true);

    m_settingsLayout->addWidget(m_settingsLabel);
    m_settingsLayout->addStretch();
    m_settingsFrame->setLayout(m_settingsLayout);

    m_NoDeviceLabel = new KyLable(this);
    m_NoDeviceLabel->hide();
    m_NoDeviceLabel->setText(tr(""));
    m_NoDeviceLabel->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    m_mainLayout->addWidget(m_titleFrame);
    //临时增加的设备选择区域
    m_mainLayout->addWidget(m_deviceFrame);
    m_mainLayout->addWidget(m_titleDivider);
    m_mainLayout->addWidget(m_activatedNetFrame);
    m_mainLayout->addWidget(m_activatedNetDivider);
    m_mainLayout->addWidget(m_inactivatedNetFrame);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_NoDeviceLabel);
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_inactivatedNetDivider);
    m_mainLayout->addWidget(m_settingsFrame);

//    QPalette pal = m_inactivatedNetListArea->palette();
//    pal.setBrush(QPalette::Base, QColor(0,0,0,0));     //背景透明
//    m_inactivatedNetListArea->setPalette(pal);
    onPaletteChanged();
}

void TabPage::initPanelGSettings()
{
    const QByteArray id(PANEL_SETTINGS);
    if (QGSettings::isSchemaInstalled(id)) {
        if (m_panelGSettings == nullptr) {
            m_panelGSettings = new QGSettings(id, QByteArray(), this);
        }
        if (m_panelGSettings->keys().contains(PANEL_POSITION_KEY)) {
            m_panelPosition = m_panelGSettings->get(PANEL_POSITION_KEY).toInt();
        }
        if (m_panelGSettings->keys().contains(PANEL_SIZE_KEY)) {
            m_panelSize = m_panelGSettings->get(PANEL_SIZE_KEY).toInt();
        }
        connect(m_panelGSettings, &QGSettings::changed, this, [&] (const QString &key) {
            if (key == PANEL_POSITION_KEY) {
                m_panelPosition = m_panelGSettings->get(PANEL_POSITION_KEY).toInt();
            }
            if (key == PANEL_SIZE_KEY) {
                m_panelSize = m_panelGSettings->get(PANEL_SIZE_KEY).toInt();
            }
        });
    }
}

void TabPage::onPaletteChanged()
{
    QPalette labPal = m_activatedNetLabel->palette();
    QColor color = this->palette().color(QPalette::PlaceholderText);
    labPal.setColor(QPalette::WindowText, color);
    m_activatedNetLabel->setPalette(labPal);
    m_inactivatedNetLabel->setPalette(labPal);

    if (m_deviceComboBox->view()) {
        QPalette view_pal = m_deviceComboBox->view()->palette();
        QColor view_color = this->palette().color(QPalette::Active, QPalette::Button);
        view_pal.setColor(QPalette::Base, view_color);
        view_pal.setColor(QPalette::Text, this->palette().color(QPalette::Text));
        view_pal.setColor(QPalette::Foreground, this->palette().color(QPalette::Foreground));
        m_deviceComboBox->setPalette(view_pal);
        m_deviceComboBox->view()->setPalette(view_pal);
    }
}

int TabPage::getCurrentLoadRate(QString dev, long *save_rate, long *tx_rate)
{
    FILE * net_dev_file; //文件指针
    char buffer[1024]; //文件中的内容暂存在字符缓冲区里
    //size_t bytes_read; //实际读取的内容大小
    char * match; //用以保存所匹配字符串及之后的内容
    char * device;//将QString转为Char *
    QByteArray ba = dev.toLatin1(); // must
    device = ba.data();
    int counter = 0;
    //int i = 0;
    char tmp_value[128];

    if ((NULL == device) || (NULL == save_rate) || (NULL == tx_rate)) {
        qDebug() << LOG_FLAG << "parameter pass error" ;
        return -1;
    }

    if ((net_dev_file = fopen("/proc/net/dev", "r")) == NULL) {
        //打开文件/pro/net/dev/，从中读取流量数据
        qDebug() << LOG_FLAG << "error occurred when try to open file /proc/net/dev/";
        return -1;
    }
    memset(buffer, 0, sizeof(buffer));

    while (fgets(buffer, sizeof(buffer), net_dev_file) != NULL) {
        match = strstr(buffer, device);

        if (NULL == match) {
            // qDebug()<<"No eth0 keyword to find!";
            continue;
        } else {
            match = match + qstrlen(device) + qstrlen(":"); //地址偏移到冒号
            sscanf(match, "%ld ", save_rate);
            memset(tmp_value, 0, sizeof(tmp_value));
            sscanf(match, "%s ", tmp_value);
            match = match + qstrlen(tmp_value);
            for (size_t i=0; i<qstrlen(buffer); ++i) {
                if (0x20 == *match) {
                    match ++;
                } else {
                    if (8 == counter) {
                        sscanf(match, "%ld ", tx_rate);
                    }
                    memset(tmp_value, 0, sizeof(tmp_value));
                    sscanf(match, "%s ", tmp_value);
                    match = match + qstrlen(tmp_value);
                    counter ++;
                }
            }
        }
    }

    fclose(net_dev_file);
    net_dev_file = nullptr;

    return 0; //返回成功
}

QRect TabPage::caculatePositionWithPanel(const int windowWidth, const int windowHeight)
{
    QRect availableGeo = QGuiApplication::screenAt(QCursor::pos())->geometry();
    int x, y;
    int margin = 8;

    switch (m_panelPosition)
    {
    case PanelPosition::Top:
        x = availableGeo.x() + availableGeo.width() - windowWidth - margin;
        y = availableGeo.y() + m_panelSize + margin;
        break;
    case PanelPosition::Bottom:
        x = availableGeo.x() + availableGeo.width() - windowWidth - margin;
        y = availableGeo.y() + availableGeo.height() - m_panelSize - windowHeight - margin;
        break;
    case PanelPosition::Left:
        x = availableGeo.x() + m_panelSize + margin;
        y = availableGeo.y() + availableGeo.height() - windowHeight - margin;
        break;
    case PanelPosition::Right:
        x = availableGeo.x() + availableGeo.width() - m_panelSize - windowWidth - margin;
        y = availableGeo.y() + availableGeo.height() - windowHeight - margin;
        break;
    }
    return QRect(x, y, windowWidth, windowHeight);
}

void TabPage::onSetNetSpeed(QListWidget* m_activatedNetListWidget, bool isEmpty, QString dev)
{
    //未连接不显示网速
    QListWidgetItem* activeitem = m_activatedNetListWidget->item(0);
    ListItem *p_item = (ListItem *)m_activatedNetListWidget->itemWidget(activeitem);
    if (isEmpty) {
        p_item->m_lbLoadUp->hide();
        p_item->m_lbLoadDown->hide();
        p_item->m_lbLoadDownImg->hide();
        p_item->m_lbLoadUpImg->hide();
        return;
    }

    if (this->isVisible()) {

        if (getCurrentLoadRate(dev, &start_rcv_rates, &start_tx_rates) == -1) {
            start_rcv_rates = end_rcv_rates;
            return;
        }

        long int delta_rcv = (start_rcv_rates - end_rcv_rates) / 1024;
        long int delta_tx = (start_tx_rates - end_tx_rates) / 1024;

        //简易滤波
        if (delta_rcv < 0 || delta_tx < 0) {
            delta_rcv = 0;
            delta_tx = 0;
        }
        else if (end_rcv_rates == 0 || end_tx_rates == 0){
            delta_rcv = 0;
            delta_tx = 0;
        }

        end_rcv_rates = start_rcv_rates;
        end_tx_rates = start_tx_rates;

        int rcv_num = delta_rcv;
        int tx_num = delta_tx;

        QString str_rcv = 0;
        QString str_tx = 0;

        if (rcv_num < 1024) {
            str_rcv = QString::number(rcv_num) + "KB/s";
        } else {
            int remainder;
            if (rcv_num % 1024 < 100) {
                remainder = 0;
            } else {
                remainder = (rcv_num % 1024) / 100;
            }
            str_rcv = QString::number(rcv_num / 1024) + "."  + QString::number(remainder) + "MB/s";
        }

        if (tx_num < 1024) {
            str_tx = QString::number(tx_num) + "KB/s";
        } else {
            int remainder;
            if (tx_num % 1024 < 100) {
                remainder = 0;
            } else {
                remainder = (tx_num % 1024)/100;
            }
            str_tx = QString::number(tx_num / 1024) + "."  + QString::number(remainder) + "MB/s";
        }
        p_item->m_lbLoadDown->setText(str_rcv);
        p_item->m_lbLoadUp->setText(str_tx);
        if (!p_item->m_hoverButton->isVisible()) {
            p_item->m_lbLoadDown->show();
            p_item->m_lbLoadUp->show();
            p_item->m_lbLoadDownImg->show();
            p_item->m_lbLoadUpImg->show();
        }
    }
}

void TabPage::showDesktopNotify(const QString &message, QString soundName)
{
    QDBusInterface iface("org.freedesktop.Notifications",
                         "/org/freedesktop/Notifications",
                         "org.freedesktop.Notifications",
                         QDBusConnection::sessionBus());
    QStringList actions;  //跳转动作
    actions.append("default");
    actions.append("kylin-nm");          //默认动作：点击消息体时打开麒麟录音
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

void getDeviceEnableState(int type, QMap<QString, bool> &map)
{
    map.clear();
    if (type != WIRED && type != WIRELESS) {
        qDebug() << "getDeviceEnableState but wrong type";
        return;
    }

    KyNetworkDeviceResourse * kdr = new KyNetworkDeviceResourse();
    QStringList wiredDevList,wirelessDevList;
    wiredDevList.clear();
    wirelessDevList.clear();

    if (type == WIRED) {
        kdr->getNetworkDeviceList(NetworkManager::Device::Type::Ethernet, wiredDevList);
        if (!wiredDevList.isEmpty()) {
            for (int i = 0; i < wiredDevList.size(); ++i) {
                QString devName = wiredDevList.at(i);
                map.insert(devName, kdr->getDeviceManaged(devName));
            }
        }
    } else if (type == WIRELESS) {
        kdr->getNetworkDeviceList(NetworkManager::Device::Type::Wifi, wirelessDevList);
        if (!wirelessDevList.isEmpty()) {
            for (int i = 0; i < wirelessDevList.size(); ++i) {
                QString devName = wirelessDevList.at(i);
                map.insert(devName, kdr->getDeviceManaged(devName));
            }
        }
    }

    delete kdr;
    kdr = nullptr;
}

bool getOldVersionWiredSwitchState(bool &state)
{
    QSettings * m_settings = new QSettings(CONFIG_FILE_PATH, QSettings::IniFormat);
    QVariant value = m_settings->value("lan_switch_opened");

    if (!value.isValid()) {
        delete m_settings;
        m_settings = nullptr;
        return false;
    }
    state = value.toBool();
    m_settings->remove("lan_switch_opened");
    delete m_settings;
    m_settings = nullptr;
    return true;
}

void TabPage::showNoDeiceInfo(bool visible,QString text)
{
    m_NoDeviceLabel->setText(text);
    if(visible) {
        m_inactivatedNetFrame->hide();
        m_activatedNetFrame->hide();
        //m_deviceFrame->hide();
        m_activatedNetDivider->hide();
        m_inactivatedNetDivider->hide();
        m_settingsFrame->hide();

        m_NoDeviceLabel->resize(m_NoDeviceLabel->sizeHint()); // 设置标签大小以适应内容
        m_NoDeviceLabel->show();
        m_NoDeviceFlag=true;
    } else {
        m_NoDeviceLabel->hide();
        m_inactivatedNetFrame->show();
        m_activatedNetFrame->show();
        m_activatedNetDivider->show();
        m_inactivatedNetDivider->show();
        //m_deviceFrame->show();
        m_settingsFrame->show();
        m_NoDeviceFlag=false;
    }
}

bool TabPage::isNoDevice()
{
    return m_NoDeviceFlag;
}

void TabPage::updateLoadingIcon()
{
    if (m_currentIconIndex > 6) {
        m_currentIconIndex = 0;
    }
    m_statusLabel->setPixmap(m_loadIcons.at(m_currentIconIndex).pixmap(ICONSIZE));
    m_currentIconIndex ++;
}

void TabPage::startLoading()
{
    m_waitTimer->start(FRAME_SPEED);
    m_netSwitch->hide();
    m_statusLabel->setFocus();
    m_statusLabel->show();
}

void TabPage::stopLoading()
{
    m_waitTimer->stop();
    m_statusLabel->clearFocus();
    m_statusLabel->hide();
    m_netSwitch->show();
}

QString TabPage::changeDeviceStateText(QString deviceName,int state)
{
    QString finalShow="";
    finalShow=deviceName+"  "+(state==2? tr("connected"):tr(""));
    return finalShow;
}

void TabPage::updateDeviceConnectState(QString deviceName,int flag)
{
    int index=0;
    int oldState=0;
    QString devStrState;
    if (!deviceName.isEmpty() && m_deviceState.contains(deviceName)) {
        oldState=m_deviceState[deviceName];
        if(oldState!=flag) {
            m_deviceState[deviceName]=flag;
            index=m_deviceComboBox->findData(deviceName);
            if(index<0) {
                qDebug() << "cannt find  " << deviceName;
            }
            devStrState=changeDeviceStateText(deviceName,flag);
            QFontMetrics fm(m_deviceComboBox->font());
            int textWidth = fm.horizontalAdvance(devStrState);
            if (textWidth > DEVICE_COMBOBOX_WIDTH) {
                QString completeStr = devStrState;
                devStrState = fm.elidedText(devStrState, Qt::ElideRight, DEVICE_COMBOBOX_WIDTH);
                m_deviceComboBox->setToolTip(completeStr);
            }
            m_deviceComboBox->setItemText(index,devStrState);
            m_deviceComboBox->setItemData(index,deviceName);
        }
    }
}

void TabPage::replaceDeviceConnectState(QString oldName, QString newName)
{
    int flag = 0;
    if (!newName.isEmpty() && m_deviceState.contains(oldName) && oldName!=newName) {
        flag=m_deviceState[oldName];
        m_deviceState.remove(oldName);
        m_deviceState[newName]=flag;//不存在即创建
    }
}
