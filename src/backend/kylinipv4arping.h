#ifndef KYLIN_IPV4_ARPING
#define KYLIN_IPV4_ARPING

#include <arpa/inet.h>
#include <errno.h>
#include <ifaddrs.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/rtnetlink.h>
#include <netdb.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <poll.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/signalfd.h>
#include <sys/timerfd.h>
#include <unistd.h>


#include <QString>
#include <QDebug>
#include <QObject>

enum {
    POLLFD_TIMER = 0,
    POLLFD_SOCKET,
    POLLFD_COUNT
};

class KyIpv4Arping : public QObject
{
    Q_OBJECT
public:
    explicit KyIpv4Arping(QString ifaceName, QString ipAddress, int retryCount=3, int timeout=1, QObject *parent = nullptr);
    ~KyIpv4Arping();

public:
    bool ipv4IsConflict() {
        return m_ipConflict;
    }

    QString getMacAddress() {
        return m_conflictMac;
    }

    int ipv4ConflictCheck();

private:
    void monoGetTime(struct timespec *ts);
    void saveMacAddress(const uint8_t *ptr, size_t len);
    void findBroadcastAddress();
    int checkDevice();
    int checkIfflags(unsigned int ifflags);

    int sendIpv4Packet();
    int ipv4EventLoop();
    int ipv4PacketProcess(unsigned char *buf, ssize_t len, struct sockaddr_ll *from);

    void setConflictFlag(bool isConflict);

private:
    struct in_addr m_srcAddress;
    struct in_addr m_destAddress;
    int m_destAddressFamily;

    struct sockaddr_storage m_me;
    struct sockaddr_storage m_he;
    int  m_ipv4Socket = 0;

    QString m_ifaceName;
    int m_ifindex;
    struct ifaddrs *m_ifa;
    struct ifaddrs *m_ifa0;

    QString m_ipv4Address;
    int     m_retryCount;
    int     m_timeout;

    bool    m_ipConflict = false;
    QString m_conflictMac = nullptr;
};

#endif
