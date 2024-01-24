/*
 * Copyright (C) 2023, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#ifndef KYLINNETWORKRESOURCETYPE_H
#define KYLINNETWORKRESOURCETYPE_H


typedef enum{
    CONNECT_STATE_UNKONW = 0,                 /**< The active connection is in an unknown state */
    CONNECT_STATE_ACTIVATING,                 /**< The connection is activating */
    CONNECT_STATE_ACTIVATED,                  /**< The connection is activated */
    CONNECT_STATE_DEACTIVATING,               /**< The connection is being torn down and cleaned up */
    CONNECT_STATE_DEACTIVATED,                /**< The connection is no longer active */

    CONNECT_STATE_COUNT
}KyConnectState;

typedef enum{
    CHANGE_REASON_UNKNOWN = 0,          /**< The reason for the active connection state change is unknown */
    CHANGE_REASON_NONE,                 /**< No reason was given for the active connection state change */
    CHANGE_REASON_USER_DISCONNECTED,    /**< The active connection changed state because the user disconnected it */
    CHANGE_REASON_DEVICE_DISCONNECTIED, /**< The active connection changed state because the device it was using was disconnected */
    CHANGE_REASON_SERVICE_STOP,         /**< The service providing the VPN connection was stopped */

    CHANGE_REASON_IPCONFIG_INVALID = 5, /**< The IP config of the active connection was invalid */
    CHANGE_REASON_CONNECT_TIMEOUT,      /**< The connection attempt to the VPN service timed out */
    CHANGE_REASON_SERVICE_START_TIMEOUT,/**< A timeout occurred while starting the service providing the VPN connection */
    CHANGE_REASON_SERVICE_START_FAILED, /**< Starting the service providing the VPN connection failed */
    CHANGE_REASON_NO_SECRET,            /**< Necessary secrets for the connection were not provided */

    CHANGE_REASON_LOGIN_FAILED = 10,    /**< Authentication to the server failed */
    CHANGE_REASON_CONNECT_REMOVED,      /**< The connection was deleted from settings */
    CHANGE_REASON_DEPENDENCY_FAILED,    /**< Master connection of this connection failed to activate */
    CHANGE_REASON_DEVICE_REALIZE_FAILED,/**< Could not create the software device link */
    CHANGE_REASON_DEVICE_REMOVED,       /**< The device this connection depended on disappeared */

    CHANGE_REASON_COUNT
}KyConnectChangeReason;

typedef enum {
    CONNECT_TYPE_UNKNOWN = 0,
    CONNECT_TYPE_ADSL,
    CONNECT_TYPE_BLUETOOTH,
    CONNECT_TYPE_BOND,
    CONNECT_TYPE_BRIDGE,

    CONNECT_TYPE_CDMA = 5,
    CONNECT_TYPE_GSM,
    CONNECT_TYPE_INFINIBAND,
    CONNECT_TYPE_OLPCMESH,
    CONNECT_TYPE_PPPOE,

    CONNECT_TYPE_VLAN = 10,
    CONNECT_TYPE_VPN,
    CONNECT_TYPE_WIMAX,
    CONNECT_TYPE_WIRED,
    CONNECT_TYPE_WIRELESS,

    CONNECT_TYPE_TEAM = 15,
    CONNECT_TYPE_GENERIC,
    CONNECT_TYPE_TUN,
    CONNECT_TYPE_IPTUNNEL,
    CONNECT_TYPE_WIREDGUARD,

    CONNECT_TYPE_COUNT
}KyConnectionType;

typedef enum {
    CONNECTIVITY_UNKNOWN = 0, /**< Network connectivity is unknown. */
    CONNECTIVITY_NO,          /**< The host is not connected to any network. */
    CONNECTIVITY_PORTAL,      /**< The host is behind a captive portal and cannot reach the full Internet. */
    CONNECTIVITY_LIMITED,     /**< The host is connected to a network, but does not appear to be able to reach the full Internet. */
    CONNECTIVITY_FULL,        /**< The host is connected to a network, and appears to be able to reach the full Internet. */

    CONNECTIVITY_COUNT
}KyConnectivity;

