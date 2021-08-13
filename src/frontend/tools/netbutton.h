#ifndef NETBUTTON_H
#define NETBUTTON_H
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#define ACTIVATED true
#define INACTIVATED false

class NetButton : public QPushButton
{
    Q_OBJECT
public:
    NetButton(bool isActivated = INACTIVATED, QWidget * parent = nullptr);
    ~NetButton() = default;
    void startLoading();
    void stopLoading();
    void setPressed();
    void setReleased();
    void setButtonIcon(QIcon *);

signals:
    void requestStartLoading();
    void requestStopLoading();

private:
    bool m_is_activated = INACTIVATED;
    QLabel * m_icon_label = nullptr;

private slots:
    void onLoadingStarted();
    void onLoadingStopped();
};

#endif // NETBUTTON_H
