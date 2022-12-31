#ifndef FIXLABEL_H
#define FIXLABEL_H

#include <QWidget>
#include <QLabel>

class FixLabel : public QLabel
{
    Q_OBJECT
public:
    explicit FixLabel(QWidget *parent = 0);

public:
    void setLabelText(QString text);
    QString getText();

private Q_SLOTS:
    void changedLabelSlot();
private:
    QString mStr;

};

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

private Q_SLOTS:
    void onPaletteChanged();

};

#endif // FIXLABEL_H





