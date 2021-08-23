#ifndef WLANPAGE_H
#define WLANPAGE_H

#include "tabpage.h"

class WlanPage : public TabPage
{
    Q_OBJECT
public:
    explicit WlanPage(QWidget *parent = nullptr);
    ~WlanPage() = default;

signals:

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void initWlanUI();
    QFrame * m_inactivatedWlanListAreaCentralWidget = nullptr;
    QVBoxLayout * m_inactivatedWlanListAreaLayout = nullptr;
    QFrame * m_hiddenWlanWidget = nullptr;
    QHBoxLayout * m_hiddenWlanLayout = nullptr;
    QLabel * m_hiddenWlanLabel = nullptr;
};

#endif // WLANPAGE_H
