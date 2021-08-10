#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QGSettings/QGSettings>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:

private:
    void initWindowProperties();
    void initUI();
    void initWindowTheme();
    void resetWindowTheme();

    QTabWidget * m_central_widget = nullptr;
    QGSettings * m_style_gsettings = nullptr;

private slots:
};

#endif // MAINWINDOW_H
