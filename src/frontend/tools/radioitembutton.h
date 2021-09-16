#ifndef NETBUTTON_H
#define NETBUTTON_H
#include <QPushButton>
#include <QIcon>
#include <QLabel>
#include<QTimer>

#define MIDDLE_COLOR 178

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
    enum PixmapColor {
        WHITE = 0,
        BLACK,
        GRAY,
        BLUE,
    };
    const QPixmap loadSvg(const QPixmap &source, const PixmapColor &color);

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
    QTimer * m_loadingTimer = nullptr;
    QTimer * m_timeoutTimer = nullptr;
    QColor m_backgroundColor;

    int currentPage = 8;
    QPixmap m_pixmap;

private slots:
    void onLoadingStarted();
    void onLoadingStopped();
    void onLoadingTimerTimeout();
    void onPaletteChanged();
};

#endif // NETBUTTON_H

