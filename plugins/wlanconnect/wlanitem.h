#ifndef WLANITEM_H
#define WLANITEM_H
#include <QObject>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QImage>
#include <QMouseEvent>
#include <QMenu>
#include <QCheckBox>

#include "../component/InfoButton/infobutton.h"
#include "kpasswordedit.h"
using namespace kdk;

class WlanItem : public QFrame
{
    Q_OBJECT
public:
    WlanItem(bool isSimple, QWidget *parent = nullptr);
public:
    void setName(QString name) {
        titileLabel->setText(name);
    }

    QString getName() {
        return titileLabel->text();
    }

    void setItemIcon(const QPixmap &pixmap) {
        iconLabel->setPixmap(pixmap);
    }

    void setStatus(bool isAcitve){
        m_isAcitve = isAcitve;
    }

    bool getStatus(){
        return m_isAcitve;
    }

    bool getIsLoading() {
        return m_loading;
    }

    QString getUuid(){
        return m_uuid;
    }

    void setUuid(QString uuid){
        m_uuid = uuid;
    }

    QString getPath(){
        return m_dbusPath;
    }

    void setPath(QString dbusPath){
        m_dbusPath = dbusPath;
    }

    QString getSecuType(){
        return m_secutype;
    }

    void setSecuType(QString &secuType){
        m_secutype = secuType;
    }

    QString getInterface(){
        return m_interface;
    }

    void setInterface(QString &interface){
        m_interface = interface;
    }


    void startLoading();
    void stopLoading();

    void setExpend(bool);
    bool getExpend();

Q_SIGNALS:
    void itemClick();
    void infoButtonClick();
    void connectButtonClick(QString , bool);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QTimer *waitTimer = nullptr;
    QList<QIcon> loadIcons;
    int currentIconIndex=0;

    QMenu *m_menu = nullptr;
    QLabel * iconLabel = nullptr;
    InfoButton * infoLabel = nullptr;
    QLabel * titileLabel = nullptr;

    bool isSimple;
    bool m_isAcitve = false;
    bool m_loading = false;
    bool m_isExpand = false;

    QString m_uuid = "";
    QString m_dbusPath ="";
    //ap类型
    QString m_secutype = "";
    QString m_interface = "";

    QFrame* m_nameFrame = nullptr;
    QFrame* m_expendFrame = nullptr;
    QFrame* m_pwdFrame = nullptr;
    QFrame* m_autoConnectFrame = nullptr;

    KPasswordEdit* m_pwdLineEdit = nullptr;
    QPushButton* m_connectButton = nullptr;
    QCheckBox* m_autoConnectCheckBox = nullptr;
    QLabel* m_autoConnectLabel = nullptr;

    QHBoxLayout* m_autoConnectFrameLyt = nullptr;
    QHBoxLayout* m_pwdFrameLyt = nullptr;

private Q_SLOTS:
    void updateIcon();
//    void onMenuTriggered(QAction *action);
    void onPwdEditorTextChanged();
    void onConnectButtonClicked();
};

#endif // WLANITEM_H
