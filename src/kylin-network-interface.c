/*
 * Copyright (C) 2020 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/&gt;.
 *
 */

#include "kylin-network-interface.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <linux/route.h>
#include <netinet/ether.h>
#include <net/ethernet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/syslog.h>
#include <pwd.h>


//获取网络接口名
ifname *kylin_network_get_ifacename()
{
    int i = 0;
    int sockfd;
    struct ifconf ifconf;
    struct ifreq *ifreq;
    unsigned char buf[1024];

    //初始化ifconf
    ifconf.ifc_len = 1024;
    //字符串指针ifconf.ifc_buf指向buf
    ifconf.ifc_buf = buf;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket error");
        return 0;
    }

    //获取所有接口信息
    ioctl(sockfd, SIOCGIFCONF, &ifconf);
    //    printf("%s\n",ifconf.ifc_buf);

    //逐个获取Ip地址
    //结构体指针ifreq指向buf，即ifconf.ifc_buf
    ifreq = (struct ifreq*)buf;
    int number=ifconf.ifc_len/sizeof(struct ifreq);
    // printf("%d\n",number);

    ifname *ifn=(ifname *)malloc(sizeof(ifname)*(number+1));

    for(i = number; i>0; i--)
    {
        // printf("name = [%s] \n",ifreq->ifr_name);

        int j=number-i;
        ifn[j].if_name=(char *)malloc(sizeof(char)*10);
        strcpy(ifn[j].if_name,ifreq->ifr_name);
        // ifn[j].if_name=ifreq->ifr_name;
        // printf("if_name[%d]:%s\n",j,if_name[j]);

        ifreq++;
    }
    ifn[number].if_name=NULL;


    return ifn;
}

//获取所有网络连接
conlist *kylin_network_get_conlist_info()
{
    int status = system("nmcli connection show > /tmp/conlist.txt");
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection show' in function 'kylin_network_get_conlist_info' failed");}
    char *filename="/tmp/conlist.txt";

    FILE *confp;
    int connum=0;
    char ConStrLine[1024];
    if((confp=fopen(filename,"r"))==NULL)
    {
        printf("error!");

    }
    fgets(ConStrLine,1024,confp);
    while(!feof(confp))
    {
        fgets(ConStrLine,1024,confp);
        connum++;
    }
    // printf("%d\n",connum);
    fclose(confp);
    conlist *clist=(conlist *)malloc(sizeof(conlist)*connum);

    int count=0;
    FILE *fp;
    char StrLine[1024];
    if((fp=fopen(filename,"r"))==NULL)
    {
        printf("error!");

    }
    fgets(StrLine,1024,fp);
    while(!feof(fp))
    {
        if(count==connum-1)break;

        fgets(StrLine,1024,fp);

        char *index=StrLine;
        char conname[100];

        //截取连接名称
        int num=0;
        for(index;*index!='\n';index++)
        {
            if(*index==' ')
            {
                if(*(index+1)==' ')
                    break;
            }
            num++;
        }

        // printf("连接名称长度：%d\n",num);
        clist[count].con_name=(char *)malloc(sizeof(char)*(num+1));
        strncpy(conname,StrLine,num+1);
        conname[num]='\0';
        strncpy(clist[count].con_name,conname,num+1);
        // printf("%s\n",clist[count].con_name);

        //截取连接类型
        char type[100];
        for(index;*index!='\n';index++)
        {
            if(*index==' ')
            {
                if(*(index+1)=' ')
                    if(*(index+2)!=' ')
                        break;

            }
        }
        char *index1=index+2;
        for(index1;*index1!='\n';index1++)
        {
            if(*index1==' ')
            {
                if(*(index1+1)==' ')
                    break;
            }
        }
        int num1=0;
        char *index2=index1+2;
        for(index2;*index2!='\n';index2++)
        {
            if(*index2==' ')break;
            num1++;
        }
        clist[count].type=(char *)malloc(sizeof(char)*(num1+1));
        strncpy(type,index1+2,num1+1);
        type[num1]='\0';
        strncpy(clist[count].type,type,num1+1);
        // printf("%s\n",clist[count].type);
        count++;
    }
    fclose(fp);

    clist[count].con_name=NULL;
    clist[count].type=NULL;

    return clist;
}

