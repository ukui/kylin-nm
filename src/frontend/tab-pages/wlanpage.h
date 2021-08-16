#ifndef WLANPAGE_H
#define WLANPAGE_H

#include "divider.h"
#include "switchbutton.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QListWidget>

class WlanPage : public QWidget
{
    Q_OBJECT
public:
    explicit WlanPage(QWidget *parent = nullptr);
    ~WlanPage();

signals:

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void initUI();
    QVBoxLayout * m_mainLayout = nullptr;
    QFrame * m_titleFrame = nullptr;
    QHBoxLayout * m_titleLayout = nullptr;
    QLabel * m_titleLabel = nullptr;
    SwitchButton * m_lanSwitch = nullptr;
    Divider * m_titleDivider = nullptr;

    QFrame * m_activatedWlanFrame = nullptr;
    QVBoxLayout * m_activatedWlanLayout = nullptr;
    QLabel *m_activatedWlanLabel = nullptr;
    QListWidget * m_activatedWlanListWidget = nullptr;
    Divider * m_activatedWlanDivider = nullptr;

    QFrame * m_inactivatedWlanFrame = nullptr;
    QVBoxLayout * m_inactivatedWlanLayout = nullptr;
    QLabel * m_inactivatedWlanLabel = nullptr;
    QScrollArea * m_inactivatedWlanListArea = nullptr;
    QFrame * m_inactivatedWlanListAreaCentralWidget = nullptr;
    QVBoxLayout * m_inactivatedWlanListAreaLayout = nullptr;
    QListWidget * m_inactivatedWlanListWidget = nullptr;
    QFrame * m_hiddenWlanWidget = nullptr;
    QHBoxLayout * m_hiddenWlanLayout = nullptr;
    QLabel * m_hiddenWlanLabel = nullptr;
    Divider * m_inactivatedWlanDivider = nullptr;

    QFrame * m_settingsFrame = nullptr;
    QHBoxLayout * m_settingsLayout = nullptr;
    QLabel * m_settingsLabel = nullptr;
};

#endif // WLANPAGE_H
