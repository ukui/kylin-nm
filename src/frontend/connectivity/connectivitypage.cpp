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

#include "connectivitypage.h"
#include <QLayout>
#include <QFormLayout>
#include <QApplication>
#include <QGSettings>
#include <QDebug>
#include <QRegExpValidator>
#include <QDesktopWidget>

#include "windowmanager/windowmanager.h"
#include "kwindowsystem.h"
#include "kwindowsystem_export.h"
#include "kylinutil.h"

#define THEME_SCHAME "org.ukui.style"
#define COLOR_THEME "styleName"

#define  BOTTOM_LAYOUT_MARGINS  24, 16, 24, 24
#define  LAYOUT_SPACING  16

ConnectivityPage::ConnectivityPage(QString uri, QWidget *parent)
    :m_uri(uri), QDialog(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setFixedSize(380, 369);
    this->setWindowTitle(tr("Network connectivity detection"));
    setAttribute(Qt::WA_DeleteOnClose, false);
    KWindowSystem::setState(this->winId(), NET::SkipTaskbar | NET::SkipPager);
    centerToScreen();
    m_connectResource = new KyConnectResourse(this);
    initUi();
    initConnect();
    NetworkManager::Connectivity connectivity;
    m_connectResource->getConnectivity(connectivity);
    setWarning(connectivity);
    m_publicNetworkButton->setChecked(m_uri.isEmpty());
    m_intranetButton->setChecked(!m_uri.isEmpty());
    if (!m_uri.isEmpty()) {
        checkUri();
    }
}

void ConnectivityPage::initUi()
{
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setFixedWidth(380);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setContentsMargins(0,0,0,0);

    m_icon = new QLabel(this);
    m_statusText = new QLabel(this);
    m_statusText->setWordWrap(true);

    //如访问 Internet 受限，请切换网络 IP 连通性检测方式后再试。
    m_text = new QLabel(this);
    m_text->setText(tr("If access to the Internet is restricted, please switch the network IP connectivity detection method and try again."));
    m_text->setWordWrap(true);
    m_text->adjustSize();
//    m_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_publicTextLabel = new QLabel(this);
    m_intranetTextLabel = new QLabel(this);
    m_publicNetworkButton = new QRadioButton(this);
    m_intranetButton = new QRadioButton(this);
    m_publicTextLabel->setText(tr("Public network (default)"));
    m_publicTextLabel->setWordWrap(true);
    m_intranetTextLabel->setText(tr("Local area network (intranet)"));
    m_intranetTextLabel->setWordWrap(true);
    QWidget *radioBtnWidget = new QWidget(this);
    QFormLayout *formLayout = new QFormLayout(radioBtnWidget);
    formLayout->setContentsMargins(0, 0, 0, 0);
    formLayout->setVerticalSpacing(12);
    formLayout->addRow(m_publicNetworkButton, m_publicTextLabel);
    formLayout->addRow(m_intranetButton, m_intranetTextLabel);


    m_uriEdit = new QLineEdit(this);
    m_uriEdit->setText(m_uri);
    m_warningLabel = new QLabel(this);
    QPalette hintTextColor;
    hintTextColor.setColor(QPalette::WindowText, Qt::red);
    m_warningLabel->setPalette(hintTextColor);
    m_warningLabel->setWordWrap(true);

    m_confirmBtn = new QPushButton(this);
    m_confirmBtn->setText(tr("Confirm"));

    m_warningWidget = new QWidget(this);
    m_warningWidget->adjustSize();
    m_editWidget = new QWidget(this);
    m_centerWidget = new QWidget(this);
    m_centerWidget->setFixedWidth(380);
    m_bottomWidget = new QWidget(this);

    QHBoxLayout* warningLayout = new QHBoxLayout(m_warningWidget);
    warningLayout->setContentsMargins(0, 0, 0, 24);
    warningLayout->setSpacing(8);
    warningLayout->addWidget(m_icon);
    warningLayout->addWidget(m_statusText);
    warningLayout->addStretch();

    QVBoxLayout* editLayout = new QVBoxLayout(m_editWidget);
    editLayout->setContentsMargins(22,0,0,0);
    editLayout->setSpacing(0);
    editLayout->addWidget(m_uriEdit);
    editLayout->addWidget(m_warningLabel);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(m_scrollArea);
    mainLayout->addWidget(m_bottomWidget);
    this->setLayout(mainLayout);

    //中间页面
    QVBoxLayout *vLayout = new QVBoxLayout(m_centerWidget);
    vLayout->setContentsMargins(25, 17, 23, 0);
    vLayout->setSpacing(0);
    vLayout->addWidget(m_warningWidget);
    vLayout->addWidget(m_text);
    vLayout->addSpacing(10);
    vLayout->addWidget(radioBtnWidget);
    vLayout->addWidget(m_editWidget);

    //底部按钮
    QHBoxLayout* bottomLayout = new QHBoxLayout(m_bottomWidget);
    bottomLayout->setContentsMargins(BOTTOM_LAYOUT_MARGINS);
    bottomLayout->setSpacing(LAYOUT_SPACING);
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_confirmBtn);

    m_scrollArea->setWidget(m_centerWidget);
    m_scrollArea->setWidgetResizable(true);

    this->setWindowFlags(Qt::Dialog);
}

