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
    QLabel * m_nameLabel = nullptr;
    QFrame * m_itemFrame = nullptr;
    NetButton * m_netButton = nullptr;

private:
    QVBoxLayout * m_mainLayout = nullptr;
    QHBoxLayout * m_hItemLayout = nullptr;
    InfoButton * m_infoButton = nullptr;

private:
    void initUI();
};

#endif // LISTITEM_H
