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
#ifndef MULTIPLEDNSWIDGET_H
#define MULTIPLEDNSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QAbstractItemView>
#include <QString>
#include <QList>
#include <QHostAddress>
#include <QDebug>

#include "listitemedit.h"

class MultipleDnsWidget: public QWidget
{
    Q_OBJECT

public:
    MultipleDnsWidget(const QRegExp &rx, QWidget *parent = nullptr);
    ~MultipleDnsWidget() = default;
    void setEditEnabled(bool state);
    QList<QHostAddress> getDns() const;
    void setDnsListText(const QList<QHostAddress> &dns);

private:
    void initUI();
    void initComponent();
    void AddOneDnsItem(QListWidget *listWidget);
    void RemoveOneDnsItem(QListWidgetItem *aItem, QListWidget *listWidget);

    QLabel *m_mulDnsLabel;
    QListWidget  *m_dnsListWidget = nullptr;
    QPushButton *m_addDnsBtn;
    QPushButton *m_removeDnsBtn;
    QRegExp m_regExp;

private Q_SLOTS:
    void setDnsListWidgetStyle();
    void onAddBtnClicked();
    void onRemoveBtnClicked();

};

#endif // MULTIPLEDNSWIDGET_H
