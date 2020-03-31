#include "notifysend.h"
#include "ui_notifysend.h"

NotifySend::NotifySend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotifySend)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QPainterPath path;
    auto rect = this->rect();
    rect.adjust(1, 1, -1, -1);
    path.addRoundedRect(rect, 4, 4);
    setProperty("blurRegion", QRegion(path.toFillPolygon().toPolygon()));

    this->setStyleSheet("QWidget{border:none;border-radius:4px;}");
    ui->backwidget->setStyleSheet("QWidget{border:1px solid rgba(255, 255, 255, 0.05);border-radius:4px;background:rgba(19,19,20,0.7);}");
    ui->lbColor->setStyleSheet("QLabel{border:none;background:rgba(61,107,229,1);}");
    ui->leText->setStyleSheet("QLineEdit{border:none;background:transparent;font-size:14px;color:white;font-size:14px;}");
    ui->btnClose->setStyleSheet("QPushButton{background-image:url(:/res/g/close_white.png);background-color:transparent;border:none;}");
    ui->btnClose->setFocusPolicy(Qt::NoFocus);

    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
    int d = 10; //窗口上边沿到屏幕边沿距离
    int s = 10; //窗口右边沿到屏幕边沿距离

    this->move(availableGeometry.x() + availableGeometry.width() - this->width() - s, availableGeometry.y() + d);
}

NotifySend::~NotifySend()
{
    delete ui;
}

void  NotifySend::execNotifySend(QString text)
{
    ui->leText->setText(text);
    this->show();

    QTimer::singleShot(4000, this, SLOT(closeNotifyDialog() ));
}

void  NotifySend::closeNotifyDialog(){
    ui->leText->setText("");
    this->hide();
}

void NotifySend::on_btnClose_clicked()
{
    this->hide();
}
