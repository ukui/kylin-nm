#ifndef NETBUTTON_H
#define NETBUTTON_H
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include<QTimer>
#define ACTIVATED true
#define INACTIVATED false

class RadioItemButton : public QPushButton
{
    Q_OBJECT
public:
    RadioItemButton(bool isActivated = INACTIVATED, QWidget * parent = nullptr);
    ~RadioItemButton() = default;
    void startLoading();
    void stopLoading();
    void setPressed();
    void setReleased();
    void setButtonIcon(const QIcon &icon);

signals:
    void requestStartLoading();
    void requestStopLoading();

private:
    void paintEvent(QPaintEvent *event);
    bool m_isActivated = INACTIVATED;
    QLabel * m_iconLabel = nullptr;
    QTimer * switchTimer = nullptr;

    int currentPage;
    int countCurrentTime;

private slots:
    void onLoadingStarted();
    void onLoadingStopped();
};

#endif // NETBUTTON_H
