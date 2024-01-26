/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2023, KylinSoft Co., Ltd.
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

#include "kynetworkicon.h"
#include "imageutil.h"

#define EXCELLENT_SIGNAL 80
#define GOOD_SIGNAL 55
#define OK_SIGNAL 30
#define LOW_SIGNAL 5
#define NONE_SIGNAL 0

#define EXCELLENT_SIGNAL_ICON   "network-wireless-signal-excellent-symbolic"
#define GOOD_SIGNAL_ICON        "network-wireless-signal-good-symbolic"
#define OK_SIGNAL_ICON          "network-wireless-signal-ok-symbolic"
#define LOW_SIGNAL_ICON         "network-wireless-signal-weak-symbolic"
#define NONE_SIGNAL_ICON        "network-wireless-signal-none-symbolic"

#define EXCELLENT_SIGNAL_LIMIT_ICON   "ukui-network-wireless-signal-excellent-error-symbolic"
#define GOOD_SIGNAL_LIMIT_ICON        "ukui-network-wireless-signal-good-error-symbolic"
#define OK_SIGNAL_LIMIT_ICON          "ukui-network-wireless-signal-ok-error-symbolic"
#define LOW_SIGNAL_LIMIT_ICON         "ukui-network-wireless-signal-weak-error-symbolic"
#define NONE_SIGNAL_LIMIT_ICON        "ukui-network-wireless-signal-none-error-symbolic"

