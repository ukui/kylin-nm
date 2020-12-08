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
#include "wpawifidialog.h"
#include "ui_wpawifidialog.h"
#include "kylin-network-interface.h"
#include "backthread.h"
#include "utils.h"

#include <KWindowEffects>
#include <sys/syslog.h>
#include <QFile>
#include <QStringListModel>
#include <QCompleter>

const QString CONFIG_FILE = "/tmp/wpaconf.ini";

UpConnThread::UpConnThread()
{
}

UpConnThread::~UpConnThread() {
}

void UpConnThread::run() {
    QString cmdStr = "nmcli connection up " + this->conn_name;
    int res = Utils::m_system(cmdStr.toUtf8().data());
    emit connRes(res);
}

WpaWifiDialog::WpaWifiDialog(QWidget *parent, QString conname) :
    QDialog(parent),
    ui(new Ui::WpaWifiDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowIcon(QIcon::fromTheme("kylin-network", QIcon(":/res/x/setup.png")));
//    this->setAttribute(Qt::WA_DeleteOnClose);


    connectionName = conname;
//    configPath = getenv("HOME") + CONFIG_FILE;

    QPainterPath path;
    auto rect = this->rect();
    rect.adjust(1, 1, -1, -1);
    path.addRoundedRect(rect, 6, 6);
    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));
    KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));

    initUI();
    initCombox();
    initConnect();
}

WpaWifiDialog::~WpaWifiDialog()
{
    delete ui;
}

