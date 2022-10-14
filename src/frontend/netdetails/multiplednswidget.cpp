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
#include "multiplednswidget.h"

#include <QApplication>

#define DNS_LISTWIDGET_HEIGHT 76
#define BUTTON_SIZE 36,36
#define ITEM_HEIGHT 36

MultipleDnsWidget::MultipleDnsWidget(const QRegExp &rx, QWidget *parent)
    : m_regExp(rx),
      QWidget(parent)
{
    initUI();
    initComponent();
}

void MultipleDnsWidget::initUI()
{
    QVBoxLayout *mulDnsVLayout = new QVBoxLayout(this);
    mulDnsVLayout->setContentsMargins(0, 0, 0, 0);

    m_mulDnsLabel = new QLabel(this);
    m_mulDnsLabel->setText(tr("DNS server:")); //DNS服务器：
    m_dnsListWidget = new QListWidget(this);
    m_dnsListWidget->setFixedHeight(DNS_LISTWIDGET_HEIGHT);
    m_dnsListWidget->setBackgroundRole(QPalette::Base);
    m_dnsListWidget->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_dnsListWidget->setFrameShape(QFrame::Shape::StyledPanel);
    m_dnsListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setDnsListWidgetStyle();

    m_addDnsBtn = new QPushButton(this);
    m_addDnsBtn->setFixedSize(BUTTON_SIZE);
    m_addDnsBtn->setProperty("useButtonPalette", true);
    m_addDnsBtn->setIcon(QIcon::fromTheme("list-add-symbolic"));
    m_removeDnsBtn = new QPushButton(this);
    m_removeDnsBtn->setFixedSize(BUTTON_SIZE);
    m_removeDnsBtn->setProperty("useButtonPalette", true);
    m_removeDnsBtn->setIcon(QIcon::fromTheme("list-remove-symbolic"));
    m_removeDnsBtn->setEnabled(false);
    QHBoxLayout *btnHLayout = new QHBoxLayout();
    btnHLayout->setContentsMargins(0, 0, 0, 0);
    btnHLayout->setSpacing(1);
    btnHLayout->setAlignment(Qt::AlignLeft);
    btnHLayout->addWidget(m_addDnsBtn);
    btnHLayout->addWidget(m_removeDnsBtn);

    mulDnsVLayout->addWidget(m_mulDnsLabel, Qt::AlignLeft);
    mulDnsVLayout->addWidget(m_dnsListWidget);
    mulDnsVLayout->addLayout(btnHLayout);
}

void MultipleDnsWidget::initComponent()
{
    connect(qApp, &QApplication::paletteChanged, this, &MultipleDnsWidget::setDnsListWidgetStyle);
    connect(m_addDnsBtn, &QPushButton::clicked, this, &MultipleDnsWidget::onAddBtnClicked);
    connect(m_removeDnsBtn, &QPushButton::clicked, this, &MultipleDnsWidget::onRemoveBtnClicked);
    connect(m_dnsListWidget, &QListWidget::itemClicked, this, [=]() {
        if (m_dnsListWidget->count() < 1) {
            m_removeDnsBtn->setEnabled(false);
        } else {
            m_removeDnsBtn->setEnabled(true);
        }
    });
}

void MultipleDnsWidget::setEditEnabled(bool state)
{
    m_addDnsBtn->setEnabled(state);

    if (!state) {
        m_dnsListWidget->clear();
    }
}

QList<QHostAddress> MultipleDnsWidget::getDns() const
{
    QStringList dnsList;
    dnsList.clear();
    QList<QHostAddress> ipv4dnsList;
    ipv4dnsList.clear();
    int row = 0;
    QString aDns;
    while (m_dnsListWidget->count() > row) {
        aDns = m_dnsListWidget->item(row)->text();
        if (!dnsList.contains(aDns)) {
            dnsList << aDns;
            ipv4dnsList << QHostAddress(aDns);
        }
        row ++;
    }
    return ipv4dnsList;
}

void MultipleDnsWidget::setDnsListText(const QList<QHostAddress> &dns)
{
    m_dnsListWidget->clear();
    for (QHostAddress str: dns) {
        QListWidgetItem *dnsListWidgetItem = new QListWidgetItem(m_dnsListWidget);
        dnsListWidgetItem->setSizeHint(QSize(0,ITEM_HEIGHT));
        dnsListWidgetItem->setText(str.toString());
    }
}

void MultipleDnsWidget::AddOneDnsItem(QListWidget *listWidget)
{
    QListWidgetItem *dnsListWidgetItem = new QListWidgetItem(listWidget);
    dnsListWidgetItem->setSizeHint(QSize(0,ITEM_HEIGHT));
    dnsListWidgetItem->setFlags(dnsListWidgetItem->flags() | Qt::ItemIsEditable);
    listWidget->addItem(dnsListWidgetItem);
    listWidget->setCurrentItem(dnsListWidgetItem);

    ListItemEdit *dnsListItemEdit = new ListItemEdit(m_regExp);
    listWidget->setItemDelegateForRow(listWidget->currentIndex().row() , dnsListItemEdit);
    listWidget->editItem(dnsListWidgetItem);
}

void MultipleDnsWidget::RemoveOneDnsItem(QListWidgetItem *aItem, QListWidget *listWidget)
{
    if (aItem) {
        listWidget->removeItemWidget(aItem);
        delete aItem;
    }
}

void MultipleDnsWidget::setDnsListWidgetStyle()
{
    QPalette mpal(m_dnsListWidget->palette());
    mpal.setColor(QPalette::Base, qApp->palette().base().color());
    mpal.setColor(QPalette::AlternateBase, qApp->palette().alternateBase().color());
    m_dnsListWidget->setAlternatingRowColors(true);
    m_dnsListWidget->setPalette(mpal);
}

void MultipleDnsWidget::onAddBtnClicked()
{
    //避免重复添加空白项
    if (m_dnsListWidget->currentItem()) {
        if (m_dnsListWidget->currentItem()->text().isEmpty()) {
            m_dnsListWidget->removeItemWidget(m_dnsListWidget->currentItem());
            delete m_dnsListWidget->currentItem();
        }
    }

    AddOneDnsItem(m_dnsListWidget);
    m_removeDnsBtn->setEnabled(true);
}

void MultipleDnsWidget::onRemoveBtnClicked()
{
    QListWidgetItem *aItem = m_dnsListWidget->currentItem();
    if (!aItem) {
        return;
    }
    RemoveOneDnsItem(aItem, m_dnsListWidget);
    if (m_dnsListWidget->count()< 1) {
        m_removeDnsBtn->setEnabled(false);
    }
}
