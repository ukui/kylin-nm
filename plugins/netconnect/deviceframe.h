#ifndef DEVICEFRAME_H
#define DEVICEFRAME_H
#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include "../component/DrownLabel/drownlabel.h"
#include "kwidget.h"
#include "kswitchbutton.h"

using namespace kdk;

class DeviceFrame : public QFrame
{

public:
    DeviceFrame(QString devName, QWidget *parent = nullptr);
    ~DeviceFrame();
public:
    QLabel * deviceLabel = nullptr;
    KSwitchButton * deviceSwitch = nullptr;
    DrownLabel *dropDownLabel = nullptr;

protected:
    void paintEvent(QPaintEvent *event);

private:
    bool isDropDown = false;
    int frameSize;

};

#endif // DEVICEFRAME_H
