#ifndef DetailWidget_H
#define DetailWidget_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class DetailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DetailWidget(QWidget *valueWidget = nullptr, QWidget *parent = nullptr);
    ~DetailWidget();

    void setKey(const QString &keyLabel);

private:
    QHBoxLayout * m_mainLayout = nullptr;
    QLabel * m_keyLabel = nullptr;
    QWidget * m_valueWidget = nullptr;

    void initUI();
signals:

};

#endif // DetailWidget_H
