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
    bool eventFilter(QObject *, QEvent *);

private:
    void initUI();
    QVBoxLayout * m_main_layout = nullptr;
    QFrame * m_title_frame = nullptr;
    QHBoxLayout * m_title_layout = nullptr;
    QLabel * m_title_label = nullptr;
    SwitchButton * m_lan_switch = nullptr;
    Divider * m_title_divider = nullptr;

    QFrame * m_activated_wlan_frame = nullptr;
    QVBoxLayout * m_activated_wlan_layout = nullptr;
    QLabel * m_activated_wlan_label = nullptr;
    QListWidget * m_activated_wlan_list_widget = nullptr;
    Divider * m_activated_wlan_divider = nullptr;

    QFrame * m_inactivated_wlan_frame = nullptr;
    QVBoxLayout * m_inactivated_wlan_layout = nullptr;
    QLabel * m_inactivated_wlan_label = nullptr;
    QScrollArea * m_inactivated_wlan_list_area = nullptr;
    QFrame * m_inactivated_wlan_list_area_central_widget = nullptr;
    QVBoxLayout * m_inactivated_wlan_list_area_layout = nullptr;
    QListWidget * m_inactivated_wlan_list_widget = nullptr;
    QFrame * m_hidden_wlan_widget = nullptr;
    QHBoxLayout * m_hidden_wlan_layout = nullptr;
    QLabel * m_hidden_wlan_label = nullptr;
    Divider * m_inactivated_wlan_divider = nullptr;

    QFrame * m_settings_frame = nullptr;
    QHBoxLayout * m_settings_layout = nullptr;
    QLabel * m_settings_label = nullptr;
};

#endif // WLANPAGE_H
