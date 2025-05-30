#ifndef PANELGSETTINGS_H
#define PANELGSETTINGS_H

#include <QObject>

class PanelGSettings : public QObject
{
    Q_OBJECT
public:
    static PanelGSettings *instance();

    int getPanelLength(QString screenName);

    ~PanelGSettings();

private:
    PanelGSettings(QObject *parent = nullptr);

    bool isKeysContain(const char* key);

    QMap<QString, QVariant> getPanelLengthMap();
};

#endif // PANELGSETTINGS_H
