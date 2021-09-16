#include "radioitembutton.h"
#include <QPainter>
#include <QSvgRenderer>
#include <QApplication>
#include <QStyle>
#include <QDebug>
#define FLASH_SPEED 100
#define TIMEOUT_TIMER 90*1000
#define BUTTON_SIZE 36,36
#define ICON_SIZE 16,16
#define BACKGROUND_COLOR QColor(0,0,0,0)
#define FOREGROUND_COLOR_NORMAL_INACTIVE_LIGHT QColor(230,230,230,255)
#define FOREGROUND_COLOR_NORMAL_INACTIVE_DARK QColor(55,55,55,255)
#define FOREGROUND_COLOR_PRESS_INACTIVE_LIGHT QColor(217,217,217,255)
#define FOREGROUND_COLOR_PRESS_INACTIVE_DARK QColor(70,70,70,255)
#define FOREGROUND_COLOR_NORMAL_ACTIVE QColor(55,144,250,255)
#define FOREGROUND_COLOR_PRESS_ACTIVE QColor(36,109,212,255)

RadioItemButton::RadioItemButton(QWidget *parent) : QPushButton(parent)
{
    this->setAutoFillBackground(false);
    m_iconLabel = new QLabel(this);

    this->setFixedSize(BUTTON_SIZE);
    m_iconLabel->setFixedSize(BUTTON_SIZE);
    m_iconLabel->setAlignment(Qt::AlignCenter);


    setActive(false);
    //JXJ_TODO loading动画
    connect(this, &RadioItemButton::requestStartLoading, this, &RadioItemButton::onLoadingStarted);
    connect(this , &RadioItemButton::requestStopLoading, this, &RadioItemButton::onLoadingStopped);
    connect(qApp, &QApplication::paletteChanged, this, &RadioItemButton::onPaletteChanged);
}

RadioItemButton::~RadioItemButton()
{

}

void RadioItemButton::startLoading()
{
    emit this->requestStartLoading();
}

void RadioItemButton::stopLoading()
{
    emit this->requestStopLoading();
}

void RadioItemButton::setButtonIcon(const QIcon &icon)
{
    if (icon.isNull()) {
        return;
    }
    m_pixmap = icon.pixmap(ICON_SIZE);
    m_iconLabel->setPixmap(m_pixmap);
}

void RadioItemButton::setActive(const bool &isActive)
{
    m_isActivated = isActive;
//    m_backgroundColor = m_isActivated? FOREGROUND_COLOR_NORMAL_ACTIVE : FOREGROUND_COLOR_NORMAL_INACTIVE;
    if (m_isActivated) {
        m_backgroundColor = FOREGROUND_COLOR_NORMAL_ACTIVE;
//        m_iconLabel->setProperty("useIconHighlightEffect", 0x08);
        m_iconLabel->setPixmap(loadSvg(m_pixmap, PixmapColor::WHITE));
    } else {
        if (qApp->palette().base().color().red() > MIDDLE_COLOR) {
            m_backgroundColor = FOREGROUND_COLOR_NORMAL_INACTIVE_LIGHT;
            m_iconLabel->setPixmap(m_pixmap);
        } else {
            m_backgroundColor = FOREGROUND_COLOR_NORMAL_INACTIVE_DARK;
            m_iconLabel->setPixmap(loadSvg(m_pixmap, PixmapColor::WHITE));
        }
    }
}
void RadioItemButton::onLoadingStarted()
{
#define ANIMATION_SPEED 0.5*1000
#define START_ROTATION 0
#define END_ROTATION 360
#define ANIMATION_LOOP -1 //无限旋转
    if (!m_iconLabel) {
        qWarning() << "Start loading failed, iconLabel is null pointer!" << Q_FUNC_INFO << __LINE__;
    }
    if (!m_animation) {
        m_animation = new QVariantAnimation(m_iconLabel);
    }
    m_animation->setDuration(ANIMATION_SPEED);
    m_animation->setStartValue(START_ROTATION);
    m_animation->setEndValue(END_ROTATION);
    m_animation->setLoopCount(ANIMATION_LOOP);
    connect(m_animation, &QVariantAnimation::valueChanged, this, &RadioItemButton::onAnimationValueChanged);
    m_animation->start();
}