//获取当前活动网络连接
activecon *kylin_network_get_activecon_info()
{
    struct passwd *pwd;
    pwd = getpwuid(getuid());
    char *name = pwd->pw_name;
    char *tmpPrefix = "/tmp/kylin-nm-activecon-";
    char *chr = "nmcli connection show -active > ";

    char *cmd;
    asprintf(&cmd, "%s%s%s", chr, tmpPrefix, name);
    char *path;
    asprintf(&path, "%s%s", tmpPrefix, name);
    int status = system(cmd);
    //int status = system("nmcli connection show -active > /tmp/activecon.txt");
    if (status != 0)
        syslog(LOG_ERR, "execute 'nmcli connection show -active' in function 'kylin_network_get_activecon_info' failed");
    free(cmd);

    char *filename = path;

    FILE *activefp;
    int activenum=0;
    char activeStrLine[1024];
    if((activefp=fopen(filename,"r"))==NULL)
    {
        printf("error!");
    }
    fgets(activeStrLine,1024,activefp);
    while(!feof(activefp))
    {
        fgets(activeStrLine,1024,activefp);
        activenum++;
    }
    // printf("%d\n",activenum);
    fclose(activefp);
    activecon *activelist=(activecon *)malloc(sizeof(activecon)*activenum);

    int count=0;
    FILE *fp;
    char StrLine[1024];
    if((fp=fopen(filename,"r"))==NULL)
    {
        printf("error!");

    }
    free(path);

    fgets(StrLine,1024,fp);
    while(!feof(fp))
    {
        if(count==activenum-1)break;

        fgets(StrLine,1024,fp);

        char *index=StrLine;
        char conname[100];

        //截取连接名称
        int num=0;
        for(index;*index!='\n';index++)
        {
            if(*index==' ')
            {
                if(*(index+1)==' ')
                    break;
            }
            num++;
        }

        // printf("连接名称长度：%d\n",num);
        activelist[count].con_name=(char *)malloc(sizeof(char)*(num+1));
        strncpy(conname,StrLine,num+1);
        conname[num]='\0';
        strncpy(activelist[count].con_name,conname,num+1);
        // printf("%s\n",activelist[count].con_name);

        //截取连接类型
        char type[100];
        for(index;*index!='\n';index++)
        {
            if(*index==' ')
            {
                if(*(index+1)=' ')
                    if(*(index+2)!=' ')
                        break;

            }
        }
        char *index1=index+2;
        for(index1;*index1!='\n';index1++)
        {
            if(*index1==' ')
            {
                if(*(index1+1)==' ')
                    break;
            }
        }
        int num1=0;
        char *index2=index1+2;
        for(index2;*index2!='\n';index2++)
        {
            if(*index2==' ')break;
            num1++;
        }
        activelist[count].type=(char *)malloc(sizeof(char)*(num1+1));
        strncpy(type,index1+2,num1+1);
        type[num1]='\0';
        strncpy(activelist[count].type,type,num1+1);
        // printf("%s\n",activelist[count].type);

        //截取连接所属设备
        char *index3=index2;
        char dev[100];
        for(index3;*index3!='\n';index3++)
        {
            if(*index3==' ')
            {
                if(*(index3+1)!=' ')
                    break;
            }
        }
        int num2=0;
        char *index4=index3+1;
        for(index4;*index4!='\n';index4++)
        {
            if(*index4==' ')break;
            num2++;
        }
        activelist[count].dev=(char *)malloc(sizeof(char)*(num2+1));
        strncpy(dev,index3+1,num2+1);
        dev[num2]='\0';
        strncpy(activelist[count].dev,dev,num2+1);
        // printf("%s\n",activelist[count].dev);
        count++;
    }
    fclose(fp);

    activelist[count].con_name=NULL;
    activelist[count].type=NULL;
    activelist[count].dev=NULL;

    return activelist;
}

//创建新的以太网连接
void kylin_network_create_new_ethernet(char *con_name,char *if_name)
{
    char str[100];
    char *net_type="ethernet";
    sprintf(str,"nmcli connection add con-name %s ifname %s type %s",con_name,if_name,net_type);
    int status = system(str);
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection add con-name' in function 'kylin_network_create_new_ethernet' failed");}
}

// 创建新的wifi连接配置
void kylin_network_create_new_wifi(char *con_name, char *if_name)
{
    char str[200];
    sprintf(str, "nmcli connection add con-name '%s' ifname '%s' type wifi ssid '%s'",
            con_name, if_name, con_name);
    int status = system(str);
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection add con-name' in function 'kylin_network_create_new_wifi' failed");}
}

//删除以太网连接
void kylin_network_del_ethernet_con(char *con_name)
{
    char str[100];
    sprintf(str,"nmcli connection delete %s",con_name);
    int status = system(str);
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection delete' in function 'kylin_network_del_ethernet_con' failed");}
}

