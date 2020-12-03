#include "wpawifidialog.h"
#include "ui_wpawifidialog.h"
#include "kylin-network-interface.h"
#include "backthread.h"
#include "utils.h"
#include <KWindowEffects>

WpaWifiDialog::WpaWifiDialog(QWidget *parent, QString conname) :
    QDialog(parent),
    ui(new Ui::WpaWifiDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowIcon(QIcon::fromTheme("kylin-network", QIcon(":/res/x/setup.png")) );
//    this->setAttribute(Qt::WA_DeleteOnClose);

    connection_name = conname;

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
    nameEditor->setText(connection_name);
    nameEditor->setEnabled(false);//不允许编辑wifi名
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
    eapStringList<< "PEAP" << "TLS" << "TTLS" << "PWD" << "SIM" << "AKA" << "AKA'";
    for (int i = 0; i < eapStringList.length(); i++) {
        eapCombox->addItem(eapStringList.at(i), eapStringList.at(i));
    }
    //阶段2认证方式
    QStringList innerStringList;
    innerStringList<< tr("None") << "MSCHAPv2" << "GTC" << "SIM" << "AKA" << "AKA'";
    innerCombox->addItem(innerStringList.at(0), "None");
    for (int i = 1; i < innerStringList.length(); i++) {
        innerCombox->addItem(innerStringList.at(i), innerStringList.at(i));
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
}

void WpaWifiDialog::slot_on_connectBtn_clicked() {
    qDebug()<<"Clicked on connect Btn!";
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
