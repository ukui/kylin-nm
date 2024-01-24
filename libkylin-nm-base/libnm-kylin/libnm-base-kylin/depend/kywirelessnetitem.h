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
#ifndef KYWIRELESSNETITEM_H
#define KYWIRELESSNETITEM_H

#include <QString>
#include "kywirelessconnectoperation.h"

class KyWirelessNetItem
{
public:
    QString m_NetSsid = "";
    QString m_connectUuid = "";
    QString m_dbusPath = "";
    KySecuType m_connectSecuType = KySecuType::NONE;
    bool m_isApConnection = false;
    QString m_secuType = "";
    int m_signalStrength = 0;
};

#endif // KYWIRELESSNETITEM_H