//设置动态分配ip
void kylin_network_set_automethod(char *con_name)
{
    char str[100];
    char *automethod="auto";
    sprintf(str,"nmcli connection modify '%s' ipv4.method %s",con_name,automethod);
    int status = system(str);
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection modify' in function 'kylin_network_set_automethod' failed");}
}

//设置手动分配ip
void kylin_network_set_manualmethod(char *con_name,char *ip)
{
    char str[100];
    char *method="manual";
    sprintf(str,"nmcli connection modify '%s' ipv4.method %s ipv4.address %s",con_name,method,ip);
    int status = system(str);
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection modify' in function 'kylin_network_set_manualmethod' failed");}
}

// 设置手动分配all
void kylin_network_set_manualall(char *con_name, char *addr, char *mask, char *gateway, char *dns){
    char str[200];
    sprintf(str, "nmcli connection modify '%s' ipv4.method manual ipv4.address %s/%s ipv4.gateway %s ipv4.dns %s",
            con_name, addr, mask, gateway, dns);
    int status = system(str);
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection modify' in function 'kylin_network_set_manualall' failed");}
}

//设置是否自动连接
void kylin_network_set_autoconnect(char *con_name,bool autocon)
{
    char str[100];
    if(autocon==false)
    {
        char *ac="no";
        sprintf(str,"nmcli connection modify %s connection.autoconnect %s",con_name,ac);
    }
    else{
        char *ac="yes";
        sprintf(str,"nmcli connection modify %s connection.autoconnect %s",con_name,ac);
    }
    int status = system(str);
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection modify' in function 'kylin_network_set_autoconnect' failed");}
}

//修改ip
void kylin_network_mod_ip(char *con_name,char *ip)
{
    char str[100];
    sprintf(str,"nmcli connection modify %s ipv4.address %s",con_name,ip);
    int status = system(str);
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection modify' in function 'kylin_network_mod_ip' failed");}
}

//修改网关
void kylin_network_mod_gateway(char *con_name,char *gw)
{
    char str[100];
    sprintf(str,"nmcli connection modify %s ipv4.gateway %s",con_name,gw);
    int status = system(str);
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection modify' in function 'kylin_network_mod_gateway' failed");}
}

//修改dns
void kylin_network_mod_dns(char *con_name,char *dns)
{
    char str[100];
    sprintf(str,"nmcli connection modify %s ipv4.dns %s",con_name,dns);
    int status = system(str);
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection modify' in function 'kylin_network_mod_dns' failed");}
}

//连接以太网
void kylin_network_set_con_up(char *con_name)
{
    char str[100];
    sprintf(str,"nmcli connection up '%s'",con_name);
    int status = system(str);
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection up' in function 'kylin_network_set_con_up' failed");}
}

//断开以太网
void kylin_network_set_con_down(char *con_name)
{
    char str[100];
    sprintf(str,"nmcli connection down '%s'",con_name);
    int status = system(str);
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli connection down' in function 'kylin_network_set_con_down' failed");}
}

//连接wifi
void kylin_network_set_wifi_up(char *con_name,char *passwd)
{
    char str[100];
    sprintf(str,"export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli device wifi connect '%s' password '%s'", con_name,passwd);
    int status = system(str);
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli device wifi connect' in function 'kylin_network_set_wifi_up' failed");}
}

//断开wifi连接
void kylin_network_set_wifi_down(char *if_name)
{
    char str[100];
    sprintf(str,"nmcli device disconnect '%s'",if_name);
    int status = system(str);
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli device disconnect' in function 'kylin_network_set_wifi_down' failed");}
}

