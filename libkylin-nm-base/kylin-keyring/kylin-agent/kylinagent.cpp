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
#include "kylinagent.h"
#include <QApplication>
#include <QTranslator>

KylinAgent::KylinAgent(QObject *parent) : QObject(parent)
{
    QLocale local;
    QString locale = /*QLocale::system().*/local.name();
    QTranslator* translator = new QTranslator(this);
    if (translator->load(":/translations/kylin-keyring_"+ locale + ".qm")) {
        QApplication::installTranslator(translator);
    } else {
//        qWarning() << "Translations load fail";
    }
}

KylinAgent::~KylinAgent()
{

}

void KylinAgent::startKylinAgent()
{
    m_secretAgnet = new KylinSecretAgent("kylinagent", pWidget, this);
}

//void KylinAgent::stopKylinAgent()
//{
//    delete m_kylinAgnetObject;
//    m_kylinAgnetObject = nullptr;
//    return;
//}
