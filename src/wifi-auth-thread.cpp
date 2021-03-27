#include "wifi-auth-thread.h"

#include <QProcess>

void WifiAuthThread::run()
{
    sleep(3);
	QDBusInterface interface( "org.freedesktop.NetworkManager",
			"/org/freedesktop/NetworkManager",
			"org.freedesktop.NetworkManager",
			QDBusConnection::systemBus() );
	QDBusReply<uint> reply= interface.call("CheckConnectivity");
	if (reply.isValid()) {
        if (reply.value() == 2) {
            //system("xdg-open http://nmcheck.gnome.org");
            QProcess::startDetached(QString("xdg-open http://nmcheck.gnome.org"));
        }
	}
}


