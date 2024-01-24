/*
 * Copyright (C) 2023, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#include "kylinapconnectitem.h"

KyApConnectItem::KyApConnectItem(QObject *parent) : QObject(parent)
{
    m_connectName = "";
    m_connectUuid = "";
    m_connectSsid = "";
    m_ifaceName = "";
    m_password = "";
    m_band = "";
    m_isActivated = false;
}
