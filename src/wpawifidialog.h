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

namespace Ui {
class WpaWifiDialog;
}

class UpConnThread : public QThread
{
    Q_OBJECT

public:
    explicit UpConnThread();
    ~UpConnThread();

public:
    void run();
    QString conn_name = 0;

Q_SIGNALS:
    void connRes(int respond);
};

class WpaWifiDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WpaWifiDialog(QWidget *parent = nullptr, QString conname = 0);
    ~WpaWifiDialog();

private:
    Ui::WpaWifiDialog *ui;
    void initUI(); //初始化UI界面
    void initCombox(); //初始化所有下拉框
    void initConnect(); //初始化连接

protected:
    void paintEvent(QPaintEvent *event);

private:
    QString connectionName;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    bool isPress;
    QPoint winPos;
    QPoint dragPos;

    QString configPath = 0; //配置文件路径

    void setEditorEnable(bool is_checking); //设置是否禁用输入
    void activateConnection();

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
};

#endif // WPAWIFIDIALOG_H