void WpaWifiDialog::initUI() {
    mainWidget = new QWidget(this);
    mainLyt = new QVBoxLayout(mainWidget);
    mainWidget->setFixedSize(360, 504);

    titleFrame = new QFrame(mainWidget); //标题栏
    titleFrame->setFixedHeight(48);
    titleLyt = new QHBoxLayout(titleFrame);
    titleLabel = new QLabel(titleFrame);
    titleLabel->setText(tr("Connect Wifi"));
    titleLyt->addWidget(titleLabel);
    titleLyt->addStretch();
    titleFrame->setLayout(titleLyt);

    nameFrame = new QFrame(mainWidget); //网络名称
    nameFrame->setFixedHeight(48);
    nameLyt = new QHBoxLayout(nameFrame);
    nameLyt->setContentsMargins(24, 0, 24, 0);
    nameLabel = new QLabel(nameFrame);
    nameEditor = new QLineEdit(nameFrame);
    nameEditor->setFixedHeight(48);
    nameLabel->setFixedWidth(100);
    nameLabel->setText(tr("Connection name"));
    nameEditor->setText(connectionName);
    nameEditor->setEnabled(false);//暂时不允许编辑wifi名
    nameLyt->addWidget(nameLabel);
    nameLyt->addWidget(nameEditor);
    nameFrame->setLayout(nameLyt);

    securityFrame = new QFrame(mainWidget); //网络安全性
    securityFrame->setFixedHeight(48);
    securityLyt = new QHBoxLayout(securityFrame);
    securityLyt->setContentsMargins(24, 0, 24, 0);
    securityLabel = new QLabel(securityFrame);
    securityCombox = new QComboBox(securityFrame);
    securityCombox->setFixedHeight(48);
    securityLabel->setFixedWidth(100);
    securityLabel->setText(tr("Security"));
    securityLyt->addWidget(securityLabel);
    securityLyt->addWidget(securityCombox);
    securityFrame->setLayout(securityLyt);

    hLine = new QFrame(mainWidget); //分割线
    hLine->setFrameShape(QFrame::Shape::HLine);
    hLine->setLineWidth(0);
    hLine->setFixedHeight(1);
    hLine->setStyleSheet("QFrame{background-color: palette(base);}");

    eapFrame = new QFrame(mainWidget); //EAP方法
    eapFrame->setFixedHeight(48);
    eapLyt = new QHBoxLayout(eapFrame);
    eapLyt->setContentsMargins(24, 0, 24, 0);
    eapLabel = new QLabel(eapFrame);
    eapCombox = new QComboBox(eapFrame);
    eapCombox->setFixedHeight(48);
    eapLabel->setFixedWidth(100);
    eapLabel->setText(tr("EAP type"));
    eapLyt->addWidget(eapLabel);
    eapLyt->addWidget(eapCombox);
    eapFrame->setLayout(eapLyt);

    innerFrame = new QFrame(mainWidget); //阶段2身份认证
    innerFrame->setFixedHeight(48);
    innerLyt = new QHBoxLayout(innerFrame);
    innerLyt->setContentsMargins(24, 0, 24, 0);
    innerLabel = new QLabel(innerFrame);
    innerCombox = new QComboBox(innerFrame);
    innerCombox->setFixedHeight(48);
    innerLabel->setFixedWidth(100);
    innerLabel->setText(tr("inner authentication"));
    innerLyt->addWidget(innerLabel);
    innerLyt->addWidget(innerCombox);
    innerFrame->setLayout(innerLyt);

    userFrame = new QFrame(mainWidget); //用户名
    userFrame->setFixedHeight(48);
    userLyt = new QHBoxLayout(userFrame);
    userLyt->setContentsMargins(24, 0, 24, 0);
    userLabel = new QLabel(userFrame);
    userEditor = new QLineEdit(userFrame);
    userEditor->setFixedHeight(48);
    userLabel->setFixedWidth(100);
    userLabel->setText(tr("Username"));
    userLyt->addWidget(userLabel);
    userLyt->addWidget(userEditor);
    userFrame->setLayout(userLyt);

    pwdFrame = new QFrame(mainWidget); //密码
    pwdFrame->setFixedHeight(72);
    pwdLyt = new QVBoxLayout(pwdFrame);
    pwdLyt->setContentsMargins(0, 0, 0, 0);
    pwdLyt->setSpacing(0);
    pwdEditFrame = new QFrame(pwdFrame); //输入密码
    pwdEditFrame->setFixedHeight(48);
    pwdEditLyt = new QHBoxLayout(pwdEditFrame);
    pwdEditLyt->setContentsMargins(24, 0, 24, 0);
    pwdLabel = new QLabel(pwdEditFrame);
    pwdEditor = new QLineEdit(pwdEditFrame);
    pwdEditor->setEchoMode(QLineEdit::Password);
    pwdLabel->setText(tr("Password"));
    pwdLabel->setFixedWidth(100);
    pwdEditor->setFixedHeight(48);
    pwdEditLyt->addWidget(pwdLabel);
    pwdEditLyt->addWidget(pwdEditor);
    pwdEditFrame->setLayout(pwdEditLyt);
    pwdShowFrame = new QFrame(pwdFrame); //显示密码
    pwdShowFrame->setFixedHeight(24);
    pwdShowLyt = new QHBoxLayout(pwdShowFrame);
    pwdShowLyt->setContentsMargins(130, 0, 0, 0);
    pwdShowBtn = new QCheckBox(pwdShowFrame);
    pwdShowLabel = new QLabel(pwdShowFrame);
    pwdShowLabel->setFixedWidth(120);
    pwdShowBtn->setFixedSize(16, 16);
    pwdShowLabel->setText(tr("Show password"));
    pwdShowLyt->addWidget(pwdShowBtn);
    pwdShowLyt->addWidget(pwdShowLabel);
    pwdShowLyt->addStretch();
    pwdShowFrame->setLayout(pwdShowLyt);
    pwdLyt->addWidget(pwdEditFrame);
    pwdLyt->addWidget(pwdShowFrame);
    pwdFrame->setLayout(pwdLyt);

    buttonFrame = new QFrame(mainWidget); //按钮
    buttonLyt = new QHBoxLayout(buttonFrame);
    cancelBtn = new QPushButton(buttonFrame); //取消
    connectBtn = new QPushButton(buttonFrame); //连接
    connectBtn->setEnabled(false);
    cancelBtn->setFixedSize(72, 36);
    connectBtn->setFixedSize(72, 36);
    cancelBtn->setText(tr("Cancel"));
    connectBtn->setText(tr("Connect"));
    buttonLyt->addStretch();
    buttonLyt->addWidget(cancelBtn);
    buttonLyt->addWidget(connectBtn);
    buttonFrame->setLayout(buttonLyt);

    mainLyt->addWidget(titleFrame);
    mainLyt->addWidget(nameFrame);
    mainLyt->addWidget(securityFrame);
    mainLyt->addWidget(hLine);
    mainLyt->addWidget(eapFrame);
    mainLyt->addWidget(innerFrame);
    mainLyt->addWidget(userFrame);
    mainLyt->addWidget(pwdFrame);
    mainLyt->addWidget(buttonFrame);
    mainLyt->setSpacing(8);
    mainLyt->setContentsMargins(8, 16, 8, 16);
    mainWidget->setLayout(mainLyt);
}

