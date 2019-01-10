#include "ksimplenm.h"
#include <stdio.h>
#define MAX_LEN 2048
#define MAX_PATH 1024

KSimpleNM::KSimpleNM(QObject *parent) : QObject(parent)
{
    runShellProcess =new QProcess(this);

    connect(runShellProcess, &QProcess::readyRead, this, &KSimpleNM::readProcess);
    connect(runShellProcess, SIGNAL(finished(int)), this, SLOT(finishedProcess(int)));
}

void KSimpleNM::execGetLanList(){
    shellOutput = "";
    type = 0;
    runShellProcess->start("nmcli -f type,device,name connection show");
}

void KSimpleNM::execGetWifiList(){
    shellOutput = "";
    type = 1;
    runShellProcess->start("nmcli -f signal,rate,security,ssid device wifi");
}

void KSimpleNM::readProcess(){
    QString output = runShellProcess->readAll();
    shellOutput += output;
}

void KSimpleNM::finishedProcess(int msg){
    QStringList slist = shellOutput.split("\n");
    if(type == 0){
        emit getLanListFinished(slist);
    }else{
        emit getWifiListFinished(slist);
    }
}
