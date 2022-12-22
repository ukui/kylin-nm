#ifndef SECURITYWIDGET_H
#define SECURITYWIDGET_H

#include <QWidget>
#include <QFormLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>

#include "coninfo.h"
#include "kwidget.h"
#include "kpasswordedit.h"
#include "detailwidget.h"

using namespace kdk;

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
    void setLeapInfo(KyEapMethodLeapInfo &info);
    void setPwdInfo(KyEapMethodPwdInfo &info);
    void setFastInfo(KyEapMethodFastInfo &info);
    void setSecurityVisible(const bool &visible);

    bool checkIsChanged(const ConInfo info);
    void updateSecurityChange(KyWirelessConnectSetting &setting);
    void updateTlsChange(KyEapMethodTlsInfo &info);
    void updatePeapChange(KyEapMethodPeapInfo &info);
    void updateTtlsChange(KyEapMethodTtlsInfo &info);
    void updateLeapChange(KyEapMethodLeapInfo &info);
    void updatePwdChange(KyEapMethodPwdInfo &info);
    void updateFastChange(KyEapMethodFastInfo &info);

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
    KPasswordEdit *pwdEdit = nullptr;
    QComboBox *eapTypeCombox;
    //TLS
    LineEdit *identityEdit;
    LineEdit *domainEdit;
    QComboBox *caCertPathCombox;
    QCheckBox *caNeedBox;
    QComboBox *clientCertPathCombox;
    QComboBox *clientPrivateKeyCombox;
    KPasswordEdit *clientPrivateKeyPwdEdit = nullptr;

    //PEAP && TTLS
    QComboBox *eapMethodCombox;
    LineEdit *userNameEdit;
    KPasswordEdit *userPwdEdit = nullptr;
    QCheckBox *userPwdFlagBox;

    //FAST
    QCheckBox *m_pacCheckBox;
    QComboBox *m_pacProvisionComboBox;
    QComboBox *m_pacFilePathComboBox;
    FixLabel *m_pacProvisionLabel;
    FixLabel *m_pacFlagLabel;
    QLabel *m_pacFileLabel;
    QWidget *m_pacCheckWidget;

    void showNone();
    void showPsk();
    void showTls();
    void showPeapOrTtls();
    void showLeapOrPwd();
    void showFast();
    void initUI();
    void initConnect();

    KyEapMethodTlsInfo assembleTlsInfo();
    KyEapMethodPeapInfo assemblePeapInfo();
    KyEapMethodTtlsInfo assembleTtlsInfo();
    KyEapMethodLeapInfo assembleLeapInfo();
    KyEapMethodPwdInfo assemblePwdInfo();
    KyEapMethodFastInfo assembleFastInfo();

    bool checkConnectBtnIsEnabled();


private slots:
    void onSecuTypeComboxIndexChanged();
    void onEapTypeComboxIndexChanged();
    void setEnableOfSaveBtn();

    void onCaNeedBoxClicked();
    void onPacBoxClicked();

    void onCaCertPathComboxIndexChanged(QString str);
    void onClientCertPathComboxIndexChanged(QString str);
    void onClientPrivateKeyComboxIndexChanged(QString str);
    void onPacFilePathComboxIndexChanged(QString str);

signals:
    void setSecuPageState(bool);
    void eapTypeChanged(const KyEapMethodType &type);
};

#endif // SECURITYWIDGET_H
