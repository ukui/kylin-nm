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

class LanItem : public QFrame
{
    Q_OBJECT
public:
    LanItem(bool isSimple, QWidget *parent = nullptr);

    QLabel * iconLabel = nullptr;
    InfoButton * infoLabel = nullptr;
    QLabel * titileLabel = nullptr;

    void startLoading();
    void stopLoading();

    bool loading = false;
    bool isAcitve = false;
    bool isSimple;

    QString uuid;
    QString dbusPath;
Q_SIGNALS:
    void itemClick();

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

private Q_SLOTS:
    void updateIcon();
//    void onMenuTriggered(QAction *action);
};

#endif // LANITEM_H
