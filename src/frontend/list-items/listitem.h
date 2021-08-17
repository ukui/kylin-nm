#ifndef LISTITEM_H
#define LISTITEM_H
#include <QFrame>
#include <QHBoxLayout>
#include "netbutton.h"
#include "infobutton.h"

class ListItem : public QFrame
{
    Q_OBJECT
public:
    ListItem(QWidget *parent = nullptr);
    ~ListItem();
    void setName(const QString &name);

protected:
    QVBoxLayout * m_mainLayout = nullptr;
    QFrame * m_itemFrame = nullptr;
    QHBoxLayout * m_hItemLayout = nullptr;
    NetButton * m_netButton = nullptr;
    QLabel * m_nameLabel = nullptr;
    InfoButton * m_infoButton = nullptr;

private:
    void initUI();
};

#endif // LISTITEM_H
