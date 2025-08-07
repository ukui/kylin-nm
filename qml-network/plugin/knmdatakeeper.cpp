#include "knmdatakeeper.h"
#include "knminterface.h"

KnmDataKeeper::KnmDataKeeper(QObject *parent)
    : QObject{parent}
{
}

KnmDataKeeper::~KnmDataKeeper()
{
    if (nullptr != m_pSpeedTimer) {
        delete m_pSpeedTimer;
        m_pSpeedTimer = nullptr;
    }
}

void KnmDataKeeper::addDevice(QString devName, bool isAvailable, DeviceType type)
{
    if (!isAvailable)
        return;
    NetDevicePtr dev(new NetDevice(devName, type));
    dev->setAvailable(isAvailable);
    m_deviceList.insert(devName, dev);
}

void KnmDataKeeper::clearDevice()
{
    m_deviceList.clear();
}

QMap<QString, NetDevicePtr> KnmDataKeeper::getDeviceList()
{
    return m_deviceList;
}

bool KnmDataKeeper::getWiredMainSwitchState()
{
    return m_wiredMainSwitchState;
}

void KnmDataKeeper::setWiredMainSwitchState(bool switched)
{
    qWarning()<< Q_FUNC_INFO <<__LINE__ << switched;
    m_wiredMainSwitchState = switched;
}


bool KnmDataKeeper::getSwitchState()
{
    return m_switchState;
}

void KnmDataKeeper::setSwitchState(bool switched)
{
    m_switchState = switched;
}

QVariantList KnmDataKeeper::getDevConnections(QString devName)
{
    QVariantList ret;
    if (m_deviceList.isEmpty() || !m_deviceList.contains(devName))
        return ret;
    m_currentDev = devName;
//    m_pSpeedTimer->start();
    return m_deviceList.value(devName)->getConnections();
}

void KnmDataKeeper::clearConnectionList(QString devName)
{
    if (!m_deviceList.contains(devName))
        return;
    NetDevicePtr dev = m_deviceList.take(devName);
    dev->clearConnections();
    m_deviceList.insert(devName, dev);
}

int KnmDataKeeper::getCurrentLoadRate(QString dev, long *save_rate, long *tx_rate)
{
    FILE * net_dev_file; //文件指针
    char buffer[1024]; //文件中的内容暂存在字符缓冲区里
    //size_t bytes_read; //实际读取的内容大小
    char * match; //用以保存所匹配字符串及之后的内容
    char * device;//将QString转为Char *
    QByteArray ba = dev.toLatin1(); // must
    device = ba.data();
    int counter = 0;
    //int i = 0;
    char tmp_value[128];

    if ((NULL == device) || (NULL == save_rate) || (NULL == tx_rate)) {
        qDebug() << "parameter pass error" ;
        return -1;
    }

    if ((net_dev_file = fopen("/proc/net/dev", "r")) == NULL) {
        //打开文件/pro/net/dev/，从中读取流量数据
        qDebug() << "error occurred when try to open file /proc/net/dev/";
        return -1;
    }
    memset(buffer, 0, sizeof(buffer));

    while (fgets(buffer, sizeof(buffer), net_dev_file) != NULL) {
        match = strstr(buffer, device);

        if (NULL == match) {
            // qDebug()<<"No eth0 keyword to find!";
            continue;
        } else {
            match = match + strlen(device) + strlen(":"); //地址偏移到冒号
            sscanf(match, "%ld ", save_rate);
            memset(tmp_value, 0, sizeof(tmp_value));
            sscanf(match, "%s ", tmp_value);
            match = match + strlen(tmp_value);
            for (size_t i=0; i<strlen(buffer); ++i) {
                if (0x20 == *match) {
                    match ++;
                } else {
                    if (8 == counter) {
                        sscanf(match, "%ld ", tx_rate);
                    }
                    memset(tmp_value, 0, sizeof(tmp_value));
                    sscanf(match, "%s ", tmp_value);
                    match = match + strlen(tmp_value);
                    counter ++;
                }
            }
        }
    }

    fclose(net_dev_file);
    net_dev_file = nullptr;

    return 0; //返回成功
}

