#include "kylinpassworddialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

KyPasswordDialog::KyPasswordDialog(QWidget *parent):QDialog(parent)
{
    usrLabel = new QLabel(this);
    usrLabel->move(70,80);
    usrLabel->setText("用户名");
    usrLineEdit = new QLineEdit(this);
    usrLineEdit->move(140,80);
    usrLineEdit->setPlaceholderText("请输入用户名");

    pwdLabel = new QLabel(this);
    pwdLabel->move(70,130);
    pwdLabel->setText("密码");
    pwdLineEdit = new QLineEdit(this);
    pwdLineEdit->move(140,130);
    pwdLineEdit->setPlaceholderText("请输入密码");

    LoginBtn = new QPushButton(this);
    LoginBtn->move(70,200);
    LoginBtn->setText("登录");
    exitBtn = new QPushButton(this);
    exitBtn->move(220,200);
    exitBtn->setText("退出");

    connect(LoginBtn,SIGNAL(clicked(bool)),this,SLOT(login()));
    connect(exitBtn,SIGNAL(clicked(bool)),this,SLOT(close()));
}

KyPasswordDialog::~KyPasswordDialog(){

}

QString KyPasswordDialog::getPassword()
{
    return pwdLineEdit->text();
}

void KyPasswordDialog::setPassword(QString password)
{
    pwdLineEdit->setText(password);

}

QString KyPasswordDialog::getUserName()
{
    return usrLineEdit->text();
}

void KyPasswordDialog::setUserName(QString name)
{
    usrLineEdit->setText(name);

    return;
}

void KyPasswordDialog::login(){
    if(!pwdLineEdit->text().isEmpty()){
        accept();
    }else{
        QMessageBox::warning(this,"警告","密码或者用户名错误！",QMessageBox::Yes);
        usrLineEdit->clear();
        pwdLineEdit->clear();
        usrLineEdit->setFocus();
    }
}

void KyPasswordDialog::close(){
    this->close();
}