typedef enum {
    DEVICE_TYPE_UNKNOWN = 0,      /**< Unknown device type */
    DEVICE_TYPE_ETHERNET,         /**< Ieee8023 wired ethernet */
    DEVICE_TYPE_WIFI,             /**< the Ieee80211 family of wireless networks */
    DEVICE_TYPE_UNUSED1,          /**< Currently unused */
    DEVICE_TYPE_UNUSED2,          /**< Currently unused */

    DEVICE_TYPE_BLUETOOTH = 5,    /**< network bluetooth device (usually a cell phone) */
    DEVICE_TYPE_OLPCMESH,         /**< OLPC Mesh networking device */
    DEVICE_TYPE_WIMAX,            /**< WiMax WWAN technology */
    DEVICE_TYPE_MODEM,            /**< POTS, GSM, CDMA or LTE modems */
    DEVICE_TYPE_INFINIBAND,       /**< Infiniband network device */

    DEVICE_TYPE_BOND = 10,        /**< Bond virtual device */
    DEVICE_TYPE_VLAN,             /**< Vlan virtual device */
    DEVICE_TYPE_ADSL,             /**< ADSL modem device */
    DEVICE_TYPE_BRIDGE,           /**< Bridge virtual device */
    DEVICE_TYPE_GENERIC,          /**< Generic device @since 1.0.0 */

    DEVICE_TYPE_TEAM = 15,        /**< Team master device @since 1.0.0 */
    DEVICE_TYPE_GRE,              /**< Gre virtual device @since 1.2.0, @deprecated use IpTunnel instead*/
    DEVICE_TYPE_MACVLAN,          /**< MacVlan virtual device @since 1.2.0 */
    DEVICE_TYPE_TUN,              /**< Tun virtual device @since 1.2.0 */
    DEVICE_TYPE_VETH,             /**< Veth virtual device @since 1.2.0 */

    DEVICE_TYPE_IPTUNNEL = 20,    /**< IP Tunneling Device @since 1.2.0 */
    DEVICE_TYPE_VXLAN,            /**< Vxlan Device @since 1.2.0 */
    DEVICE_TYPE_MACSEC,           /**< MacSec Device @since 1.6.0 */
    DEVICE_TYPE_DUMMY,            /**< Dummy Device @since 1.8.0 */
    DEVICE_TYPE_PPP,              /**< Ppp Device @since 1.10 */

    DEVICE_TYPE_OVSINTERFACE = 25,   /**< OvsInterface Device @since 1.10 */
    DEVICE_TYPE_OVSPORT,             /**< OvsPort Device @since 1.10 */
    DEVICE_TYPE_OVSBRIDGE,           /**< OvsBridge Device @since 1.10 */
    DEVICE_TYPE_WPAN,                /**< Wpan Device @since 1.14 */
    DEVICE_TYPE_LOWPAN,              /**< Lowpan Device @since 1.14 */

    DEVICE_TYPE_WIREGUARD = 30,      /**< WireGuard Device @since 1.14 */
    DEVICE_TYPE_WIFIP2P,             /**< WifiP2P Device @since 1.16 */
    DEVICE_TYPE_COUNT
}KyDeviceType;

typedef enum {
    DEVICE_STATE_UNKNOWN = 0,       /**< The device is in an unknown state */
    DEVICE_STATE_UNMANAGED = 10,    /**< The device is recognized but not managed by NetworkManager */
    DEVICE_STATE_UNAVAILABLE = 20,  /**< The device cannot be used (carrier off, rfkill, etc) */
    DEVICE_STATE_DISCONNECTED = 30, /**< The device is not connected */
    DEVICE_STATE_PREPARING = 40,    /**< The device is preparing to connect */
    DEVICE_STATE_CONFIGURINGHARDWARE = 50,  /**< The device is being configured */
    DEVICE_STATE_NEEDAUTH = 60,             /**< The device is awaiting secrets necessary to continue connection */
    DEVICE_STATE_CONFIGURINGIP = 70,        /**< The IP settings of the device are being requested and configured */
    DEVICE_STATE_CHECKINGIP = 80,           /**< The device's IP connectivity ability is being determined */
    DEVICE_STATE_WAITINGFORSECONDARIES = 90,    /**< The device is waiting for secondary connections to be activated */
    DEVICE_STATE_ACTIVATED = 100,               /**< The device is active */
    DEVICE_STATE_DEACTIVATING = 110,            /**< The device's network connection is being torn down */
    DEVICE_STATE_FAILED = 120                   /**< The device is in a failure state following an attempt to activate it */
}KyDeviceState;


