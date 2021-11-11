#ifndef DEVICEFRAME_H
#define DEVICEFRAME_H
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include "../component/SwitchButton/switchbutton.h"
#include "../component/DrownLabel/drownlabel.h"

class DeviceFrame : public QFrame
{

public:
    DeviceFrame(QString devName, QWidget *parent = nullptr);
    ~DeviceFrame();
public:
    QLabel * deviceLabel = nullptr;
    SwitchButton * deviceSwitch = nullptr;
    DrownLabel *dropDownLabel = nullptr;

private:
    bool isDropDown = false;
    int frameSize;

};

#endif // DEVICEFRAME_H
