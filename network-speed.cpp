#include "networkspeed.h"

//NetworkSpeed::NetworkSpeed(QObject *parent) :QObject(parent)
//{
//    qDebug()<<"debug: this is creator function of class NetworkSpeed";
//}

int NetworkSpeed::getCurrentDownloadRates(char *netname, long *save_rate, long *tx_rate)
{
    FILE * net_dev_file;    //文件指针
    char buffer[1024];  //文件中的内容暂存在字符缓冲区里
    size_t bytes_read;  //实际读取的内容大小
    char * match;    //用以保存所匹配字符串及之后的内容
    int counter = 0;
    int i = 0;
    char tmp_value[128];

    if((NULL == netname)||(NULL == save_rate)||(NULL == tx_rate))
    {
        printf("bad param\n");
        return -1;
    }

    if ( (net_dev_file=fopen("/proc/net/dev", "r")) == NULL ) //打开文件/pro/net/dev/，我们要读取的数据就是它啦
    {
        printf("open file /proc/net/dev/ error!\n");
        return -1;
    }
    memset(buffer,0,sizeof(buffer));

    while(fgets(buffer,sizeof(buffer),net_dev_file) != NULL)
    {
        match = strstr(buffer,netname);

        if(NULL == match)
        {
            //printf("no eth0 keyword to find!\n");
            continue;
        }
        else
        {
            //printf("%s\n",buffer);
            match = match + strlen(netname) + strlen(":");/*地址偏移到冒号*/
            sscanf(match,"%ld ",save_rate);
            memset(tmp_value,0,sizeof(tmp_value));
            sscanf(match,"%s ",tmp_value);
            match = match + strlen(tmp_value);
            for(i=0;i<strlen(buffer);i++)
            {
                if(0x20 == *match)
                {
                    match ++;
                }
                else
                {
                    if(8 == counter)
                    {
                        sscanf(match,"%ld ",tx_rate);
                    }
                    memset(tmp_value,0,sizeof(tmp_value));
                    sscanf(match,"%s ",tmp_value);
                    match = match + strlen(tmp_value);
                    counter ++;
                }
            }
            //printf("%s save_rate:%ld tx_rate:%ld\n",netname,*save_rate,*tx_rate);
        }
    }

    return 0;/*返回成功*/
}
