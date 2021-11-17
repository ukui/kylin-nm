#ifndef THREADDRIVER_H
#define THREADDRIVER_H


#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QDebug>

class ThreadDriver:public QObject
{
    Q_OBJECT
public:
    ThreadDriver(QObject* parent=0):QObject(parent){}

public slots:
    void emitSignal() {
        emit initSignal();
    }

signals:
    void initSignal();
};

#endif // THREADDRIVER_H
