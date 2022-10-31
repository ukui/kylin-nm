/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
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
#include "vpn.h"
#include "ui_vpn.h"

#include <QProcess>
#include <QMouseEvent>
#include <QDebug>

#define LABEL_RECT 17, 0, 105, 23
#define CONTENTS_MARGINS 0, 0, 0, 0
#define ITEM_MARGINS 16, 0, 16, 0
#define FRAME_MIN_SIZE 550, 60
#define FRAME_MAX_SIZE 16777215, 16777215
#define CONTECT_FRAME_MAX_SIZE 16777215, 60
#define HINT_TEXT_MARGINS 8, 0, 0, 0
#define FRAME_MIN_SIZE 550, 60
#define LABLE_MIN_WIDTH 188
#define COMBOBOX_MIN_WIDTH 200
#define LINE_MAX_SIZE 16777215, 1
#define LINE_MIN_SIZE 0, 1
#define ICON_SIZE   24,24
#define PASSWORD_FRAME_MIN_HIGHT 60
#define PASSWORD_FRAME_FIX_HIGHT 80
#define PASSWORD_FRAME_MIN_SIZE 550, 60
#define PASSWORD_FRAME_MAX_SIZE 16777215, 86
#define PASSWORD_ITEM_MARGINS 16, 12, 16, 14

#define KVpnSymbolic "ukui-vpn-symbolic"

Vpn::Vpn() : mFirstLoad(true)
{
    pluginName = tr("Vpn");
    pluginType = NETWORK;
}

Vpn::~Vpn()
{
    if (!mFirstLoad) {
        delete ui;
        ui = nullptr;
    }
}

QString Vpn::plugini18nName(){
    return pluginName;
}

int Vpn::pluginTypes(){
    return pluginType;
}

QWidget *Vpn::pluginUi(){
    if (mFirstLoad) {
        mFirstLoad = false;
        ui = new Ui::Vpn;
        pluginWidget = new QWidget;
        pluginWidget->setAttribute(Qt::WA_DeleteOnClose);
        ui->setupUi(pluginWidget);

        initComponent();
    }
    return pluginWidget;
}

const QString Vpn::name() const {

    return QStringLiteral("Vpn");
}

bool Vpn::isShowOnHomePage() const
{
    return true;
}

QIcon Vpn::icon() const
{
    return QIcon::fromTheme("ukui-vpn-symbolic");
}

bool Vpn::isEnable() const
{
    return true;
}

void Vpn::initComponent(){
    //在任务栏上显示图标
    //显示已连接时间
    m_topFrame = new QFrame(pluginWidget);
    m_topFrame->setMinimumSize(FRAME_MIN_SIZE);
    m_topFrame->setMaximumSize(FRAME_MAX_SIZE);
    m_topFrame->setFrameShape(QFrame::Box);

    QVBoxLayout *hotspotLyt = new QVBoxLayout(pluginWidget);
    hotspotLyt->setContentsMargins(0, 0, 0, 0);
    m_topFrame->setLayout(hotspotLyt);

    m_showFrame = new QFrame(m_topFrame);
    m_showFrame->setFrameShape(QFrame::Shape::NoFrame);
    m_showFrame->setMinimumSize(FRAME_MIN_SIZE);
    m_showFrame->setMaximumSize(CONTECT_FRAME_MAX_SIZE);
    QHBoxLayout *showLayout = new QHBoxLayout(m_showFrame);
    m_showLabel = new QLabel(tr("Open"), m_showFrame);
    m_showLabel->setMinimumWidth(LABLE_MIN_WIDTH);
    m_showBtn = new KSwitchButton(m_showFrame);
    showLayout->setContentsMargins(ITEM_MARGINS);
    showLayout->addWidget(m_showLabel);
    showLayout->addStretch();
    showLayout->addWidget(m_showBtn);

    m_showFrame->setLayout(showLayout);

    m_Line = myLine();

    m_timeFrame = new QFrame(m_topFrame);
    m_timeFrame->setFrameShape(QFrame::Shape::NoFrame);
    m_timeFrame->setMinimumSize(FRAME_MIN_SIZE);
    m_timeFrame->setMaximumSize(CONTECT_FRAME_MAX_SIZE);
    QHBoxLayout *timeLayout = new QHBoxLayout(m_timeFrame);
    m_timeLabel = new QLabel(tr("Open"), m_timeFrame);
    m_timeLabel->setMinimumWidth(LABLE_MIN_WIDTH);
    m_timeBtn = new KSwitchButton(m_timeFrame);
    timeLayout->setContentsMargins(ITEM_MARGINS);
    timeLayout->addWidget(m_timeLabel);
    timeLayout->addStretch();
    timeLayout->addWidget(m_timeBtn);

    m_timeFrame->setLayout(timeLayout);

    hotspotLyt->addWidget(m_showFrame);
    hotspotLyt->addWidget(m_Line);
    hotspotLyt->addWidget(m_timeFrame);
    hotspotLyt->setSpacing(0);

    //列表
    m_listFrame = new ItemFrame(pluginWidget);

    ui->verticalLayout_4->addWidget(m_topFrame);
    ui->verticalLayout_3->addWidget(m_listFrame);

    connect(m_listFrame->addWlanWidget, &AddNetBtn::clicked, this, [=]() {
        runExternalApp();
    });
}