//获取wifi列表信息
wifilist *kylin_network_get_wifilist_info()
{
    int status = system("nmcli device wifi > /tmp/wflist.txt");
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli device wifi' in function 'kylin_network_get_wifilist_info' failed");}
    char *filename="/tmp/wflist.txt";

    FILE *wffp;
    int wfnum=0;
    char WfStrLine[1024];
    if((wffp=fopen(filename,"r"))==NULL)
    {
        printf("error!");

    }
    fgets(WfStrLine,1024,wffp);
    while(!feof(wffp))
    {
        fgets(WfStrLine,1024,wffp);
        wfnum++;
    }
    // printf("wifi数量：%d\n",wfnum);
    fclose(wffp);
    //wifi实际数量是wfnum-1
    wifilist *wflist=(wifilist *)malloc(sizeof(wifilist)*wfnum);

    int count=0;
    FILE *fp;
    char StrLine[1024];
    if((fp=fopen(filename,"r"))==NULL)
    {
        printf("error!");

    }
    fgets(StrLine,1024,fp);
    while(!feof(fp))
    {
        if(count==wfnum-1)break;

        fgets(StrLine,1024,fp);

        // printf("%s\n",StrLine+3);
        char *index=StrLine+3;

        //截取ssid
        char *str1="Infra";
        int num=0;
        for(index;strcmp(index+5,"\n")!=0;index++)
        {
            int result=strncmp(index,str1,5);
            if(result==0)
            {
                // printf("%s\n",index);
                break;

            }
            num++;

        }
        char ssid[100];
        char *ssidindex=index-1;
        int ssidnum=0;
        for(ssidindex;*ssidindex==' ';ssidindex--)ssidnum++;
        // printf("空格数量：%d\n",ssidnum);
        // if(ssidnum==1)
        strncpy(ssid,StrLine+3,num-1);
        ssid[num-ssidnum]='\0';
        // printf("-6666--%s---\n",ssid);

        wflist[count].ssid=(char *)malloc(sizeof(char)*(num-ssidnum));
        strncpy(wflist[count].ssid,ssid,num-ssidnum+1);
        // printf("第%d个：%s ",count,wflist[count].ssid);


        //截取信号强度
        char *str2="Mbit/s";
        for(index;strcmp(index+6,"\n")!=0;index++)
        {
            int result=strncmp(index,str2,6);
            if(result==0)
            {
                // printf("%s\n",index);
                break;
            }
        }

        char signal[10];
        char *signalindex=index+8;
        int signalnum=0;
        for(signalindex;*signalindex!=' ';signalindex++)signalnum++;
        strncpy(signal,index+8,signalnum);
        signal[signalnum]='\0';
        // printf("-7777--%s---\n",signal);

        wflist[count].signal=atoi(signal);
        // printf("%d ",wflist[count].signal);

         //截取安全性
        char *str3="WPA";
        for(index;strcmp(index+3,"\n")!=0;index++)
        {
            int result=strncmp(index,str3,3);
            if(result==0)
            {
                // printf("%s\n",index);
                break;
            }
        }

        char safety[20];
        char *safetyindex=index;
        int safetynum=0;
        for(safetyindex;strcmp(safetyindex+2,"\n")!=0;safetyindex++)
        {
            int result=strncmp(safetyindex,"  ",2);
            if(result==0)break;
            safetynum++;
        }
        strncpy(safety,index,safetynum+1);
        safety[safetynum+1]='\0';
        // printf("-8888--%s---\n",safety);

        wflist[count].safety=(char *)malloc(sizeof(char)*(safetynum+1));
        strncpy(wflist[count].safety,safety,safetynum+2);
        // printf("%s\n",wflist[count].safety);

        count++;

    }

    fclose(fp);

    wflist[count].ssid=NULL;
    wflist[count].signal=0;
    wflist[count].safety=NULL;

    return wflist;
}

//启用联网
void kylin_network_enable_networking()
{
    int status = system("nmcli networking on");
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli networking on' in function 'kylin_network_enable_networking' failed");}
}

//禁用联网
void kylin_network_disable_networking()
{
    int status = system("nmcli networking off");
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli networking off' in function 'kylin_network_disable_networking' failed");}
}

//启用wifi
void kylin_network_enable_wifi()
{
    int status = system("nmcli radio wifi on;sleep 3");
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli radio wifi on' in function 'kylin_network_enable_wifi' failed");}
}

//禁用wifi
void kylin_network_disable_wifi()
{
    int status = system("nmcli radio wifi off;sleep 2");
    if (status != 0){ syslog(LOG_ERR, "execute 'nmcli radio wifi off' in function 'kylin_network_disable_wifi' failed");}
}