static QPalette getPalette(bool isDark)
{
    QPalette palette = qApp->palette();

    //ukui-light palette UKUI3.1 亮主题色板
    QColor windowText_at(38, 38, 38),
           windowText_iat(0, 0, 0, 255 * 0.55),
           windowText_dis(0, 0, 0, 255 * 0.3),
           button_at(230, 230, 230),
           button_iat(230, 230, 230),
           button_dis(233, 233, 233),
           light_at(255, 255, 255),
           light_iat(255, 255, 255),
           light_dis(242, 242, 242),
           midlight_at(218, 218, 218),
           midlight_iat(218, 218, 218),
           midlight_dis(230, 230, 230),
           dark_at(77, 77, 77),
           dark_iat(77, 77, 77),
           dark_dis(64, 64, 64),
           mid_at(115, 115, 115),
           mid_iat(115, 115, 115),
           mid_dis(102, 102, 102),
           text_at(38, 38, 38),
           text_iat(38, 38, 38),
           text_dis(0, 0, 0, 255 * 0.3),
           brightText_at(0, 0, 0),
           brightText_iat(0, 0, 0),
           brightText_dis(0, 0, 0),
           buttonText_at(38, 38, 38),
           buttonText_iat(38, 38, 38),
           buttonText_dis(179, 179, 179),
           base_at(255, 255, 255),
           base_iat(245, 245, 245),
           base_dis(237, 237, 237),
           window_at(245, 245, 245),
           window_iat(237, 237, 237),
           window_dis(230, 230, 230),
           shadow_at(0, 0, 0, 255 * 0.16),
           shadow_iat(0, 0, 0, 255 * 0.16),
           shadow_dis(0, 0, 0, 255 * 0.21),
           highLightText_at(255, 255, 255),
           highLightText_iat(255, 255, 255),
           highLightText_dis(179, 179, 179),
           alternateBase_at(245, 245, 245),
           alternateBase_iat(245, 245, 245),
           alternateBase_dis(245, 245, 245),
           noRale_at(240, 240, 240),
           noRole_iat(240, 240, 240),
           noRole_dis(217, 217, 217),
           toolTipBase_at(255, 255, 255),
           toolTipBase_iat(255, 255, 255),
           toolTipBase_dis(255, 255, 255),
           toolTipText_at(38, 38, 38),
           toolTipText_iat(38, 38, 38),
           toolTipText_dis(38, 38, 38),
           placeholderText_at(0, 0, 0, 255 * 0.35),
           placeholderText_iat(0, 0, 0, 255 * 0.35),
           placeholderText_dis(0, 0, 0, 255 * 0.3);

    //ukui-dark  暗主题色板
    if (isDark) {
        windowText_at.setRgb(217, 217, 217);
        windowText_iat.setRgb(255, 255, 255, 255 * 0.55);
        windowText_dis.setRgb(255, 255, 255, 255 * 0.3);
        button_at.setRgb(55, 55, 59);
        button_iat.setRgb(55, 55, 59);
        button_dis.setRgb(46, 46, 46);
        light_at.setRgb(255, 255, 255);
        light_iat.setRgb(255, 255, 255);
        light_dis.setRgb(242, 242, 242);
        midlight_at.setRgb(95, 95, 98);
        midlight_iat.setRgb(95, 95, 98);
        midlight_dis.setRgb(79, 79, 82);
        dark_at.setRgb(38, 38, 38);
        dark_iat.setRgb(38, 38, 38);
        dark_dis.setRgb(26, 26, 26);
        mid_at.setRgb(115, 115, 115);
        mid_iat.setRgb(115, 115, 115);
        mid_dis.setRgb(102, 102, 102);
        text_at.setRgb(217, 217, 217);
        text_iat.setRgb(217, 217, 217);
        text_dis.setRgb(255, 255, 255, 255 * 0.3);
        brightText_at.setRgb(255, 255, 255);
        brightText_iat.setRgb(255, 255, 255);
        brightText_dis.setRgb(255, 255, 255);
        buttonText_at.setRgb(217, 217, 217);
        buttonText_iat.setRgb(217, 217, 217);
        buttonText_dis.setRgb(76, 76, 79);
        base_at.setRgb(29, 29, 29);
        base_iat.setRgb(28, 28, 28);
        base_dis.setRgb(36, 36, 36);
        window_at.setRgb(35, 36, 38);
        window_iat.setRgb(26, 26, 26);
        window_dis.setRgb(18, 18, 18);
        shadow_at.setRgb(0, 0, 0, 255 * 0.16);
        shadow_iat.setRgb(0, 0, 0, 255 * 0.16);
        shadow_dis.setRgb(0, 0, 0, 255 * 0.21);
        highLightText_at.setRgb(255, 255, 255);
        highLightText_iat.setRgb(255, 255, 255);
        highLightText_dis.setRgb(77, 77, 77);
        alternateBase_at.setRgb(38, 38, 38);
        alternateBase_iat.setRgb(38, 38, 38);
        alternateBase_dis.setRgb(38, 38, 38);
        noRale_at.setRgb(51, 51, 51);
        noRole_iat.setRgb(51, 51, 51);
        noRole_dis.setRgb(60, 60, 60);
        toolTipBase_at.setRgb(38, 38, 38);
        toolTipBase_iat.setRgb(38, 38, 38);
        toolTipBase_dis.setRgb(38, 38, 38);
        toolTipText_at.setRgb(217, 217, 217);
        toolTipText_iat.setRgb(217, 217, 217);
        toolTipText_dis.setRgb(217, 217, 217);
        placeholderText_at.setRgb(255, 255, 255, 255 * 0.35);
        placeholderText_iat.setRgb(255, 255, 255, 255 * 0.35);
        placeholderText_dis.setRgb(255, 255, 255, 255 * 0.3);
    }

    palette.setColor(QPalette::Active, QPalette::WindowText, windowText_at);
    palette.setColor(QPalette::Inactive, QPalette::WindowText, windowText_iat);
    palette.setColor(QPalette::Disabled, QPalette::WindowText, windowText_dis);

    palette.setColor(QPalette::Active, QPalette::Button, button_at);
    palette.setColor(QPalette::Inactive, QPalette::Button, button_iat);
    palette.setColor(QPalette::Disabled, QPalette::Button, button_dis);

    palette.setColor(QPalette::Active, QPalette::Light, light_at);
    palette.setColor(QPalette::Inactive, QPalette::Light, light_iat);
    palette.setColor(QPalette::Disabled, QPalette::Light, light_dis);

    palette.setColor(QPalette::Active, QPalette::Midlight, midlight_at);
    palette.setColor(QPalette::Inactive, QPalette::Midlight, midlight_iat);
    palette.setColor(QPalette::Disabled, QPalette::Midlight, midlight_dis);

    palette.setColor(QPalette::Active, QPalette::Dark, dark_at);
    palette.setColor(QPalette::Inactive, QPalette::Dark, dark_iat);
    palette.setColor(QPalette::Disabled, QPalette::Dark, dark_dis);

    palette.setColor(QPalette::Active, QPalette::Mid, mid_at);
    palette.setColor(QPalette::Inactive, QPalette::Mid, mid_iat);
    palette.setColor(QPalette::Disabled, QPalette::Mid, mid_dis);

    palette.setColor(QPalette::Active, QPalette::Text, text_at);
    palette.setColor(QPalette::Inactive, QPalette::Text, text_iat);
    palette.setColor(QPalette::Disabled, QPalette::Text, text_dis);

    palette.setColor(QPalette::Active, QPalette::BrightText, brightText_at);
    palette.setColor(QPalette::Inactive, QPalette::BrightText, brightText_iat);
    palette.setColor(QPalette::Disabled, QPalette::BrightText, brightText_dis);

    palette.setColor(QPalette::Active, QPalette::ButtonText, buttonText_at);
    palette.setColor(QPalette::Inactive, QPalette::ButtonText, buttonText_iat);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, buttonText_dis);

    palette.setColor(QPalette::Active, QPalette::Base, base_at);
    palette.setColor(QPalette::Inactive, QPalette::Base, base_iat);
    palette.setColor(QPalette::Disabled, QPalette::Base, base_dis);

    palette.setColor(QPalette::Active, QPalette::Window, window_at);
    palette.setColor(QPalette::Inactive, QPalette::Window, window_iat);
    palette.setColor(QPalette::Disabled, QPalette::Window, window_dis);

    palette.setColor(QPalette::Active, QPalette::Shadow, shadow_at);
    palette.setColor(QPalette::Inactive, QPalette::Shadow, shadow_iat);
    palette.setColor(QPalette::Disabled, QPalette::Shadow, shadow_dis);

    palette.setColor(QPalette::Active, QPalette::HighlightedText, highLightText_at);
    palette.setColor(QPalette::Inactive, QPalette::HighlightedText, highLightText_iat);
    palette.setColor(QPalette::Disabled, QPalette::HighlightedText, highLightText_dis);

    palette.setColor(QPalette::Active, QPalette::AlternateBase, alternateBase_at);
    palette.setColor(QPalette::Inactive, QPalette::AlternateBase, alternateBase_iat);
    palette.setColor(QPalette::Disabled, QPalette::AlternateBase, alternateBase_dis);

    palette.setColor(QPalette::Active, QPalette::NoRole, noRale_at);
    palette.setColor(QPalette::Inactive, QPalette::NoRole, noRole_iat);
    palette.setColor(QPalette::Disabled, QPalette::NoRole, noRole_dis);

    palette.setColor(QPalette::Active, QPalette::ToolTipBase, toolTipBase_at);
    palette.setColor(QPalette::Inactive, QPalette::ToolTipBase, toolTipBase_iat);
    palette.setColor(QPalette::Disabled, QPalette::ToolTipBase, toolTipBase_dis);

    palette.setColor(QPalette::Active, QPalette::ToolTipText, toolTipText_at);
    palette.setColor(QPalette::Inactive, QPalette::ToolTipText, toolTipText_iat);
    palette.setColor(QPalette::Disabled, QPalette::ToolTipText, toolTipText_dis);

