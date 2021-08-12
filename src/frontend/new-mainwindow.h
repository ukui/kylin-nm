#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QGSettings/QGSettings>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QDBusInterface>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void showMainwindow();
    void hideMainwindow();

signals:

public slots:

protected:
    void keyPressEvent(QKeyEvent *);
    bool eventFilter(QObject *, QEvent *) override;

private:
    void firstlyStart(); //一级启动
    void secondaryStart(); //二级启动
    bool m_load_finished = false; //是否二级启动已执行完
    QTimer * m_secondary_start_timer = nullptr; //执行二级启动的倒计时
    void initWindowProperties();
    void initUI();
    void initTrayIcon();
    void resetTrayIconTool();
    void initWindowTheme();
    void resetWindowTheme();
    void showControlCenter();

    //主窗口的主要构成控件
    QTabWidget * m_central_widget = nullptr;

    //监听主题的Gsettings
    QGSettings * m_style_gsettings = nullptr;

    //获取和重置窗口位置
    void resetWindowPosition();
    QDBusInterface * m_position_interface = nullptr;

    //托盘图标，托盘图标右键菜单
    QSystemTrayIcon * m_tray_icon = nullptr;
    QMenu * m_tray_icon_menu = nullptr;
    QAction * m_show_mainwindow_action = nullptr;
    QAction * m_show_settings_action = nullptr;

private slots:
    void onTrayIconActivated();
    void onShowMainwindowActionTriggled();
    void onShowSettingsActionTriggled();
};

#endif // MAINWINDOW_H