void WpaWifiDialog::initCombox() {
    //wifi安全性
    securityCombox->addItem(tr("WPA & WPA2"));
//    securityCombox->setEnabled(false);
    //EAP方法
    QStringList eapStringList;
    eapStringList<< "PEAP" << "FAST" << "MD5" << "PWD" << "SIM" << "TLS" << "TTLS";
    for (int i = 0; i < eapStringList.length(); i++) {
        eapCombox->addItem(eapStringList.at(i), QString(eapStringList.at(i)).toLower());
    }
    //阶段2认证方式
    QStringList innerStringList;
    innerStringList<< "MSCHAPv2" << "MSCHAP" << "MD5" << "CHAP" << "OTP" << "GTC" << "PAP" << "TLS";
    for (int i = 0; i < innerStringList.length(); i++) {
        innerCombox->addItem(innerStringList.at(i), QString(innerStringList.at(i)).toLower());
    }
//    //读配置文件
    wifi_info = getWifiInfo(connectionName);
    if (wifi_info.isEmpty()) {
        has_config = false;
    } else {
        has_config = true;
        //读配置信息
        eapCombox->setCurrentIndex(eapCombox->findData(wifi_info.at(0)));
        innerCombox->setCurrentIndex(innerCombox->findData(wifi_info.at(1)));
        for (int i = 2; i < wifi_info.length(); i++) {
            user_list << wifi_info.at(i);
        }
        userEditor->setText(user_list.at(user_list.length() - 1));
        QCompleter *completer = new QCompleter(userEditor);
        QStringListModel * listModel = new QStringListModel(user_list, userEditor);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setModel(listModel);
        userEditor->setCompleter(completer);
    }
}
void WpaWifiDialog::initConnect() {
    //取消按钮
    connect(cancelBtn, &QPushButton::clicked, this, [ = ]() {
        this->close();
    });
    //连接按钮
    connect(connectBtn, &QPushButton::clicked, this, &WpaWifiDialog::slot_on_connectBtn_clicked);
    //显示密码
    connect(pwdShowBtn, &QCheckBox::clicked, this, [ = ]() {
        if (pwdShowBtn->isChecked()) {
            pwdEditor->setEchoMode(QLineEdit::Normal);
        } else {
            pwdEditor->setEchoMode(QLineEdit::Password);
        }
    });
    connect(pwdEditor, &QLineEdit::textChanged, this, &WpaWifiDialog::slot_line_edit_changed);
    connect(userEditor, &QLineEdit::textChanged, this, &WpaWifiDialog::slot_line_edit_changed);
}

void WpaWifiDialog::slot_line_edit_changed() {
    if (pwdEditor->text().length() >= 1 && userEditor->text().length() >= 1) {
        connectBtn->setEnabled(true);
    } else {
        connectBtn->setEnabled(false);
    }
}

