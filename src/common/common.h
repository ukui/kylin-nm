#ifndef COMMON_H
#define COMMON_H

namespace KYLIN_NM
{
    enum KySecuType {
        NONE = 0,
        WPA_AND_WPA2_PERSONAL,
        WPA_AND_WPA2_ENTERPRISE,
        WPA3_PERSONAL,
        WPA_AND_WPA3,
    };

    enum KyKeyMgmt {
        Unknown = -1,
        Wep, Ieee8021x,
        WpaNone,
        WpaPsk,
        WpaEap,
        SAE
    };

    enum KyNetworkDevType {
        KyWIRED = 0,
        KyWIRELESS
    };

#define GSETTING_SCHEMA_UKCC "org.ukui.control-center.plugins"
#define GSETTING_PATH_UKCC_MOBILEHOTSPOT "/org/ukui/control-center/plugins/mobilehotspot/"
#define GSETTING_PATH_UKCC_NETCONNECT "/org/ukui/control-center/plugins/netconnect/"
#define GSETTING_PATH_UKCC_WLANCONNECT "/org/ukui/control-center/plugins/wlanconnect/"

}
using namespace KYLIN_NM;
#endif // COMMON_H
