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
#ifndef WPAWIFIDIALOG_H
#define WPAWIFIDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>
#include <QThread>
#include <QSettings>
#include <QSharedPointer>

class MainWindow;

namespace Ui {
class WpaWifiDialog;
}

class UpConnThread : public QThread
{
    Q_OBJECT

public:
    explicit UpConnThread(const QString &, const QString &);
    ~UpConnThread();

public:
    void run();
    QString conn_name = 0;
    QString m_user = 0;
    QString m_pwd = 0;

Q_SIGNALS:
    void connRes(int respond);
};

class WpaWifiDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WpaWifiDialog(QWidget *parent = nullptr, MainWindow *mw = 0, QString conname = 0);
    ~WpaWifiDialog();

private:
    Ui::WpaWifiDialog *ui;
    void initUI(); //初始化UI界面
    void initCombox(); //初始化所有下拉框
    void initConnect(); //初始化连接
    void getPwdFlag(); //获取是否每次询问密码
    bool setPwdFlag(const int&); //设置是否每次询问密码

protected:
    void paintEvent(QPaintEvent *event);

private:
    int pwd_flag = 2; //是否每次询问密码，0保存密码，2询问，默认不保存密码

    QString connectionName;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    bool isPress;
    QPoint winPos;
    QPoint dragPos;

    QStringList wifi_info;
    bool has_config;
    QStringList user_list;

    MainWindow *mw;

    void setEditorEnable(bool is_checking); //设置是否禁用输入
    void activateConnection();
    QStringList getWifiInfo(QString wifiName);
    bool appendWifiInfo(QString name, QString eap, QString inner, QString user, bool ask);
    bool appendWifiUser(QString name, QString user);

private:
    QWidget * mainWidget = nullptr;
    QVBoxLayout * mainLyt = nullptr; //弹窗整体布局

    QFrame * titleFrame = nullptr; //标题栏
    QHBoxLayout * titleLyt = nullptr;
    QLabel * titleLabel = nullptr;

    QFrame * nameFrame = nullptr; //网络名称
    QHBoxLayout * nameLyt = nullptr;
    QLabel * nameLabel = nullptr;
    QLineEdit * nameEditor = nullptr;

    QFrame * securityFrame = nullptr; //网络安全性
    QHBoxLayout * securityLyt = nullptr;
    QLabel * securityLabel = nullptr;
    QComboBox * securityCombox = nullptr;

    QFrame * hLine = nullptr; //分割线

    QFrame * eapFrame = nullptr; //EAP方法
    QHBoxLayout * eapLyt = nullptr;
    QLabel * eapLabel = nullptr;
    QComboBox * eapCombox = nullptr;

    QFrame * innerFrame = nullptr; //阶段2身份认证
    QHBoxLayout * innerLyt = nullptr;
    QLabel * innerLabel = nullptr;
    QComboBox * innerCombox = nullptr;

    QFrame * userFrame = nullptr; //用户名
    QHBoxLayout * userLyt = nullptr;
    QLabel * userLabel = nullptr;
    QLineEdit * userEditor = nullptr;

    QFrame * pwdFrame = nullptr; //密码
    QVBoxLayout * pwdLyt = nullptr;
    QFrame * pwdEditFrame = nullptr; //输入密码
    QHBoxLayout * pwdEditLyt = nullptr;
    QLabel * pwdLabel = nullptr;
    QLineEdit * pwdEditor = nullptr;
    QFrame * pwdShowFrame = nullptr; //显示密码
    QHBoxLayout * pwdShowLyt = nullptr;
    QCheckBox * pwdShowBtn = nullptr;
    QLabel * pwdShowLabel = nullptr;
    QFrame * askPwdFrame = nullptr; //每次询问密码
    QHBoxLayout * askPwdLyt = nullptr;
    QCheckBox * askPwdBtn = nullptr;
    QLabel * askPwdlabel = nullptr;

    QFrame * buttonFrame = nullptr; //按钮
    QHBoxLayout * buttonLyt = nullptr;
    QPushButton * cancelBtn = nullptr; //取消
    QPushButton * connectBtn = nullptr; //连接

Q_SIGNALS:
    void conn_done();
    void conn_failed();

private slots:
    void slot_on_connectBtn_clicked();
    void slot_line_edit_changed();
    void changeDialog();
};

#endif // WPAWIFIDIALOG_H
