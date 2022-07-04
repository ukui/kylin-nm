#ifndef DetailWidget_H
#define DetailWidget_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

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
