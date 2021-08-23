#ifndef TABPAGE_H
#define TABPAGE_H

#include "divider.h"
#include "switchbutton.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QListWidget>

#define MAIN_LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_SPACING 0
#define TITLE_FRAME_HEIGHT 52
#define TITLE_LAYOUT_MARGINS 24,0,0,0
#define NET_LAYOUT_MARGINS 8,8,8,8
#define NET_LAYOUT_SPACING 8
#define NET_LIST_SPACING 2
#define TEXT_MARGINS 16,0,0,0
//#define SCROLL_AREA_HEIGHT 200
#define SETTINGS_LAYOUT_MARGINS 24,16,24,16

class TabPage : public QWidget
{
    Q_OBJECT
public:
    explicit TabPage(QWidget *parent = nullptr);
    ~TabPage();

signals:

protected:
    void initUI();
    QVBoxLayout * m_mainLayout = nullptr;
    QFrame * m_titleFrame = nullptr;
    QHBoxLayout * m_titleLayout = nullptr;
    QLabel * m_titleLabel = nullptr;
    SwitchButton * m_netSwitch = nullptr;
    Divider * m_titleDivider = nullptr;

    QFrame * m_activatedNetFrame = nullptr;
    QVBoxLayout * m_activatedNetLayout = nullptr;
    QLabel * m_activatedNetLabel = nullptr;
    QListWidget * m_activatedNetListWidget = nullptr;
    Divider * m_activatedNetDivider = nullptr;

    QFrame * m_inactivatedNetFrame = nullptr;
    QVBoxLayout * m_inactivatedNetLayout = nullptr;
    QLabel * m_inactivatedNetLabel = nullptr;
    QScrollArea * m_inactivatedNetListArea = nullptr;
    QListWidget * m_inactivatedNetListWidget = nullptr;
    Divider * m_inactivatedNetDivider = nullptr;

    QFrame * m_settingsFrame = nullptr;
    QHBoxLayout * m_settingsLayout = nullptr;
    QLabel * m_settingsLabel = nullptr;

};

#endif // TABPAGE_H
