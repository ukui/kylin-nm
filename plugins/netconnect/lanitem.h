#ifndef LANITEM_H
#define LANITEM_H
#include <QObject>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>


#include "../component/InfoButton/infobutton.h"
#include "../component/RadioItemButton/radioitembutton.h"

class LanItem : public QFrame
{
    Q_OBJECT
public:
    LanItem(bool isSimple, QWidget *parent = nullptr);

    void setName(QString name) {
        titileLabel->setText(name);
    }

    QString getName() {
        return titileLabel->text();
    }

    void setItemIcon(const QIcon &icon) {
        radioBtn->setButtonIcon(icon);
    }

    void setStatus(bool isAcitve){
        m_isAcitve = isAcitve;
        radioBtn->setActive(isAcitve);
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

    void startLoading();
    void stopLoading();


Q_SIGNALS:
    void itemClick();
    void infoButtonClick();

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

    RadioItemButton* radioBtn = nullptr;
    InfoButton * infoLabel = nullptr;
    QLabel * titileLabel = nullptr;

    bool m_loading = false;
    bool m_isAcitve = false;
    bool isSimple;

    QString m_uuid;
    QString m_dbusPath;

    bool m_isIn = false;

private Q_SLOTS:
    void updateIcon();
//    void onMenuTriggered(QAction *action);
};

#endif // LANITEM_H
