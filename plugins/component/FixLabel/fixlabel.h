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

#endif // FIXLABEL_H





