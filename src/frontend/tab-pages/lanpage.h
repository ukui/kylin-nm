#ifndef LANPAGE_H
#define LANPAGE_H

#include "divider.h"
#include "switchbutton.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QListWidget>

class LanPage : public QWidget
{
    Q_OBJECT
public:
    explicit LanPage(QWidget *parent = nullptr);
    ~LanPage();

signals:

private:
    void initUI();
    QVBoxLayout * m_mainLayout = nullptr;
    QFrame * m_titleFrame = nullptr;
    QHBoxLayout * m_titleLayout = nullptr;
    QLabel * m_titleLabel = nullptr;
    SwitchButton * m_lanSwitch = nullptr;
    Divider * m_titleDivider = nullptr;

    QFrame * m_activatedLanFrame = nullptr;
    QVBoxLayout * m_activatedLanLayout = nullptr;
    QLabel * m_activatedLanLabel = nullptr;
    QListWidget * m_activatedLanListWidget = nullptr;
    Divider * m_activatedLanDivider = nullptr;

    QFrame * m_inactivatedLanFrame = nullptr;
    QVBoxLayout * m_inactivatedLanLayout = nullptr;
    QLabel * m_inactivatedLanLabel = nullptr;
    QScrollArea * m_inactivatedLanListArea = nullptr;
    QListWidget * m_inactivatedLanListWidget = nullptr;
    Divider * m_inactivatedLanDivider = nullptr;

    QFrame * m_settingsFrame = nullptr;
    QHBoxLayout * m_settingsLayout = nullptr;
    QLabel * m_settingsLabel = nullptr;
};

#endif // LANPAGE_H
