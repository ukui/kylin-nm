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
#ifndef KYLINWIREDCONNECTOPERATION_H
#define KYLINWIREDCONNECTOPERATION_H

#include <QSettings>
#include <QDir>

#include "kylinnetworkresourcemanager.h"
#include "kylinconnectsetting.h"
#include "kylinconnectoperation.h"

const QString WIRED_NETWORK_STATE_CONF_FILE = QDir::homePath() + "/.config/ukui/kylin-nm-wired.ini";

class KyWiredConnectOperation : public KyConnectOperation
{
    Q_OBJECT
public:
    explicit KyWiredConnectOperation(QObject *parent = nullptr);

    void createWiredConnect(KyConnectSetting &connectSettingsInfo);
};

#endif // KYLINWIREDCONNECTOPERATION_H