#if (QT_VERSION >= QT_VERSION_CHECK(5,12,0))
    palette.setColor(QPalette::Active, QPalette::PlaceholderText, placeholderText_at);
    palette.setColor(QPalette::Inactive, QPalette::PlaceholderText, placeholderText_iat);
    palette.setColor(QPalette::Disabled, QPalette::PlaceholderText, placeholderText_dis);
#endif

    return palette;
}

KyNetworkIcon::KyNetworkIcon(QWidget *parent):
    QPushButton(parent)
{
    qRegisterMetaType<KySecuType>("KyConnectState");
    qRegisterMetaType<KySecuType>("KyConnectStatus");
    qRegisterMetaType<KyConnectionType>("KyConnectionType");

    this->setProperty("useIconHighlightEffect", 0x10);
    this->setProperty("needTranslucent", true);
    this->setFlat(true);
    this->setProperty("useButtonPalette", true);

    thread = new QThread;
    manager = new KyNetworkManager();
    manager->moveToThread(thread);
    connect(thread, &QThread::started, manager, &KyNetworkManager::kylinNetworkManagerInit);
    connect(thread, &QThread::finished, manager, &KyNetworkManager::deleteLater);

    connect(thread, &QThread::finished, [=](){
        qDebug() << "icon thread quit";
    });
    thread->start();

    while (!manager->isInitFinished()) {
        ::usleep(1000);
    }

    loadIcons.append(QIcon::fromTheme("kylin-network-1"));
    loadIcons.append(QIcon::fromTheme("kylin-network-2"));
    loadIcons.append(QIcon::fromTheme("kylin-network-3"));
    loadIcons.append(QIcon::fromTheme("kylin-network-4"));
    loadIcons.append(QIcon::fromTheme("kylin-network-5"));
    loadIcons.append(QIcon::fromTheme("kylin-network-6"));
    loadIcons.append(QIcon::fromTheme("kylin-network-7"));
    loadIcons.append(QIcon::fromTheme("kylin-network-8"));
    loadIcons.append(QIcon::fromTheme("kylin-network-9"));
    loadIcons.append(QIcon::fromTheme("kylin-network-10"));
    loadIcons.append(QIcon::fromTheme("kylin-network-11"));
    loadIcons.append(QIcon::fromTheme("kylin-network-12"));

    loadingTimer = new QTimer(this);
    connect(loadingTimer, &QTimer::timeout, this, &KyNetworkIcon::onSetTrayIconLoading);

    manager->getConnectStatus(iconStatus);
    updateIcon();
    initConnect();

    refreshTimer = new QTimer(this);
    connect(loadingTimer, &QTimer::timeout, this, &KyNetworkIcon::updateIcon);
    refreshTimer->start(10 * 1000);

    QPalette pal = qApp->palette();
    pal = getPalette(true);
    this->setPalette(pal);

    this->setCheckable(true);
}

