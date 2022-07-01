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
    void setSecurityVisible(const bool &visible);

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
    LineEdit *pwdEdit;
    QComboBox *eapTypeCombox;
    //TLS
    LineEdit *identityEdit;
    LineEdit *domainEdit;
    QComboBox *caCertPathCombox;
    QCheckBox *caNeedBox;
    QComboBox *clientCertPathCombox;
    QComboBox *clientPrivateKeyCombox;
    LineEdit *clientPrivateKeyPwdEdit;

    //PEAP && TTLS
    QComboBox *eapMethodCombox;
    LineEdit *userNameEdit;
    LineEdit *userPwdEdit;
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


private Q_SLOTS:
    void onSecuTypeComboxIndexChanged();
    void onEapTypeComboxIndexChanged();
    void setEnableOfSaveBtn();

    void onCaNeedBoxClicked();
    void onPwdBoxClicked();
    void onUserPwdBox();
    void onPrivateKeyBoxClicked();

    void onCaCertPathComboxIndexChanged(QString str);
    void onClientCertPathComboxIndexChanged(QString str);
    void onClientPrivateKeyComboxIndexChanged(QString str);

Q_SIGNALS:
    void setSecuPageState(bool);
    void eapTypeChanged(const KyEapMethodType &type);
};

#endif // SECURITYWIDGET_H
