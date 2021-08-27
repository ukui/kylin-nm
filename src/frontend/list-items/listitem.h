#ifndef LISTITEM_H
#define LISTITEM_H
#include <QFrame>
#include <QHBoxLayout>
#include "radioitembutton.h"
#include "infobutton.h"

class ListItem : public QFrame
{
    Q_OBJECT
public:
    ListItem(QWidget *parent = nullptr);
    ~ListItem();
    void setName(const QString &name);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

protected:
    QVBoxLayout * m_mainLayout = nullptr;
    QFrame * m_itemFrame = nullptr;
    QHBoxLayout * m_hItemLayout = nullptr;
    RadioItemButton * m_netButton = nullptr;
    QLabel * m_nameLabel = nullptr;
    InfoButton * m_infoButton = nullptr;

private:
    void initUI();
    void initConnection();

protected slots:
    virtual void onInfoButtonClicked();
    virtual void onNetButtonClicked();
//    virtual void onNameLableClicked();
};

#endif // LISTITEM_H
