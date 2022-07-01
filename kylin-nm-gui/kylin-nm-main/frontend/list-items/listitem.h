#ifndef LISTITEM_H
#define LISTITEM_H
#include <QFrame>
#include <QEvent>
#include <QHBoxLayout>
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>
#include "radioitembutton.h"
#include "infobutton.h"
#include "netdetails/netdetail.h"

class ListItem : public QFrame
{
    Q_OBJECT
public:
    ListItem(QWidget *parent = nullptr);
    ~ListItem();
    void setName(const QString &name);
    void setActive(const bool &isActive);
    void setConnectState(KyConnectState state);
    static void showDesktopNotify(const QString &message);

protected:
    void mousePressEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
    virtual void onRightButtonClicked() = 0;

protected:
    QFrame * m_itemFrame = nullptr;

    FixLabel * m_nameLabel = nullptr;
    RadioItemButton * m_netButton = nullptr;
    InfoButton * m_infoButton = nullptr;

    bool m_isActive = false;
    KyConnectState m_connectState;

    QMenu *m_menu = nullptr;
public:
    QVBoxLayout * m_mainLayout = nullptr;
    QHBoxLayout * m_hItemLayout = nullptr;
    bool isDetailShow;
private:
    void initUI();
    void initConnection();

public Q_SLOTS:
    virtual void onNetButtonClicked() = 0;
    void onPaletteChanged();
    void onDetailShow(bool isShow);
    virtual void onMenuTriggered(QAction *action)=0;

Q_SIGNALS:
    void detailShow(bool isShow); 
};

#endif // LISTITEM_H
