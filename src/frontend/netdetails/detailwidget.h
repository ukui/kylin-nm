#ifndef DetailWidget_H
#define DetailWidget_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

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
    explicit DetailWidget(QWidget *valueWidget = nullptr, QWidget *parent = nullptr, QWidget *buttonWidget = nullptr);
    ~DetailWidget();

    void setKey(const QString &keyLabel);

private:
    QHBoxLayout *m_mainLayout = nullptr;
    FixLabel    *m_keyLabel = nullptr;
    QWidget     *m_valueWidget = nullptr;
    QWidget     *m_copyButton;

    void initUI();

signals:

};

#endif // DetailWidget_H
