#ifndef KSIMPLENM_H
#define KSIMPLENM_H

#include <QObject>
#include <QProcess>
#include <QDebug>

class KSimpleNM : public QObject
{
    Q_OBJECT
public:
    explicit KSimpleNM(QObject *parent = nullptr);

    QProcess *runShellProcess;
    QString shellOutput;
    int type;

    void execGetLanList();
    void execGetWifiList();

signals:
    void getLanListFinished(QStringList slist);
    void getWifiListFinished(QStringList slist);

public slots:
    void readProcess();
    void finishedProcess(int msg);
};

#endif // KSIMPLENM_H
