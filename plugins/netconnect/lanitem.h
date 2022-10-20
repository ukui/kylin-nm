#ifndef LANITEM_H
#define LANITEM_H
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QVariantMap>
#include <QTimer>
#include <QGSettings>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QToolButton>
#include <QMenu>
#include <QEvent>
#include "fixlabel.h"
#include "infobutton.h"

class LanItem : public QPushButton
{
    Q_OBJECT
public:
    LanItem(bool isAcitve, QWidget *parent = nullptr);
    ~LanItem();
public:
    QLabel * iconLabel = nullptr;
    InfoButton * infoLabel = nullptr;
    FixLabel * titileLabel = nullptr;
    QLabel * statusLabel = nullptr;
    QToolButton* m_moreButton = nullptr;
    QMenu* m_moreMenu = nullptr;
    QAction* m_connectAction = nullptr;
    QAction* m_deleteAction = nullptr;

public:
    void startLoading();
    void stopLoading();
    void setConnectActionText(bool isAcitve);

    bool loading = false;
    bool isAcitve = false;

    QString uuid;
    QString dbusPath;

protected:
    void paintEvent(QPaintEvent *);
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QTimer *waitTimer = nullptr;
    QGSettings *themeGsettings = nullptr;
    QList<QIcon> loadIcons;
    int currentIconIndex=0;

private slots:
    void updateIcon();
    void onConnectTriggered();
    void onDeletetTriggered();

Q_SIGNALS:
    void connectActionTriggered();
    void disconnectActionTriggered();
    void deleteActionTriggered();

};

#endif // LANITEM_H