KyNetworkIcon::~KyNetworkIcon()
{
    qDebug() << "~KyNetworkIcon 1";
    thread->quit();
    thread->wait();
    delete thread;
    thread = nullptr;
    qDebug() << "~KyNetworkIcon 2";
}

void KyNetworkIcon::paintEvent(QPaintEvent *event)
{
    QPalette pal = qApp->palette();
    pal = getPalette(true);
    QColor checkedColor = this->palette().brightText().color();
    checkedColor.setAlphaF(0.21);
    pal.setColor(QPalette::Active, QPalette::Highlight, checkedColor);
    this->setPalette(pal);
    return QPushButton::paintEvent(event);
}

void KyNetworkIcon::initConnect()
{
    connect(manager, &KyNetworkManager::wiredStateChange, this, &KyNetworkIcon::onWiredStateChange);
    connect(manager, &KyNetworkManager::wirelessStateChange, this, &KyNetworkIcon::onWirelessStateChange);

    connect(manager, &KyNetworkManager::connectStatusChanged , [=](KyConnectStatus status){
        iconStatus = status;
        updateIcon();
    });
}

void KyNetworkIcon::updateIcon()
{
    if (loadingTimer->isActive()) {
        return;
    }
    int signalStrength = 0;
    QIcon icon;

    if (iconStatus == LAN_CONNECTED) {
        this->setIcon(QIcon::fromTheme("network-wired-symbolic"));
        return;
    } else if (iconStatus == WLAN_CONNECTED
               || iconStatus == WLAN_CONNECTED_LIMITED) {
        signalStrength = manager->getAcivateWifiSignal();
    } else if (iconStatus == NOT_CONNECTED) {
        this->setIcon(QIcon::fromTheme("network-wired-disconnected-symbolic"));
        return;
    } else if (iconStatus == LAN_CONNECTED_LIMITED) {
        icon = QIcon::fromTheme("network-error-symbolic");
    }

    if (iconStatus == WLAN_CONNECTED) {
        if (signalStrength > EXCELLENT_SIGNAL){
            icon = QIcon::fromTheme(EXCELLENT_SIGNAL_ICON);
        } else if (signalStrength > GOOD_SIGNAL) {
            icon = QIcon::fromTheme(GOOD_SIGNAL_ICON);
        } else if (signalStrength > OK_SIGNAL) {
            icon = QIcon::fromTheme(OK_SIGNAL_ICON);
       } else if (signalStrength > LOW_SIGNAL) {
            icon = QIcon::fromTheme(LOW_SIGNAL_ICON);
       } else {
            icon = QIcon::fromTheme(NONE_SIGNAL_ICON);
       }
    } else if (iconStatus == WLAN_CONNECTED_LIMITED) {
        if (signalStrength > EXCELLENT_SIGNAL){
            icon = QIcon::fromTheme(EXCELLENT_SIGNAL_LIMIT_ICON);
        } else if (signalStrength > GOOD_SIGNAL) {
            icon = QIcon::fromTheme(GOOD_SIGNAL_LIMIT_ICON);
        } else if (signalStrength > OK_SIGNAL) {
            icon = QIcon::fromTheme(OK_SIGNAL_LIMIT_ICON);
       } else if (signalStrength > LOW_SIGNAL) {
            icon = QIcon::fromTheme(LOW_SIGNAL_LIMIT_ICON);
       } else {
            icon = QIcon::fromTheme(NONE_SIGNAL_LIMIT_ICON);
       }
    }

    this->setIcon(icon);
}

