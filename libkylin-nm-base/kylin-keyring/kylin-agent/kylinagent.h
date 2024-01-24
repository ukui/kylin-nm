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
#ifndef KYLINAGENT_H
#define KYLINAGENT_H

#include "kylinsecretagent.h".h"

#if defined(LIBKYLIN_KEYRING_LIBRARY)
#  define LIBKYLIN_KEYRING_LIBRARY Q_DECL_EXPORT
#else
#  define LIBKYLIN_KEYRING_LIBRARY Q_DECL_IMPORT
#endif

class LIBKYLIN_KEYRING_LIBRARY KylinAgent : public QObject
{
public:
    explicit KylinAgent(QObject *parent = nullptr);
    ~KylinAgent();
    void startKylinAgent();
    void setParentWidget(QWidget *widget){
        pWidget = widget;
    }

private:
    KylinSecretAgent *m_secretAgnet;
    QWidget* pWidget = nullptr;
};


#endif // KYLINAGENT_H
