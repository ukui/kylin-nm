#include "networkqml_plugin.h"
#include "knminterface.h"

#include <qqml.h>
#include <QQmlContext>

void NetworkhqmlPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QString(uri) == QLatin1String("org.ukui.shortcut.network"));
    qmlRegisterModule(uri, 1, 0);
    qmlRegisterSingletonType<KnmInterface>(uri, 1, 0, "KInterface", [] (QQmlEngine *, QJSEngine *) -> QObject* {
        KInterface::instance();
        return KInterface::getInstance();
    });
}