void KyNetworkIcon::startLoading()
{
    if (!loadingTimer->isActive()) {
        loadingTimer->start(60);
    }
}

void KyNetworkIcon::stopLoading()
{
    if (loadingTimer->isActive()) {
        loadingTimer->stop();
    }
}

void KyNetworkIcon::onSetTrayIconLoading()
{
    if (currentIconIndex > 11) {
        currentIconIndex = 0;
    }

    this->setIcon(loadIcons.at(currentIconIndex));
    currentIconIndex ++;
}

void KyNetworkIcon::onWiredStateChange(QString deviceName, QString uuid, KyConnectState state)
{
    Q_UNUSED(deviceName)
    Q_UNUSED(uuid)
    if (state == CONNECT_STATE_ACTIVATING
            || state == CONNECT_STATE_DEACTIVATING) {
        startLoading();
    } else {
        stopLoading();
        updateIcon();
    }
}

void KyNetworkIcon::onWirelessStateChange(QString deviceName, QString ssid, QString uuid, KyConnectState state)
{
    Q_UNUSED(deviceName)
    Q_UNUSED(ssid)
    Q_UNUSED(uuid)
    if (state == CONNECT_STATE_ACTIVATING
            || state == CONNECT_STATE_DEACTIVATING) {
        startLoading();
    } else {
        stopLoading();
        updateIcon();
    }
}
