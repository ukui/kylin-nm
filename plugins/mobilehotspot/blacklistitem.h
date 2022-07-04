#ifndef BLACKLISTITEM_H
#define BLACKLISTITEM_H
#include <QFrame>
#include <QEvent>
#include <QHBoxLayout>
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>
#include <QLabel>
#include "kborderlessbutton.h"

using namespace kdk;

class BlacklistItem : public QFrame
{
    Q_OBJECT
public:
    BlacklistItem(QString staMac, QString staName, QWidget *parent = nullptr);
    ~BlacklistItem();

protected:
    KBorderlessButton *m_removeFromBlacklistBtn = nullptr;

    QString m_mac;
    bool eventFilter(QObject *w, QEvent *e);

signals:
    void onBtnClicked(QString blacklistMac);


};

#endif // LISTITEM_H
