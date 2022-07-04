#ifndef CONNECTDEVLISTITEM_H
#define CONNECTDEVLISTITEM_H
#include <QFrame>
#include <QEvent>
#include <QHBoxLayout>
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>
#include <QLabel>
#include "kborderlessbutton.h"

using namespace kdk;

class ConnectDevListItem : public QFrame
{
    Q_OBJECT
public:
    ConnectDevListItem(QString staMac, QString staName, QWidget *parent = nullptr);
    ~ConnectDevListItem();

protected:
    QFrame * m_itemFrame = nullptr;

    QLabel * m_nameLabel = nullptr;
    KBorderlessButton *m_dragIntoBlackListBtn = nullptr;

    QString m_mac;
    QString m_hostName;
    bool eventFilter(QObject *w, QEvent *e);

signals:
    void onBtnClicked(QString staMac);


};

#endif // LISTITEM_H
