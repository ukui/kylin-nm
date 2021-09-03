#ifndef LISTITEM_H
#define LISTITEM_H
#include <QFrame>
#include <QEvent>
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
    void setActive(const bool &isActive);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

protected:
    QFrame * m_itemFrame = nullptr;

    QLabel * m_nameLabel = nullptr;
    RadioItemButton * m_netButton = nullptr;
    InfoButton * m_infoButton = nullptr;

    bool m_isActive = false;

public:
    QVBoxLayout * m_mainLayout = nullptr;
    QHBoxLayout * m_hItemLayout = nullptr;

private:
    void initUI();
    void initConnection();

protected slots:
    virtual void onInfoButtonClicked();
    virtual void onNetButtonClicked();
};

#endif // LISTITEM_H
