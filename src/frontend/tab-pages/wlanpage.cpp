#include "wlanpage.h"
#include <QEvent>

#define MAIN_LAYOUT_MARGINS 0,0,0,0
#define MAIN_LAYOUT_SPACING 0
#define TITLE_FRAME_HEIGHT 52
#define TITLE_LAYOUT_MARGINS 24,0,0,0
#define LAN_LAYOUT_MARGINS 8,8,8,8
#define LAN_LAYOUT_SPACING 8
#define LAN_LIST_SPACING 2
#define TEXT_MARGINS 16,0,0,0
//#define SCROLL_AREA_HEIGHT 200
#define SETTINGS_LAYOUT_MARGINS 24,16,24,16

WlanPage::WlanPage(QWidget *parent) : QWidget(parent)
{
    initUI();
}

WlanPage::~WlanPage()
{
    delete m_title_divider;
    delete m_activated_wlan_divider;
    delete m_inactivated_wlan_divider;
}

bool WlanPage::eventFilter(QObject *w, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress) {
        if (w == m_hidden_wlan_label) {
            //ZJP_TODO 打开隐藏WiFi添加弹窗
        } else if (w == m_settings_label) {
            //ZJP_TODO 打开控制面板
        }
    }
    return QWidget::eventFilter(w,e);
}

void WlanPage::initUI()
{
    m_main_layout = new QVBoxLayout(this);
    m_main_layout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_main_layout->setSpacing(MAIN_LAYOUT_SPACING);
    this->setLayout(m_main_layout);

    m_title_frame = new QFrame(this);
    m_title_frame->setFixedHeight(TITLE_FRAME_HEIGHT);
    m_title_layout = new QHBoxLayout(m_title_frame);
    m_title_layout->setContentsMargins(TITLE_LAYOUT_MARGINS);
    m_title_label = new QLabel(m_title_frame);
    m_title_label->setText(tr("WLAN"));
    m_lan_switch = new SwitchButton(m_title_frame);
    m_title_layout->addWidget(m_title_label);
    m_title_layout->addStretch();
    m_title_layout->addWidget(m_lan_switch);
    m_title_divider = new Divider(this);

    m_activated_wlan_frame = new QFrame(this);
    m_activated_wlan_layout = new QVBoxLayout(m_activated_wlan_frame);
    m_activated_wlan_layout->setContentsMargins(LAN_LAYOUT_MARGINS);
    m_activated_wlan_layout->setSpacing(LAN_LAYOUT_SPACING);
    m_activated_wlan_label = new QLabel(m_activated_wlan_frame);
    m_activated_wlan_label->setContentsMargins(TEXT_MARGINS);
    m_activated_wlan_label->setText(tr("Activated WLAN"));
    m_activated_wlan_list_widget = new QListWidget(m_activated_wlan_frame);
    m_activated_wlan_list_widget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_activated_wlan_list_widget->setSpacing(LAN_LIST_SPACING);
    m_activated_wlan_layout->addWidget(m_activated_wlan_label);
    m_activated_wlan_layout->addWidget(m_activated_wlan_list_widget);
    m_activated_wlan_divider = new Divider(this);

    m_inactivated_wlan_frame = new QFrame(this);
    m_inactivated_wlan_layout = new QVBoxLayout(m_inactivated_wlan_frame);
    m_inactivated_wlan_layout->setContentsMargins(LAN_LAYOUT_MARGINS);
    m_inactivated_wlan_layout->setSpacing(LAN_LAYOUT_SPACING);
    m_inactivated_wlan_frame->setLayout(m_inactivated_wlan_layout);
    m_inactivated_wlan_label = new QLabel(m_inactivated_wlan_frame);
    m_inactivated_wlan_label->setContentsMargins(TEXT_MARGINS);
    m_inactivated_wlan_label->setText(tr("Inactivated WLAN"));
    m_inactivated_wlan_list_area = new QScrollArea(m_inactivated_wlan_frame);
    m_inactivated_wlan_list_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_inactivated_wlan_list_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_inactivated_wlan_list_area_central_widget = new QFrame(m_inactivated_wlan_list_area);
    m_inactivated_wlan_list_area->setWidget(m_inactivated_wlan_list_area_central_widget);
    m_inactivated_wlan_list_area_layout = new QVBoxLayout(m_inactivated_wlan_list_area_central_widget);
    m_inactivated_wlan_list_area_central_widget->setLayout(m_inactivated_wlan_list_area_layout);
    m_inactivated_wlan_list_area_layout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivated_wlan_list_widget = new QListWidget(m_inactivated_wlan_list_area_central_widget);
    m_inactivated_wlan_list_widget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivated_wlan_list_widget->setSpacing(LAN_LIST_SPACING);
    m_hidden_wlan_widget = new QFrame(m_inactivated_wlan_list_area_central_widget);
    m_hidden_wlan_layout = new QHBoxLayout(m_hidden_wlan_widget);
    m_hidden_wlan_layout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_hidden_wlan_label = new QLabel(m_hidden_wlan_widget);
    m_hidden_wlan_label->setText(tr("More..."));
    m_hidden_wlan_layout->addWidget(m_hidden_wlan_label);
    m_hidden_wlan_layout->addStretch();
    m_inactivated_wlan_list_area_layout->addWidget(m_inactivated_wlan_list_widget);
    m_inactivated_wlan_list_area_layout->addWidget(m_hidden_wlan_widget);

//    m_inactivated_lan_list_area->setFixedHeight(SCROLL_AREA_HEIGHT);
    m_inactivated_wlan_layout->addWidget(m_inactivated_wlan_label);
    m_inactivated_wlan_layout->addWidget(m_inactivated_wlan_list_area);
    m_inactivated_wlan_divider = new Divider(this);

    m_settings_frame = new QFrame(this);
    m_settings_layout = new QHBoxLayout(m_settings_frame);
    m_settings_layout->setContentsMargins(SETTINGS_LAYOUT_MARGINS);
    m_settings_label = new QLabel(m_settings_frame);
    m_settings_label->setText(tr("Settings"));
    m_settings_layout->addWidget(m_settings_label);
    m_settings_frame->setLayout(m_settings_layout);

    m_main_layout->addWidget(m_title_frame);
    m_main_layout->addWidget(m_title_divider);
    m_main_layout->addWidget(m_activated_wlan_frame);
    m_main_layout->addWidget(m_activated_wlan_divider);
    m_main_layout->addWidget(m_inactivated_wlan_frame);
    m_main_layout->addWidget(m_inactivated_wlan_divider);
    m_main_layout->addWidget(m_settings_frame);
}
