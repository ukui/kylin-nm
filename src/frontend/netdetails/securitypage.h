#ifndef SECURITYWIDGET_H
#define SECURITYWIDGET_H

#include <QWidget>
#include <QFormLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>

#include "coninfo.h"


class SecurityPage : public QFrame
{
    Q_OBJECT
public:
    SecurityPage(QWidget *parent = nullptr);

    void setSecurity(KySecuType index);
    void setPsk(const QString &psk);
    void setTlsInfo(KyEapMethodTlsInfo &info);
    void setPeapInfo(KyEapMethodPeapInfo &info);
    void setTtlsInfo(KyEapMethodTtlsInfo &info);

    bool checkIsChanged(const ConInfo info);
    void updateSecurityChange(KyWirelessConnectSetting &setting);
    void updateTlsChange(KyEapMethodTlsInfo &info);
    void updatePeapChange(KyEapMethodPeapInfo &info);
    void updateTtlsChange(KyEapMethodTtlsInfo &info);

    void getSecuType(KySecuType &secuType, KyEapMethodType &enterpriseType);

private:
    QFormLayout *mSecuLayout;

private:

    QLabel *secuTypeLabel;
    QLabel *pwdLabel;
    //企业wifi共有
    QLabel *eapTypeLabel;
    //TLS
    QLabel *identityLable;
    QLabel *domainLable;
    QLabel *caCertPathLabel;
    QLabel *caNeedFlagLabel;
    QLabel *clientCertPathLabel;
    QLabel *clientPrivateKeyLabel;
    QLabel *clientPrivateKeyPwdLabel;

    //PEAP TTLS共有
    QLabel *eapMethodLabel;
    QLabel *userNameLabel;
    QLabel *userPwdLabel;
    QLabel *userPwdFlagLabel;

    QComboBox *secuTypeCombox;
    QLineEdit *pwdEdit;
    QComboBox *eapTypeCombox;
    //TLS
    QLineEdit *identityEdit;
    QLineEdit *domainEdit;
    QComboBox *caCertPathCombox;
    QCheckBox *caNeedBox;
    QComboBox *clientCertPathCombox;
    QComboBox *clientPrivateKeyCombox;
    QLineEdit *clientPrivateKeyPwdEdit;

    //PEAP && TTLS
    QComboBox *eapMethodCombox;
    QLineEdit *userNameEdit;
    QLineEdit *userPwdEdit;
    QCheckBox *userPwdFlagBox;

    QCheckBox *pwdBox;
    QCheckBox *userPwdBox;
    QCheckBox *privateKeyBox;

    void showNone();
    void showPsk();
    void showTls();
    void showPeapOrTtls();
    void initUI();
    void initConnect();

    KyEapMethodTlsInfo assembleTlsInfo();
    KyEapMethodPeapInfo assemblePeapInfo();
    KyEapMethodTtlsInfo assembleTtlsInfo();

    bool checkConnectBtnIsEnabled();


private slots:
    void onSecuTypeComboxIndexChanged();
    void onEapTypeComboxIndexChanged();
    void setEnableOfSaveBtn();

signals:
    void setSecuPageState(bool);
};

#endif // SECURITYWIDGET_H
