#include "wifi-auth-thread.h"

#include <QDebug>

void WifiAuthThread::run()
{
    sleep(3);
	QDBusInterface interface( "org.freedesktop.NetworkManager",
			"/org/freedesktop/NetworkManager",
			"org.freedesktop.NetworkManager",
			QDBusConnection::systemBus() );
	QDBusReply<uint> reply= interface.call("CheckConnectivity");
	if (reply.isValid()) {
        //qDebug() << "-----------------------> 000000000000000011" << reply.value();
        if (reply.value() == 2) {
            system("gdbus call --session --dest org.gnome.Shell.PortalHelper --object-path /org/gnome/Shell/PortalHelper --method org.gtk.Application.Activate []");
        }
	}
}


