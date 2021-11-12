#ifndef DROWNLABEL_H
#define DROWNLABEL_H

#include <QObject>
#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

class DrownLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DrownLabel(QString devName, QWidget * parent = nullptr);
    ~DrownLabel();
    void setDropDownStatus(bool status);
     QString m_devName;
    bool isChecked = true;
private:
    void loadPixmap(bool isChecked);
protected:
    virtual void mouseReleaseEvent(QMouseEvent * event);

Q_SIGNALS:
    void labelClicked();
};

#endif // DROWNLABEL_H
