#ifndef NETWORKQML_PLUGIN_H
#define NETWORKQML_PLUGIN_H

#include <QQmlExtensionPlugin>

class NetworkhqmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
};

#endif // NETWORKQML_PLUGIN_H
