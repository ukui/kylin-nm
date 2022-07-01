#ifndef KYLINAGENT_H
#define KYLINAGENT_H

#include "kylinsecretagent.h".h"

#if defined(LIBKYLIN_KEYRING_LIBRARY)
#  define LIBKYLIN_KEYRING_LIBRARY Q_DECL_EXPORT
#else
#  define LIBKYLIN_KEYRING_LIBRARY Q_DECL_IMPORT
#endif

class LIBKYLIN_KEYRING_LIBRARY KylinAgent : public QObject
{
public:
    explicit KylinAgent(QObject *parent = nullptr);
    ~KylinAgent();
    void startKylinAgent();
    void setParentWidget(QWidget *widget){
        pWidget = widget;
    }

private:
    KylinSecretAgent *m_secretAgnet;
    QWidget* pWidget = nullptr;
};


#endif // KYLINAGENT_H
