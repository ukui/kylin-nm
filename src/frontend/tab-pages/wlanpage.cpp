#include "wlanpage.h"
#include <QEvent>

WlanPage::WlanPage(QWidget *parent) : TabPage(parent)
{
    initWlanUI();
}

bool WlanPage::eventFilter(QObject *w, QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress) {
        if (w == m_hiddenWlanLabel) {
            //ZJP_TODO 打开隐藏WiFi添加弹窗
        } else if (w == m_settingsLabel) {
            //ZJP_TODO 打开控制面板
        }
    }
    return QWidget::eventFilter(w,e);
}

void WlanPage::initWlanUI()
{
    m_titleLabel->setText(tr("WLAN"));
    m_activatedNetLabel->setText(tr("Activated WLAN"));
    m_inactivatedNetLabel->setText(tr("Inactivated WLAN"));

    //一些独有控件
    m_inactivatedWlanListAreaCentralWidget = new QFrame(m_inactivatedNetListArea);
    m_inactivatedNetListArea->setWidget(m_inactivatedWlanListAreaCentralWidget);
    m_inactivatedWlanListAreaLayout = new QVBoxLayout(m_inactivatedWlanListAreaCentralWidget);
    m_inactivatedWlanListAreaCentralWidget->setLayout(m_inactivatedWlanListAreaLayout);
    m_inactivatedWlanListAreaLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivatedNetListWidget = new QListWidget(m_inactivatedWlanListAreaCentralWidget);
    m_inactivatedNetListWidget->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_inactivatedNetListWidget->setSpacing(NET_LIST_SPACING);
    m_hiddenWlanWidget = new QFrame(m_inactivatedWlanListAreaCentralWidget);
    m_hiddenWlanLayout = new QHBoxLayout(m_hiddenWlanWidget);
    m_hiddenWlanLayout->setContentsMargins(MAIN_LAYOUT_MARGINS);
    m_hiddenWlanLabel = new QLabel(m_hiddenWlanWidget);
    m_hiddenWlanLabel->setText(tr("More..."));
    m_hiddenWlanLayout->addWidget(m_hiddenWlanLabel);
    m_hiddenWlanLayout->addStretch();
}