void WpaWifiDialog::slot_on_connectBtn_clicked() {
    qDebug()<<"Clicked on connect Btn!";
    //先写配置文件
    if (has_config) {
        appendWifiUser(nameEditor->text(), userEditor->text());
    } else {
        appendWifiInfo(nameEditor->text(), eapCombox->currentData().toString(), innerCombox->currentData().toString(), userEditor->text());
        has_config = true;
    }
    QString cmdStr = "nmcli connection modify " + nameEditor->text() + " 802-1x.password " + pwdEditor->text();
    int res = Utils::m_system(cmdStr.toUtf8().data());
    if (res == 0) {
        //有网络配置文件，密码已修改，接下来修改用户名和其他配置，然后激活连接
        QString cmdStr_1 = "nmcli connection modify " + nameEditor->text() + " 802-1x.identity " + userEditor->text()
                           + " 802-1x.eap " + eapCombox->currentData().toString() + " 802-1x.phase2-auth " + innerCombox->currentData().toString();
        Utils::m_system(cmdStr_1.toUtf8().data());
        //激活连接
        activateConnection();
    } else {
        //无网络配置文件，需要新创建
        //获取网卡名称
        KylinDBus mkylindbus;
        QString wifi_card_name= mkylindbus.dbusWiFiCardName;
//        qDebug()<<"qDebug: con-name & ssid: "<<nameEditor->text()<<"\n"<<
//                  "qDebug: wifi card name(ifname): "<<wifi_card_name<<"\n"<<
//                  "qDebug: 802-1x.eap: "<<eapCombox->currentData().toString()<<"\n"<<
//                  "qDebug: 802-1x.phase2-auth: "<<innerCombox->currentData().toString()<<"\n"<<
//                  "qDebug: 802-1x.identity: "<<userEditor->text()<<"\n"<<
//                  "qDebug: 802-1x.password: "<<pwdEditor->text();
        QString create_cmd;
        create_cmd = "nmcli connection add con-name " + nameEditor->text() + " ifname "
                     + wifi_card_name + " ipv4.method auto type wifi ssid " + nameEditor->text()
                     + " 802-1x.eap " + eapCombox->currentData().toString() + " 802-1x.phase2-auth "
                     + innerCombox->currentData().toString() + " 802-1x.identity "
                     + userEditor->text() + " 802-1x.password " + pwdEditor->text() +
                     " wifi-sec.key-mgmt wpa-eap autoconnect no";
//        qDebug() << create_cmd;
        int res = Utils::m_system(create_cmd.toUtf8().data());
        if (res == 0) {
            syslog(LOG_DEBUG, "In function slot_on_connectBtn_clicked, created a wifi config named %s", nameEditor->text());
            qDebug() << "qDebug: created a wifi config successfully";
            //创建成功，激活连接
            activateConnection();
        } else {
            syslog(LOG_ERR, "execute 'nmcli connection add' in function 'slot_on_connectBtn_clicked' failed");
            qDebug() << "qDebug: create wpa wifi config file failed!";
        }
    }
}

void WpaWifiDialog::setEditorEnable(bool is_checking) {
    securityCombox->setEnabled(is_checking);
    eapCombox->setEnabled(is_checking);
    innerCombox->setEnabled(is_checking);
    userEditor->setEnabled(is_checking);
    pwdEditor->setEnabled(is_checking);
    pwdShowBtn->setEnabled(is_checking);
    pwdShowLabel->setEnabled(is_checking);
    cancelBtn->setEnabled(is_checking);
    connectBtn->setEnabled(is_checking);
}

void WpaWifiDialog::activateConnection() {
    UpConnThread * upThread = new UpConnThread();
    upThread->conn_name = nameEditor->text();
    connect(upThread, &UpConnThread::started, this, [ = ]() {
        //线程开始，开始校验密码，此时弹窗的连接按钮被禁用，所有输入框禁用
        setEditorEnable(false);
        //超时计时器
        QTimer * timeout = new QTimer(this);
        QObject::connect(timeout, &QTimer::timeout, this, [ = ](){
            //连接超时
            timeout->stop();
            timeout->deleteLater();
            QString cmdStr_2 = "nmcli connection down " + nameEditor->text();
            Utils::m_system(cmdStr_2.toUtf8().data());
            syslog(LOG_DEBUG, "execute 'nmcli connection up' in function 'activateConnection' time out");
            qDebug() << "qDebug: activate time out!";
        });
        //设置超时时间
        timeout->start(5000);
    });
    connect(upThread, &UpConnThread::connRes, this, [ = ](int res) {
        qDebug()<<"qDebug: Connect result is: "<<res;
        if (res != 0) {
            //连接错误或连接超时
            setEditorEnable(true);
            emit conn_failed();
        } else {
            //连接成功
            upThread->quit();
            upThread->wait();
            upThread->deleteLater();
            emit conn_done();
            syslog(LOG_DEBUG, "execute 'nmcli connection up' in function 'activateConnection' accepted");
            qDebug() << "qDebug: activated wpa wifi successfully";
            this->close();
        }
    });
    upThread->start();
}

