#ifndef KYLABLE_H
#define KYLABLE_H

#include <QObject>
#include <QLabel>

class KyLable : public QLabel
{
    Q_OBJECT
public:
    explicit KyLable(QWidget *parent = nullptr);
    ~KyLable() = default;

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QColor m_foregroundColor;

    void setPressColor();
    void setHoverColor();
    void setNormalColor();

private slots:
    void onPaletteChanged();

};

#endif // KYLABLE_H
