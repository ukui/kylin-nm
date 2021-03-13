#include "wifi-auth-thread.h"
void WifiAuthThread::run()
{
	QDBusInterface interface( "org.freedesktop.NetworkManager",
			"/org/freedesktop/NetworkManager",
			"org.freedesktop.NetworkManager",
			QDBusConnection::systemBus() );
	QDBusReply<uint> reply= interface.call("CheckConnectivity");
	if (reply.isValid()) {
		if (reply.value() == 2)
			system("gdbus call --session --dest org.gnome.Shell.PortalHelper --object-path /org/gnome/Shell/PortalHelper --method org.gtk.Application.Activate []");
	}
}