QStringList WpaWifiDialog::getWifiInfo(QString wifiName) {
    //返回名为wifiName的企业wifi的配置信息，包括eap,inner和用户列表
    QStringList wlist;
    QFile file(CONFIG_FILE);
    if(!file.exists()) {
        return wlist;
    }
    QSharedPointer<QSettings>  autoSettings = QSharedPointer<QSettings>(new QSettings(CONFIG_FILE, QSettings::IniFormat));
    autoSettings.get()->beginGroup(wifiName);
    wlist << autoSettings.get()->value("eap").toString();
    wlist << autoSettings.get()->value("inner").toString();
    autoSettings.get()->beginReadArray(wifiName);
    for (int i = 0; ; i++) {
        autoSettings.get()->setArrayIndex(i);
        if (autoSettings.get()->value("user").toString().isEmpty()) {
            break;
        }
        wlist << autoSettings.get()->value("user").toString();
    }
    autoSettings.get()->endArray();
    autoSettings.get()->endGroup();
    return wlist;
}

bool WpaWifiDialog::appendWifiInfo(QString name, QString eap, QString inner, QString user) {
    //向配置文件添加名为name的wifi配置，包括eap,inner和它的第一个用户
    QSharedPointer<QSettings>  autoSettings = QSharedPointer<QSettings>(new QSettings(CONFIG_FILE, QSettings::IniFormat));
    autoSettings.get()->beginGroup(name);
    autoSettings.get()->setValue("eap", eap);
    autoSettings.get()->setValue("inner", inner);
    //以数组形式写入用户名
    autoSettings.get()->beginWriteArray(name);
    autoSettings.get()->setArrayIndex(0);
    autoSettings.get()->setValue("user", user);
    user_list << user;
    autoSettings.get()->endArray();
    return true;
}

bool WpaWifiDialog::appendWifiUser(QString name, QString user) {
    //向名为name的wifi用户列表添加名为user的用户名
    QSharedPointer<QSettings>  autoSettings = QSharedPointer<QSettings>(new QSettings(CONFIG_FILE, QSettings::IniFormat));
//    autoSettings.get()->beginWriteArray(name);
//    autoSettings.get()->setArrayIndex(0);
    //读到用户名列表长度并在队尾添加一个用户名
    autoSettings.get()->beginGroup(name);
    autoSettings.get()->setValue("eap", eapCombox->currentData().toString());
    autoSettings.get()->setValue("inner", innerCombox->currentData().toString());
    if (user_list.contains(user)) {
        qDebug()<<"已经有了";
        autoSettings.get()->endGroup();
        return true;
    }
    autoSettings.get()->beginReadArray(name);
    int size = 1;
    while (size) {
        autoSettings.get()->setArrayIndex(size);
        if (autoSettings.get()->value("user").toString().isEmpty()) {
            autoSettings.get()->setValue("user", user);
            user_list << user;
            break;
        }
        size ++;
    }
//    autoSettings.get()->beginWriteArray(name);
//    autoSettings.get()->setArrayIndex(size + 1);
//    autoSettings.get()->setValue("user", user);
    autoSettings.get()->endArray();
    autoSettings.get()->endGroup();
    return true;
}

void WpaWifiDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->isPress = true;
        this->winPos = this->pos();
        this->dragPos = event->globalPos();
        event->accept();
    }
    return QDialog::mousePressEvent(event);
}
void WpaWifiDialog::mouseReleaseEvent(QMouseEvent *event)
{
    this->isPress = false;
}
void WpaWifiDialog::mouseMoveEvent(QMouseEvent *event)
{
    if (this->isPress) {
        this->move(this->winPos - (this->dragPos - event->globalPos()));
        event->accept();
    }
    return QDialog::mouseMoveEvent(event);
}

void WpaWifiDialog::paintEvent(QPaintEvent *event)
{
    KylinDBus mkylindbus;
    double trans = mkylindbus.getTransparentData();

    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QRect rect = this->rect();
    p.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    p.setBrush(opt.palette.color(QPalette::Base));
    p.setOpacity(trans);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(rect, 6, 6);
    QWidget::paintEvent(event);
}
