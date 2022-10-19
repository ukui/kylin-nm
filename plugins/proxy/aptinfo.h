#ifndef APTINFO_H
#define APTINFO_H
#include <QHash>
#include <QVariant>
#include <QString>
#include <QDBusArgument>

struct AptInfo
{
    QString arg;
    QDBusVariant out;
};

QDBusArgument &operator<<(QDBusArgument &argument, const AptInfo &mystruct)
{
    argument.beginStructure();
    argument << mystruct.arg << mystruct.out;
    argument.endStructure();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, AptInfo &mystruct)
{
    argument.beginStructure();
    argument >> mystruct.arg >> mystruct.out;
    argument.endStructure();
    return argument;
}

Q_DECLARE_METATYPE(AptInfo)

#endif // APTINFO_H
