#ifndef NETBUTTON_H
#define NETBUTTON_H
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include<QTimer>

class RadioItemButton : public QPushButton
{
    Q_OBJECT
public:
    RadioItemButton(QWidget * parent = nullptr);
    ~RadioItemButton() = default;
    void startLoading();
    void stopLoading();
    void setButtonIcon(const QIcon &icon);
    void setActive(const bool &isActive);

signals:
    void requestStartLoading();
    void requestStopLoading();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool m_isActivated = false;
    QLabel * m_iconLabel = nullptr;
    QTimer * switchTimer = nullptr;
    QColor m_backgroundColor;

    int currentPage;
    int countCurrentTime;

private slots:
    void onLoadingStarted();
    void onLoadingStopped();
};

#endif // NETBUTTON_H
