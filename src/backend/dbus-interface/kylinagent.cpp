/*
 * Copyright (C) 2020 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#include "kylinagent.h"
#include <QDebug>
// LCOV_EXCL_START
KylinAgent::KylinAgent(QObject *parent) : QObject(parent)
{
    m_secretAgnet = new KylinSecretAgent("kylinagent");
}
// LCOV_EXCL_STOP

// LCOV_EXCL_START
KylinAgent::~KylinAgent()
{
    if (nullptr != m_secretAgnet) {
        delete m_secretAgnet;
        m_secretAgnet = nullptr;
    }
}
// LCOV_EXCL_STOP

// LCOV_EXCL_START
void KylinAgent::stopKylinAgent()
{
    delete m_secretAgnet;
    m_secretAgnet = nullptr;
    return;
}
// LCOV_EXCL_STOP
