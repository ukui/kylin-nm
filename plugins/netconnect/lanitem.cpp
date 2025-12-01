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
#include "lanitem.h"
#include <kysdk/applications/accessinfohelper.h>
#include "klineframe.h"
#include <QApplication>
#include <QProcess>

#define FRAME_SPEED 150
#define LIMIT_TIME 60*1000
#define TOTAL_PAGE 8
#define MIN_TITLE_Width 80

#define THEME_QT_SCHEMA  "org.ukui.style"
#define MODE_QT_KEY      "style-name"
#define ICON_THEME       "iconThemeName"


LanItem::LanItem(bool isAcitve, QWidget *parent)
    : QPushButton(parent),isAcitve(isAcitve)
{
    this->setMinimumSize(550, 58);
    this->setProperty("useButtonPalette", true);
    this->setFlat(true);
    /* 创建主垂直布局 */
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QHBoxLayout *mLanLyt = new QHBoxLayout();
    mLanLyt->setContentsMargins(16,0,16,0);
    mLanLyt->setSpacing(16);
    mLanLyt->setAlignment(Qt::AlignVCenter);
    iconLabel = new QLabel(this);
    iconLabel->setProperty("useIconHighlightEffect", 0x2);
    titileLabel = new KLabel(this);
    titileLabel->setMinimumWidth(MIN_TITLE_Width);
    statusLabel = new QLabel(this);
    statusLabel->setProperty("useIconHighlightEffect", 0x2);
    statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    infoLabel = new GrayInfoButton(this);
    KDK_EXTEND_ALL_INFO_FORMAT(infoLabel, "NetConnect", "", "details info button of wired network");

    m_networkCheckFrame = new QFrame(this);
    m_warnLabel = new QLabel(this);
    m_warnLabel->setContentsMargins(0,0,0,0);
    m_warnLabel->setPixmap(QIcon::fromTheme("dialog-warning").pixmap(16,16));
    m_warnTextLabel = new QLabel(this);
    QHBoxLayout *netCheckFrameLyt = new QHBoxLayout(m_networkCheckFrame);
    netCheckFrameLyt->setContentsMargins(0, 0, 0, 0);
    netCheckFrameLyt->setSpacing(5);
    netCheckFrameLyt->addWidget(m_warnLabel);
    netCheckFrameLyt->addWidget(m_warnTextLabel);
    m_networkCheckFrame->setHidden(true);

    mLanLyt->addWidget(iconLabel);
    mLanLyt->addWidget(titileLabel,Qt::AlignLeft);
    mLanLyt->addStretch();
    mLanLyt->addWidget(statusLabel);
    mLanLyt->addWidget(m_networkCheckFrame);
    mLanLyt->addWidget(infoLabel);

    mainLayout->addLayout(mLanLyt);/* 将水平布局添加到主布局 */
    KHLineFrame *separator = new KHLineFrame(this); /* 分割线 */
    mainLayout->addWidget(separator);

    loadIcons.append(QIcon::fromTheme("ukui-loading-1-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-2-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-3-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-4-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-5-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-6-symbolic"));
    loadIcons.append(QIcon::fromTheme("ukui-loading-7-symbolic"));
    waitTimer = new QTimer(this);
    connect(waitTimer, &QTimer::timeout, this, &LanItem::updateIcon);

    const QByteArray id(THEME_QT_SCHEMA);
    if (QGSettings::isSchemaInstalled(id)) {
        QGSettings * styleGsettings = new QGSettings(id, QByteArray(), this);
        connect(styleGsettings, &QGSettings::changed, this, [=](QString key){
            if (ICON_THEME == key && m_warnLabel) {
                m_warnLabel->setPixmap(QIcon::fromTheme("dialog-warning").pixmap(16,16));
            }
        });
    }

}

LanItem::~LanItem()
{

}

void LanItem::updateIcon()
{
    if (currentIconIndex > 6) {
        currentIconIndex = 0;
    }
    statusLabel->setPixmap(loadIcons.at(currentIconIndex).pixmap(16,16));
    currentIconIndex ++;
}

void LanItem::startLoading()
{
    waitTimer->start(FRAME_SPEED);
    loading = true;
}

void LanItem::stopLoading(){
    waitTimer->stop();
    loading = false;
}

void LanItem::setNetworkCheckFrameHidden(bool state)
{
    m_networkCheckFrame->setHidden(state);
    statusLabel->setVisible(state);
}

void LanItem::setConnectivityWarn(ConnectivityType connectivityType)
{
    QString text;
    switch (connectivityType) {
    case NoConnectivity:
        text = QString("");
        setNetworkCheckFrameHidden(true);
        break;
    case Portal:
    case Limited:
        text = QString(tr("Connected, restricting access."));
        break;
    case Full:
    default:
        text = QString("");
        setNetworkCheckFrameHidden(true);
        break;
    }
    m_warnTextLabel->setText(text);
}

void LanItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(this->palette().base().color());

    QPalette pal = qApp->palette();
    QColor color = pal.color(QPalette::Button);
    color.setAlphaF(0.5);
    pal.setColor(QPalette::Button, color);
    this->setPalette(pal);

    QRect rect = this->rect();

    painter.drawRect(rect);
    QPushButton::paintEvent(event);
}