void Vpn::runExternalApp(){
    QString cmd = "nm-connection-editor";
    QProcess process(this);
    process.startDetached(cmd);
}

QFrame* Vpn::myLine()
{
    QFrame *line = new QFrame(pluginWidget);
    line->setMinimumSize(QSize(LINE_MIN_SIZE));
    line->setMaximumSize(QSize(LINE_MAX_SIZE));
    line->setLineWidth(0);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    return line;
}




//增加一项
void Vpn::addOneVirtualItem(QStringList infoList)
{
    if (m_listFrame->itemMap.contains(infoList.at(1))) {
        qDebug() << "[Vpn]Already exist a virtual " << infoList.at(1);
        return;
    }

    qDebug() << "[Vpn]addOneVitualItem" << infoList.at(0);
    QString connName = infoList.at(0);
    QString connUuid = infoList.at(1);
    QString connDbusPath = infoList.at(2);
    VpnItem * lanItem = new VpnItem(pluginWidget);

    QString iconPath;
    iconPath = KVpnSymbolic;
    lanItem->statusLabel->setText("");

    QIcon searchIcon = QIcon::fromTheme(iconPath);
    lanItem->iconLabel->setPixmap(searchIcon.pixmap(searchIcon.actualSize(QSize(ICON_SIZE))));
    lanItem->titileLabel->setText(connName);

    lanItem->uuid = connUuid;
    lanItem->dbusPath = connDbusPath;

//    connect(lanItem->infoLabel, &GrayInfoButton::clicked, this, [=]{
//        // open landetail page
//        if (!m_interface->isValid()) {
//            return;
//        }
//        qDebug() << "[NetConnect]call showPropertyWidget" << deviceName << connUuid << __LINE__;
//        m_interface->call(QStringLiteral("showPropertyWidget"), deviceName, connUuid);
//        qDebug() << "[NetConnect]call showPropertyWidget respond" << __LINE__;
//    });

    lanItem->isAcitve = false;

    connect(lanItem, &QPushButton::clicked, this, [=] {
        if (lanItem->isAcitve || lanItem->loading) {
            deActiveConnect(lanItem->uuid, deviceName, WIRED_TYPE);
        } else {
            activeConnect(lanItem->uuid, deviceName, WIRED_TYPE);
        }
    });

    //记录到deviceFrame的itemMap中
    m_listFrame->itemMap.insert(connUuid, lanItem);
    int index = getInsertPos(connName);
    qDebug()<<"[NetConnect]addOneVirtualItem " << connName << " to " << deviceName << " list at pos:" << index;
    m_listFrame->lanItemLayout->insertWidget(index, lanItem);
}

void Vpn::removeOneVirtualItem(QString uuid)
{
    if (!m_listFrame->itemMap.contains(uuid)) {
            qDebug() << "[Vpn]not exist a virtual " << uuid;
            return;
    }

   qDebug()<<"[Vpn]removeOneVirtualItem " << uuid;

   m_listFrame->lanItemLayout->removeWidget(m_listFrame->itemMap[uuid]);
   delete m_listFrame->itemMap[uuid];
   m_listFrame->itemMap.remove(uuid);
}
