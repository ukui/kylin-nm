/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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

#ifndef DNSSETTINGWIDGET_H
#define DNSSETTINGWIDGET_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QDBusInterface>
#include <QPushButton>

#include "divider.h"

class DnsSettingWidget : public QDialog
{
    Q_OBJECT
public:
    explicit DnsSettingWidget(QString timeout = "", QString retry = "", QString tactic = "", QWidget *parent = nullptr);

    void getDnsSettings(QString& timeout, QString& retry, QString& tactic) {
        timeout = QString::number(m_timeoutComboBox->currentData().toInt());
        retry = QString::number(m_retryComboBox->currentData().toInt());
        tactic = m_tacticComboBox->currentData().toString();
    }

private:
    QWidget* m_titleWidget;
    QWidget* m_centerWidget;
    QWidget* m_bottomWidget;

    QString m_timeout;
    QString m_retry;
    QString m_tactic;

    QLabel* m_titleLabel;
    QLabel* m_tacticLabel;
    QLabel* m_timeoutLabel;
    QLabel* m_retryLabel;

    QComboBox* m_tacticComboBox;
    QComboBox* m_timeoutComboBox;
    QComboBox* m_retryComboBox;

    Divider *m_bottomDivider = nullptr;

    QPushButton *m_closeBtn;
    QPushButton *m_cancelBtn;
    QPushButton *m_confirmBtn;

    void initUi();
    void initConnect();

private Q_SLOTS:
    void onPaletteChanged();

};

#endif // DNSSETTINGWIDGET_H