//获取ip地址
int kylin_network_get_ipaddr(char *if_name,char *ipaddr)
{
    int sock_ip;
    struct sockaddr_in sin_ip;
    struct ifreq ifr_ip;
    if((sock_ip=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("sockket error");
        return -1;
    }

    strcpy(ifr_ip.ifr_name,if_name);

    if(ioctl(sock_ip,SIOCGIFADDR,&ifr_ip)<0)//直接获取ip地址
    {
        perror("Not setup interface\n");
        return -1;
    }

    memcpy(&sin_ip,&ifr_ip.ifr_addr,sizeof(sin_ip));
    strcpy(ipaddr,inet_ntoa(sin_ip.sin_addr));//#include <arpa/inet.h>
    close(sock_ip);
    return 0;

}

//获取广播地址
int kylin_network_get_brdaddr(char *if_name,char *brdaddr)
{
    int sock_brdaddr;
    struct sockaddr_in sin_brd;
    struct ifreq ifr_brd;
    if((sock_brdaddr=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("sockket error");
        return -1;
    }

    strcpy(ifr_brd.ifr_name,if_name);

    if(ioctl(sock_brdaddr,SIOCGIFBRDADDR,&ifr_brd)<0)//直接获取广播地址
    {
        perror("Not setup interface\n");
        return -1;
    }

    memcpy(&sin_brd,&ifr_brd.ifr_broadaddr,sizeof(sin_brd));
    strcpy(brdaddr,inet_ntoa(sin_brd.sin_addr));
    close(sock_brdaddr);
    return 0;

}

//获取子网掩码
int kylin_network_get_netmask(char *if_name,char *netmask)
{
    int sock_mask;
    struct sockaddr_in sin_netmask;
    struct ifreq ifr_mask;
    if((sock_mask=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("sockket error");
        return -1;
    }

    strcpy(ifr_mask.ifr_name,if_name);

    if(ioctl(sock_mask,SIOCGIFNETMASK,&ifr_mask)<0)//直接获取子网掩码
    {
        perror("Not setup interface\n");
        return -1;
    }

    memcpy(&sin_netmask,&ifr_mask.ifr_netmask,sizeof(sin_netmask));
    strcpy(netmask,inet_ntoa(sin_netmask.sin_addr));
    close(sock_mask);
    return 0;

}

//获取MAC
int kylin_network_get_mac(char *if_name,char *macaddr)
{
    int sock_mac;
    struct ifreq ifr_mac;
    struct sockaddr_in sin_mac;
    struct ether_addr ethaddr;//#include <net/ethernet.h>

    sock_mac=socket(AF_INET,SOCK_STREAM,0);

    strcpy(ifr_mac.ifr_name,if_name);

    if(-1==ioctl(sock_mac,SIOCGIFHWADDR,&ifr_mac))
    {
        perror("Not setup interface\n");
        return -1;
    }
    memcpy(&ethaddr,&ifr_mac.ifr_hwaddr.sa_data,sizeof(ethaddr));
    strcpy(macaddr,ether_ntoa(&ethaddr));//#include <netinet/ether.h>

    // strcpy(macaddr,ether_ntoa((struct ether_addr*)ifr_mac.ifr_hwaddr.sa_data));

    close(sock_mac);
    return 0;

}

//获取MTU
int kylin_network_get_mtu(char *if_name)
{
    int sock_mtu;
//    struct sockaddr_in sin_ip;
    struct ifreq ifr_MTU;
    if((sock_mtu=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        perror("sockket error");
        return -1;
    }

    strcpy(ifr_MTU.ifr_name,if_name);

    if(ioctl(sock_mtu,SIOCGIFMTU,&ifr_MTU))
    {
        perror("Not setup interface\n");
        return -1;
    }

    int mtu=ifr_MTU.ifr_mtu;
    // printf("%d\n",ifr_MTU.ifr_mtu);
    // printf("%d\n",mtu);

    close(sock_mtu);
    return mtu;

}

static void getBuf(char *bufstr)
{
    //O_RDONLY以只读方式打开文件
    int fd=open("/proc/net/dev", O_RDONLY);
    if(-1==fd)
    {
        printf("/proc/net/dev not exists!\n");
    }

    char buf[1024*2];
    //将读写位置移到文件开头
    lseek(fd,0,SEEK_SET);
    int nBytes=read(fd,buf,sizeof(buf)-1);
    if(-1==nBytes)
    {
        perror("read error");
        close(fd);
    }

    buf[nBytes]='\0';
    strcpy(bufstr,buf);
}

//总计上传下载数据量
long *kylin_network_get_bytes(char *if_name)
{
    char buf[1024*2];
    getBuf(buf);

    //返回第一次指向if_name位置的指针
    char *pDev=strstr(buf,if_name);
    if(NULL==pDev)
    {
        printf("don't find dev %s\n",if_name);
        return NULL;
    }

    char *p;
    char *value;
    int i=0;
    static long rtbyt[2];
    /*去除空格，制表符，换行符等不需要的字段*/
    for (p = strtok(pDev, " \t\r\n"); p; p = strtok(NULL, " \t\r\n"))
    {
        i++;
        value = (char*)malloc(20);
        strcpy(value, p);
        /*得到的字符串中的第二个字段是接收流量*/
        if(i == 2)
        {
            rtbyt[0] = atol(value);
        }
        /*得到的字符串中的第十个字段是发送流量*/
        if(i == 10)
        {
            rtbyt[1] = atol(value);
            break;
        }
        free(value);
    }
    return rtbyt;
}

//总计上传下载数据包
long *kylin_network_get_packets(char *if_name)
{
    char buf[1024*2];
    getBuf(buf);

    //返回第一次指向if_name位置的指针
    char *pDev=strstr(buf,if_name);
    if(NULL==pDev)
    {
        printf("don't find dev %s\n",if_name);
        return NULL;
    }

    char *p;
    char *value;
    int i=0;
    static long rtpkt[2];
    /*去除空格，制表符，换行符等不需要的字段*/
    for (p = strtok(pDev, " \t\r\n"); p; p = strtok(NULL, " \t\r\n"))
    {
        i++;
        value = (char*)malloc(20);
        strcpy(value, p);
        /*得到的字符串中的第三个字段是接收流量*/
        if(i == 3)
        {
            rtpkt[0] = atol(value);
        }
        /*得到的字符串中的第十一个字段是发送流量*/
        if(i == 11)
        {
            rtpkt[1] = atol(value);
            break;
        }
        free(value);
    }
    return rtpkt;
}

//总计上传下载错误数据包数量
long *kylin_network_get_errs(char *if_name)
{
    char buf[1024*2];
    getBuf(buf);

    //返回第一次指向if_name位置的指针
    char *pDev=strstr(buf,if_name);
    if(NULL==pDev)
    {
        printf("don't find dev %s\n",if_name);
        return NULL;
    }

    char *p;
    char *value;
    int i=0;
    static long rterrs[2];
    /*去除空格，制表符，换行符等不需要的字段*/
    for (p = strtok(pDev, " \t\r\n"); p; p = strtok(NULL, " \t\r\n"))
    {
        i++;
        value = (char*)malloc(20);
        strcpy(value, p);
        /*得到的字符串中的第四个字段是接收流量*/
        if(i == 4)
        {
            rterrs[0] = atol(value);
        }
        /*得到的字符串中的第十二个字段是发送流量*/
        if(i == 12)
        {
            rterrs[1] = atol(value);
            break;
        }
        free(value);
    }
    return rterrs;
}

//总计上传下载丢弃数据包数量
long *kylin_network_get_drop(char *if_name)
{
    char buf[1024*2];
    getBuf(buf);

    //返回第一次指向if_name位置的指针
    char *pDev=strstr(buf,if_name);
    if(NULL==pDev)
    {
        printf("don't find dev %s\n",if_name);
        return NULL;
    }

    char *p;
    char *value;
    int i=0;
    static long rtdrop[2];
    /*去除空格，制表符，换行符等不需要的字段*/
    for (p = strtok(pDev, " \t\r\n"); p; p = strtok(NULL, " \t\r\n"))
    {
        i++;
        value = (char*)malloc(20);
        strcpy(value, p);
        /*得到的字符串中的第五个字段是接收流量*/
        if(i == 5)
        {
            rtdrop[0] = atol(value);
        }
        /*得到的字符串中的第十三个字段是发送流量*/
        if(i == 13)
        {
            rtdrop[1] = atol(value);
            break;
        }
        free(value);
    }
    return rtdrop;
}

//总计上传下载过载数据包数量
long *kylin_network_get_fifo(char *if_name)
{
    char buf[1024*2];
    getBuf(buf);

    //返回第一次指向if_name位置的指针
    char *pDev=strstr(buf,if_name);
    if(NULL==pDev)
    {
        printf("don't find dev %s\n",if_name);
        return NULL;
    }

    char *p;
    char *value;
    int i=0;
    static long rtfifo[2];
    /*去除空格，制表符，换行符等不需要的字段*/
    for (p = strtok(pDev, " \t\r\n"); p; p = strtok(NULL, " \t\r\n"))
    {
        i++;
        value = (char*)malloc(20);
        strcpy(value, p);
        /*得到的字符串中的第六个字段是接收流量*/
        if(i == 6)
        {
            rtfifo[0] = atol(value);
        }
        /*得到的字符串中的第十四个字段是发送流量*/
        if(i == 14)
        {
            rtfifo[1] = atol(value);
            break;
        }
        free(value);
    }
    return rtfifo;
}