void ConnectivityPage::initConnect()
{
    connect(m_confirmBtn, &QPushButton::released, this, [=](){
        if (m_publicNetworkButton->isChecked()) {
            setConnectivityCheckSpareUriByGDbus("");
        } else {
            setConnectivityCheckSpareUriByGDbus(m_uriEdit->text());
        }
        close();
    });
    connect(m_publicNetworkButton, &QRadioButton::toggled, [&](bool checked){
        if (checked) {
            m_uriEdit->clear();
            m_uriEdit->setDisabled(true);
            m_confirmBtn->setEnabled(true);
            if (m_warningLabel) {
                m_warningLabel->clear();
                m_warningLabel->hide();
            }
        }
    });
    connect(m_intranetButton, &QRadioButton::toggled, [&](bool checked){
        if (checked) {
            checkUri();
            m_uriEdit->setDisabled(false);
            m_uriEdit->setFocus();
        }
    });
    connect(m_uriEdit, &QLineEdit::textChanged, this ,&ConnectivityPage::checkUri);
    connect(m_connectResource, &KyConnectResourse::connectivityChanged, this, &ConnectivityPage::setWarning);
}

void ConnectivityPage::centerToScreen()
{
    QDesktopWidget* m = QApplication::desktop();
    QRect desk_rect = m->screenGeometry(m->screenNumber(QCursor::pos()));
    int desk_x = desk_rect.width();
    int desk_y = desk_rect.height();
    int x = this->width();
    int y = this->height();
    this->move(desk_x / 2 - x / 2 + desk_rect.left(), desk_y / 2 - y / 2 + desk_rect.top());
}

void ConnectivityPage::setWarning(NetworkManager::Connectivity connectivity)
{
    if (NetworkManager::Connectivity::Full == connectivity) {
        //网络已连接，可正常访问 Internet 。
        m_icon->setPixmap(QIcon::fromTheme("ukui-dialog-success").pixmap(16,16));
        m_statusText->setText(tr("The network is connected and can access the Internet normally."));
    } else if (NetworkManager::Connectivity::Limited == connectivity
               || NetworkManager::Connectivity::Portal == connectivity) {
        //网络已连接，访问 Internet 受限。
        m_icon->setPixmap(QIcon::fromTheme("dialog-warning").pixmap(16,16));
        m_statusText->setText(tr("The network is connected and access to the Internet is restricted."));
    } else {
        qWarning() << "network status is " << connectivity << "should not show this page";
    }
}

void ConnectivityPage::checkUri()
{
    QString text = m_uriEdit->text();  //locationbar input data
    if (text.isEmpty()) {
        m_confirmBtn->setEnabled(false);
        m_warningLabel->setText(tr("Please enter the local area network (intranet) detection address"));
        m_warningLabel->show();
        return;
    }
    int pos = 0;

    QRegExp rx;
    //url regular expression
    rx.setPattern("(https?|ftp|file)://[-A-Za-z0-9+&@#/%?=~_|!:,.;]+[-A-Za-z0-9+&@#/%=~_|]");
    QRegExpValidator rv;
    rv.setRegExp(rx);
    //Test for a match between the url and the regular expression
    QValidator::State rvState = rv.validate(text, pos);
    if (rvState == QValidator::Acceptable) {
        m_confirmBtn->setEnabled(true);
        m_warningLabel->clear();
        m_warningLabel->hide();
    } else {
        m_confirmBtn->setEnabled(false);
        m_warningLabel->setText(tr("Format error"));
        m_warningLabel->show();
    }
}

void ConnectivityPage::closeEvent(QCloseEvent *event)
{
    Q_EMIT pageClose();
    return QWidget::closeEvent(event);
}