void KnmDataKeeper::netSpeedHandler(QString dev, QString& upLoad, QString& downLoad)
{
    if (getCurrentLoadRate(dev, &start_rcv_rates, &start_tx_rates) == -1) {
        start_rcv_rates = end_rcv_rates;
        return;
    }

    long int delta_rcv = (start_rcv_rates - end_rcv_rates) / 1024;
    long int delta_tx = (start_tx_rates - end_tx_rates) / 1024;

    //简易滤波
    if (delta_rcv < 0 || delta_tx < 0) {
        delta_rcv = 0;
        delta_tx = 0;
    }
    else if (end_rcv_rates == 0 || end_tx_rates == 0){
        delta_rcv = 0;
        delta_tx = 0;
    }

    end_rcv_rates = start_rcv_rates;
    end_tx_rates = start_tx_rates;

    int rcv_num = delta_rcv;
    int tx_num = delta_tx;

    QString str_rcv = 0;
    QString str_tx = 0;

    if (rcv_num < 1024) {
        str_rcv = QString::number(rcv_num) + "KB/s";
    } else {
        int remainder;
        if (rcv_num % 1024 < 100) {
            remainder = 0;
        } else {
            remainder = (rcv_num % 1024) / 100;
        }
        str_rcv = QString::number(rcv_num / 1024) + "."  + QString::number(remainder) + "MB/s";
    }

    if (tx_num < 1024) {
        str_tx = QString::number(tx_num) + "KB/s";
    } else {
        int remainder;
        if (tx_num % 1024 < 100) {
            remainder = 0;
        } else {
            remainder = (tx_num % 1024)/100;
        }
        str_tx = QString::number(tx_num / 1024) + "."  + QString::number(remainder) + "MB/s";
    }
    upLoad = str_tx;
    downLoad = str_rcv;
}

void KnmDataKeeper::slotSpeedTimeout()
{
    QString upLoad, downLoad;
    netSpeedHandler(m_currentDev, upLoad, downLoad);
    setUpwardRate(upLoad);
    setDownwardRate(downLoad);
    NetDevicePtr dev=m_deviceList[m_currentDev];
    if(!dev.isNull() && dev->getDevType()==WIRED_DEVICE)
    {
        emit KInterface::getInstance()->updateUpLoadWiredStr(upLoad);
        emit KInterface::getInstance()->updateDownLoadWiredStr(downLoad);
    }
    else
    {
        emit KInterface::getInstance()->updateUpLoadWirelessStr(upLoad);
        emit KInterface::getInstance()->updateDownLoadWirelessStr(downLoad);
    }
}

void KnmDataKeeper::netSpeedInit()
{

    //定时获取网速
    m_pSpeedTimer = new QTimer(this);
    m_pSpeedTimer->setTimerType(Qt::PreciseTimer);
    m_pSpeedTimer->start(1000);


    connect(m_pSpeedTimer, &QTimer::timeout, this,&KnmDataKeeper::slotSpeedTimeout);

    // //定时获取网速
    // m_pSpeedTimer = new QTimer(this);
    // m_pSpeedTimer = new QTimer(this);

    // m_pSpeedTimer->setInterval(500);
    // m_pSpeedTimer->setInterval(500);

    // connect(m_pSpeedTimer, &QTimer::timeout,  [&]() {
    //     QString upLoad, downLoad;
    //     netSpeedHandler(m_currentDev, upLoad, downLoad);
    //     KInterface::getInstance()->updateUpLoadWiredStr(upLoad);
    //     KInterface::getInstance()->updateDownLoadWiredStr(downLoad);
    // });
    // connect(m_pSpeedTimer, &QTimer::timeout,  [&]() {
    //     QString upLoad, downLoad;
    //     netSpeedHandler(m_currentDev, upLoad, downLoad);
    //     KInterface::getInstance()->updateUpLoadWirelessStr(upLoad);
    //     KInterface::getInstance()->updateDownLoadWirelessStr(downLoad);
    // });
}

void KnmDataKeeper::setUpwardRate(QString str)
{
    m_upward_rate = str;
}

QString KnmDataKeeper::getUpwardRate()
{
    return m_upward_rate;
}

void KnmDataKeeper::setDownwardRate(QString str)
{
    m_downward_rate = str;
}

QString KnmDataKeeper::getDownwardRate()
{
    return m_downward_rate;
}



