#ifndef KYLINHEADFILE_H
#define KYLINHEADFILE_H

#include "dlgconnhidwifi.h"
#include "dlgconnhidwifileap.h"
#include "dlgconnhidwifisecfast.h"
#include "dlgconnhidwifisecleap.h"
#include "dlgconnhidwifisecpeap.h"
#include "dlgconnhidwifisecpwd.h"
#include "dlgconnhidwifisectls.h"
#include "dlgconnhidwifisectunneltls.h"
#include "dlgconnhidwifiwep.h"
#include "dlgconnhidwifiwpa.h"

#include <QDebug>
#include <QFile>
#include <QPainter>
#include <QPainterPath>
#include <QListView>
#include <QString>
#include <KWindowEffects>

class MyQss
{
public:
    MyQss();

    QString labelQss, cbxQss, leQss, btnConnQss, btnCancelQss, lineQss, checkBoxQss, checkBoxCAQss;

};

#endif // KYLINHEADFILE_H
