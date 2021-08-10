#include "new-mainwindow.h"
#include "customstyle.h"
#include "lanpage.h"
#include "wlanpage.h"
#include <KWindowEffects>
#include <QApplication>
#include <QDebug>

#define MAINWINDOW_WIDTH 420
#define MAINWINDOW_HEIGHT 456
#define THEME_SCHAME "org.ukui.style"
#define COLOR_THEME "styleName"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    initWindowProperties();
    initUI();
    initWindowTheme();
}

/**
 * @brief MainWindow::initWindowProperties 初始化一些窗口属性
 */
void MainWindow::initWindowProperties()
{
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    //绘制毛玻璃特效
    this->setFixedSize(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
    QPainterPath path;
    auto rect = this->rect();
    path.addRoundedRect(rect, 6, 6);
    KWindowEffects::enableBlurBehind(this->winId(), true, QRegion(path.toFillPolygon().toPolygon()));
}

/**
 * @brief MainWindow::initUI 初始化窗口内控件
 */
void MainWindow::initUI()
{
    m_central_widget = new QTabWidget(this);
    this->setCentralWidget(m_central_widget);
    m_central_widget->tabBar()->setFixedWidth(this->width());
    LanPage * m_lanWidget = new LanPage(m_central_widget);
    WlanPage * m_wlanWidget = new WlanPage(m_central_widget);
    m_central_widget->addTab(m_lanWidget, QIcon::fromTheme("kylin-nm"), "1");
    m_central_widget->addTab(m_wlanWidget, QIcon::fromTheme("kylin-nm"), "2");
}

/**
 * @brief MainWindow::initWindowTheme 初始化窗口主题并创建信号槽
 */
void MainWindow::initWindowTheme()
{
    const QByteArray style_id(THEME_SCHAME);
    if (QGSettings::isSchemaInstalled(style_id)) {
        m_style_gsettings = new QGSettings(style_id);
        resetWindowTheme();
        connect(m_style_gsettings, &QGSettings::changed, this, [ = ](const QString &key) {
            if (key == COLOR_THEME) {
                qDebug() << "Received signal of theme changed, will reset theme." << Q_FUNC_INFO << __LINE__;
                resetWindowTheme();
            }
        });
    } else {
        qWarning() << "Gsettings interface \"org.ukui.style\" is not exist!" << Q_FUNC_INFO << __LINE__;
    }
}

/**
 * @brief MainWindow::resetWindowTheme 读取和设置窗口主题
 */
void MainWindow::resetWindowTheme()
{
    if (!m_style_gsettings) return;
    QString currentTheme = m_style_gsettings->get(COLOR_THEME).toString();
    auto app = static_cast<QApplication*>(QCoreApplication::instance());
    if(currentTheme == "ukui-dark" || currentTheme == "ukui-black"){
        app->setStyle(new CustomStyle("ukui-dark"));
        qDebug() << "Has set color theme to ukui-dark." << Q_FUNC_INFO << __LINE__;
        return;
    }
    app->setStyle(new CustomStyle("ukui-light"));
    qDebug() << "Has set color theme to ukui-light." << Q_FUNC_INFO << __LINE__;
    return;
}
