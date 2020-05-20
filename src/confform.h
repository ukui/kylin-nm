/*
 * Copyright (C) 2019 Tianjin KYLIN Information Technology Co., Ltd.
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

#ifndef CONFFORM_H
#define CONFFORM_H

#include "notifysend.h"

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QPoint>
#include <QPainter>
#include <QListView>

namespace Ui {
class ConfForm;
}

class ConfForm : public QWidget
{
    Q_OBJECT

public:
    explicit ConfForm(QWidget *parent = 0);
    ~ConfForm();

    void setProp(QString connName, QString v4method, QString addr, QString mask, QString gateway, QString dns, bool isActConf);

public slots:
    void cbTypeChanged(int index);

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();

    void on_btnCreate_clicked();

    void on_leName_textEdited(const QString &arg1);

    void on_leAddr_textEdited(const QString &arg1);

    void on_leGateway_textEdited(const QString &arg1);

    void on_leDns_textEdited(const QString &arg1);

    void on_leDns2_textEdited(const QString &arg1);

    void setEnableOfBtn();
    bool getTextEditState(QString text);
    void setBtnEnableFalse();

private:
    Ui::ConfForm *ui;

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    bool isPress;
    QPoint winPos;
    QPoint dragPos;
    bool isActConf;
    bool isCreateNewNet = false;
    bool isShowSaveBtn = true;

    QString labelQss, cbxQss, leQss, lineQss, btnOnQss, btnOffQss;

signals:
    void requestRefreshLanList(int updateType);
};

#endif // CONFFORM_H
