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

}
using namespace KYLIN_NM;
#endif // COMMON_H
