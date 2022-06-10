#ifndef DEVICEFRAME_H
#define DEVICEFRAME_H
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

class DeviceFrame : public QFrame
{
public:
    DeviceFrame(QString devName, QWidget *parent = nullptr);
public:
    QLabel * deviceLabel = nullptr;
};

#endif // DEVICEFRAME_H
