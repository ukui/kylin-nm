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
    QVBoxLayout * m_main_layout = nullptr;
    QFrame * m_title_frame = nullptr;
    QHBoxLayout * m_title_layout = nullptr;
    QLabel * m_title_label = nullptr;
    SwitchButton * m_lan_switch = nullptr;
    Divider * m_title_divider = nullptr;

    QFrame * m_activated_lan_frame = nullptr;
    QVBoxLayout * m_activated_lan_layout = nullptr;
    QLabel * m_activated_lan_label = nullptr;
    QListWidget * m_activated_lan_list_widget = nullptr;
    Divider * m_activated_lan_divider = nullptr;

    QFrame * m_inactivated_lan_frame = nullptr;
    QVBoxLayout * m_inactivated_lan_layout = nullptr;
    QLabel * m_inactivated_lan_label = nullptr;
    QScrollArea * m_inactivated_lan_list_area = nullptr;
    QListWidget * m_inactivated_lan_list_widget = nullptr;
    Divider * m_inactivated_lan_divider = nullptr;

    QFrame * m_settings_frame = nullptr;
    QHBoxLayout * m_settings_layout = nullptr;
    QLabel * m_settings_label = nullptr;
};

#endif // LANPAGE_H
