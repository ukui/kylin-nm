#ifndef NETBUTTON_H
#define NETBUTTON_H
#include <QPushButton>
#include <QIcon>
#include <QLabel>
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
    void setButtonIcon(QIcon *);

signals:
    void requestStartLoading();
    void requestStopLoading();

private:
    bool m_isActivated = INACTIVATED;
    QLabel * m_iconLabel = nullptr;

private slots:
    void onLoadingStarted();
    void onLoadingStopped();
};

#endif // NETBUTTON_H
