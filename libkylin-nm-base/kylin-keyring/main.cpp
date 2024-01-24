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
#include <QApplication>
#include <QDebug>
#include <QWidget>
#include "kylinagent.h"
#include <QTranslator>

bool isAppRuning();

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication a(argc, argv);

    KylinAgent* agent = new KylinAgent();
    agent->startKylinAgent();

    QTranslator trans_global;
    qDebug() << "QLocale " << QLocale();
    if (trans_global.load(QLocale(), "kylin-keyring", "_", "/usr/share/kylin-keyring/"))
    {
        a.installTranslator(&trans_global);
        qDebug()<<"Translations load success";
    } else {
        qWarning() << "Translations load fail";
    }

    return a.exec();
}
