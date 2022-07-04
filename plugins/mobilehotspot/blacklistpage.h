#ifndef BLACKLISTPAGE_H
#define BLACKLISTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

#include <QDBusMessage>
#include <QDBusObjectPath>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMetaType>

#include "titlelabel.h"
#include "blacklistitem.h"

using namespace kdk;

class BlacklistPage : public QWidget
{
    Q_OBJECT
public:
    explicit BlacklistPage(QWidget *parent = nullptr);
    ~BlacklistPage();

    void refreshBlacklist();
    bool setStaIntoBlacklist(QString staMac);

    inline void setInterface(QDBusInterface *settingInterface) {
            m_settingPathInterface = settingInterface;
    }

private:

    QFrame* myLine();

    TitleLabel *m_titleLabel = nullptr;
    QVBoxLayout *m_blacklistLayout = nullptr;
    QMap<QString, QString> m_blacklistMap;

    QDBusInterface  *m_settingPathInterface = nullptr;

    void getBlacklistDevice(QMap<QString, QString> &blacklistMap);
    bool removeStaFromBlacklist(QString staMac);
    void initBlacklistDev();
    void addBlacklistDevFrame(QString staMac, QString staName);
    void clearBlacklistLayout();

signals:

private slots:
    void onsetStaIntoBlacklist(QString staMac);
    void onRemoveFromBlacklistBtnClicked(QString staMac);
};

#endif // MOBILEHOTSPOTWIDGET_H
