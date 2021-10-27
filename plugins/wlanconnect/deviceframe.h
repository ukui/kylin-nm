#ifndef DEVICEFRAME_H
#define DEVICEFRAME_H
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include "drownlabel.h"

class DeviceFrame : public QFrame
{

public:
    DeviceFrame(QString devName, QWidget *parent = nullptr);
    ~DeviceFrame();
public:
    //仅设备名称+下拉label
    QLabel * deviceLabel = nullptr;
    DrownLabel *dropDownLabel = nullptr;

private:
    int frameSize;

};

#endif // DEVICEFRAME_H
