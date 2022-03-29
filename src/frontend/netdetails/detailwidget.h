#ifndef DetailWidget_H
#define DetailWidget_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QEvent>

#include <QToolButton>

//文本长自动省略并添加悬浮
class FixLabel : public QLabel
{

    Q_OBJECT
public:
    FixLabel(QWidget *parent = nullptr);
    ~FixLabel();
    void setText(const QString &text, bool saveTextFlag = true);
private:
    void paintEvent(QPaintEvent *event);

private:
    QString mStr;
};

class DetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DetailWidget(QWidget *valueWidget = nullptr, QWidget *parent = nullptr);
    ~DetailWidget();

    void setKey(const QString &keyLabel);

//    void firstDetailWidget();

private:
    QHBoxLayout * m_mainLayout = nullptr;
    FixLabel * m_keyLabel = nullptr;
    QWidget * m_valueWidget = nullptr;

    void initUI();

signals:

};

class FirstDetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FirstDetailWidget(QWidget *valueWidget = nullptr, QWidget *parent = nullptr);
    ~FirstDetailWidget();

    void setKey(const QString &keyLabel);

private:
    QHBoxLayout * m_mainLayout = nullptr;
    FixLabel * m_keyLabel = nullptr;
    QWidget * m_valueWidget = nullptr;

    void initUI();

signals:
    void sig_click();

};

class CopyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CopyButton();
    ~CopyButton();

protected:
//    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    bool event(QEvent *e);
};

#endif // DetailWidget_H