/**
 * KyApSecurityFlags:
 * @AP_SEC_NONE: the access point has no special security requirements
 * @AP_SEC_PAIR_WEP40: 40/64-bit WEP is supported for
 * pairwise/unicast encryption
 * @AP_SEC_PAIR_WEP104: 104/128-bit WEP is supported for
 * pairwise/unicast encryption
 * @AP_SEC_PAIR_TKIP: TKIP is supported for pairwise/unicast encryption
 * @AP_SEC_PAIR_CCMP: AES/CCMP is supported for pairwise/unicast encryption
 * @AP_SEC_GROUP_WEP40: 40/64-bit WEP is supported for group/broadcast
 * encryption
 * @AP_SEC_GROUP_WEP104: 104/128-bit WEP is supported for
 * group/broadcast encryption
 * @AP_SEC_GROUP_TKIP: TKIP is supported for group/broadcast encryption
 * @AP_SEC_GROUP_CCMP: AES/CCMP is supported for group/broadcast
 * encryption
 * @AP_SEC_KEY_MGMT_PSK: WPA/RSN Pre-Shared Key encryption is
 * supported
 * @AP_SEC_KEY_MGMT_802_1X: 802.1x authentication and key management
 * is supported
 * @AP_SEC_KEY_MGMT_SAE: WPA/RSN Simultaneous Authentication of Equals is
 * supported
 *
 * 802.11 access point security and authentication flags.  These flags describe
 * the current security requirements of an access point as determined from the
 * access point's beacon.
 **/
typedef enum { /*< underscore_name=nm_802_11_ap_security_flags, flags >*/
    AP_SECURITY_NONE            = 0x00000000,
    AP_SECURITY_PAIR_WEP40      = 0x00000001,
    AP_SECURITY_PAIR_WEP104     = 0x00000002,
    AP_SECURITY_PAIR_TKIP       = 0x00000004,
    AP_SECURITY_PAIR_CCMP       = 0x00000008,
    AP_SECURITY_GROUP_WEP40     = 0x00000010,
    AP_SECURITY_GROUP_WEP104    = 0x00000020,
    AP_SECURITY_GROUP_TKIP      = 0x00000040,
    AP_SECURITY_GROUP_CCMP      = 0x00000080,
    AP_SECURITY_KEY_MGMT_PSK    = 0x00000100,
    AP_SECURITY_KEY_MGMT_802_1X = 0x00000200,
    AP_SECURITY_KEY_MGMT_SAE    = 0x00000400,
}KyApSecurityFlags;

/**
 * KyApCapabilities:
 * @AP_FLAGS_NONE: access point has no special capabilities
 * @AP_FLAGS_PRIVACY: access point requires authentication and
 * encryption (usually means WEP)
 * @AP_FLAGS_WPS: access point supports some WPS method
 * @AP_FLAGS_WPS_PBC: access point supports push-button WPS
 * @AP_FLAGS_WPS_PIN: access point supports PIN-based WPS
 *
 * 802.11 access point fCapabilities.
 **/
typedef enum {
    AP_CAPABILITY_NONE    = 0x00000000,
    AP_CAPABILITY_PRIVACY = 0x00000001,
    AP_CAPABILITY_WPS     = 0x00000002,
    AP_CAPABILITY_WPS_PBC = 0x00000004,
    AP_CAPABILITY_WPS_PIN = 0x00000008,
}KyApCapabilities;

//class KySecretFlagType {
//public:
//    enum KySecretFlag{
//        SECRET_FLAG_NONE = 0,
//        SECRET_FLAG_AgentOWNED = 0x01,
//        SECRET_FLAG_NOTSAVED = 0x02,
//        SECRET_FLAG_NOTREQUIRED = 0x04
//    };
//    Q_DECLARE_FLAGS(kySecretFlags, KySecretFlag)
//};
//Q_DECLARE_OPERATORS_FOR_FLAGS(KySecretFlagType::KySecretFlags)

#endif // KYLINNETWORKRESOURCETYPE_H
