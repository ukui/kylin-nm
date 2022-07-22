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
#ifndef FIREWALLDIALOG_H
#define FIREWALLDIALOG_H

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include "kwidget.h"
#include "kdialog.h"

using namespace kdk;

class FirewallDialog : public KDialog
{
    Q_OBJECT
public:
    explicit FirewallDialog(KDialog *parent = nullptr);
    ~FirewallDialog();

private:
    void initUI();

    QLabel * m_iconLabel = nullptr;
    QLabel * m_contentLabel = nullptr;
    QLabel * m_suggestLabel = nullptr;
    QVBoxLayout *m_dialogLayout = nullptr;
    QPushButton *m_YesBtn = nullptr;
    QPushButton *m_NoBtn = nullptr;

signals:
    void setPublicNetMode();
    void setPrivateNetMode();
};
#endif // FIREWALLDIALOG_H
