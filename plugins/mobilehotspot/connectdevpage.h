#ifndef CONNECTDEVPAGE_H
#define CONNECTDEVPAGE_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include <QDBusMessage>
#include <QDBusObjectPath>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMetaType>

#include "titlelabel.h"
#include "connectdevlistitem.h"

using namespace kdk;

class ConnectdevPage : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectdevPage(QWidget *parent = nullptr);
    ~ConnectdevPage();

    void refreshStalist();

    inline void setInterface(QDBusInterface *activeInterface) {
            m_avtivePathInterface = activeInterface;
    }

private:
    QFrame* myLine();

    TitleLabel *m_titleLabel = nullptr;
    QVBoxLayout *m_staListLayout = nullptr;
    QMap<QString, QString> m_staMap;

    QDBusInterface  *m_avtivePathInterface = nullptr;

    void getConnectStaDevice(QMap<QString, QString> &blacklistMap);
    bool removeStaFromBlacklist(QString staMac);
    void initStaDev();
    void addStaDevFrame(QString staMac, QString staName);
    void clearStaListLayout();
    QString getActivePathByUuid(QDBusInterface *interface);
    void initNmDbus(QDBusInterface *interface);

    void onStaDevChanged(bool istrue, QString staMac, QString staName);

signals:
    void setStaIntoBlacklist(QString staMac);

public slots:
    void onStaDevAdded(bool istrue, QString staMac, QString staName);
    void onStaDevRemoved(bool istrue, QString staMac, QString staName);

private slots:
    void onDropIntoBlacklistBtnClicked(QString staMac);
};

#endif // MOBILEHOTSPOTWIDGET_H
