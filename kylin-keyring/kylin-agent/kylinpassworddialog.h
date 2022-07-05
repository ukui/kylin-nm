#ifndef KYLINPASSWORDDIALOG_H
#define KYLINPASSWORDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QCheckBox>
#include <QRegExpValidator>
#include <QTimer>
#include <QDBusInterface>

#include "kpasswordedit.h"

using namespace kdk;

class RowWidget : public QWidget
{
    Q_OBJECT
public:
    RowWidget(QWidget *widget, QWidget *parent = nullptr);
    ~RowWidget();
    void setKey(QString text);

private:
    QHBoxLayout *mainLayout;
    QLabel *keyLabel;
    QWidget *valueWidget;
};

class KyPasswordDialog : public QDialog
{
    Q_OBJECT
public:
    KyPasswordDialog(QString dialogType, QString connectionID = "", QWidget *parent = 0);
    ~KyPasswordDialog();

    void showDesktopNotify(const QString &message);

    void InitConnect();
private:
    QString dialogType;
    QString connectionID;

    QLineEdit *m_usrLineEdit = nullptr;
    QPushButton *m_connectBtn = nullptr;
    QPushButton *m_exitBtn = nullptr;

    KPasswordEdit *m_sdkPasswdEdit = nullptr;

    void onPaletteChanged();

protected:
    void paintEvent(QPaintEvent *event);

public:
    void initUI(QString dialogType);

    QString getPassword();
    void setPassword(QString password);

    QString getUserName();
    void setUserName(QString name);

private Q_SLOTS:
    void login();
    void onTextChanged(const QString &);

Q_SIGNALS:
    void signal_username(QString);
};

#endif // KYLINPASSWORDDIALOG_H