void RadioItemButton::onLoadingStopped()
{
    if (!m_animation) {
        qWarning() << "Stop loading failed, m_animation is null pointer!" << Q_FUNC_INFO << __LINE__;
        return;
    } else {
        m_animation->stop();
    }

    if (!m_iconLabel) {
        qWarning() << "Stop loading failed, iconLabel is null pointer!" << Q_FUNC_INFO << __LINE__;
        return;
    } else {
        m_iconLabel->setPixmap(m_pixmap);
    }
    qDebug() << "Stop loading succeed!" << Q_FUNC_INFO << __LINE__;
}

void RadioItemButton::onPaletteChanged()
{
    if (qApp->palette().base().color().red() > MIDDLE_COLOR) {
        m_backgroundColor = FOREGROUND_COLOR_NORMAL_INACTIVE_LIGHT;
        m_iconLabel->setPixmap(m_pixmap);
    } else {
        m_backgroundColor = FOREGROUND_COLOR_NORMAL_INACTIVE_DARK;
        m_iconLabel->setPixmap(loadSvg(m_pixmap, PixmapColor::WHITE));
    }
}

void RadioItemButton::onAnimationValueChanged(const QVariant& value)
{
    if (!m_iconLabel) {
        return;
    }
    QTransform t;
    t.rotate(value.toReal());
    m_iconLabel->setPixmap(QIcon(":/res/s/conning-a/1.png").pixmap(ICON_SIZE).transformed(t));
}

void RadioItemButton::paintEvent(QPaintEvent *event)
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::Base, BACKGROUND_COLOR);
    pal.setColor(QPalette::Text, m_backgroundColor);

    QPainterPath cPath;
    cPath.addRect(0, 0, this->width(), this->height());
    cPath.addEllipse(0, 0, this->width(), this->width());

    QPainterPath innerPath;
    innerPath.addEllipse(0, 0, this->width(), this->width());

    QPainter painter(this);
    painter.setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
    painter.setPen(Qt::NoPen);

    painter.setBrush(pal.color(QPalette::Base));
    painter.drawPath(cPath);

    painter.fillPath(innerPath, pal.color(QPalette::Text));
}

void RadioItemButton::mousePressEvent(QMouseEvent *event)
{
    m_backgroundColor = m_isActivated? FOREGROUND_COLOR_PRESS_ACTIVE :
                                       (qApp->palette().base().color().red() > MIDDLE_COLOR ?
                                            FOREGROUND_COLOR_PRESS_INACTIVE_LIGHT :
                                            FOREGROUND_COLOR_PRESS_INACTIVE_DARK);
    this->repaint();
    return QPushButton::mousePressEvent(event);
}

void RadioItemButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_backgroundColor = m_isActivated? FOREGROUND_COLOR_NORMAL_ACTIVE :
                                       (qApp->palette().base().color().red() > MIDDLE_COLOR ?
                                            FOREGROUND_COLOR_NORMAL_INACTIVE_LIGHT :
                                            FOREGROUND_COLOR_NORMAL_INACTIVE_DARK);
    this->repaint();
    return QPushButton::mouseReleaseEvent(event);
}

const QPixmap RadioItemButton::loadSvg(const QPixmap &source, const PixmapColor &cgColor)
{
    QImage img = source.toImage();
    for (int x = 0; x < img.width(); x++) {
        for (int y = 0; y < img.height(); y++) {
            auto color = img.pixelColor(x, y);
            if (color.alpha() > 0) {
                switch (cgColor) {
                case PixmapColor::WHITE:
                    color.setRed(255);
                    color.setGreen(255);
                    color.setBlue(255);
                    img.setPixelColor(x, y, color);
                    break;
                case PixmapColor::BLACK:
                    color.setRed(0);
                    color.setGreen(0);
                    color.setBlue(0);
                    img.setPixelColor(x, y, color);
                    break;
                case PixmapColor::GRAY:
                    color.setRed(152);
                    color.setGreen(163);
                    color.setBlue(164);
                    img.setPixelColor(x, y, color);
                    break;
                case PixmapColor::BLUE:
                    color.setRed(61);
                    color.setGreen(107);
                    color.setBlue(229);
                    img.setPixelColor(x, y, color);
                    break;
                default:
                    return source;
                    break;
                }
            }
        }
    }
    return QPixmap::fromImage(img);
}
