
/******************************************************************************
*版权所有(C) 2024 麒麟软件有限公司
*文 件 名 :kylin_herdron_log.c
*作	 者 :
*内容摘要 :
*创建日期 :2024年11月21日
*修改记录1:
*修 改 人 :
*修改日期 :
*修改内容 :
******************************************************************************/

#include <errno.h>  
#include <string.h>  
#include <json-glib/json-glib.h>
#include <glib.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/time.h>
#ifdef __cplusplus
extern "C" {
#endif

#include "kylin_common.h"
#include "kylin_hedron_log.h"

#define KYLIN_HEDRON_DBUS_SERVICE  "org.log.sys_transmit"
#define KYLIN_HEDRON_DBUS_PATH        "/org/log/sys_transmit"
#define KYLIN_HEDRON_DBUS_INTERFACE  "org.log.transmit"
#define KYLIN_HEDRON_DBUS_METHOD       "log_transmit"

#define KYLIN_HEDRON_SENDER_NAME "kylin-nm-rule" 
#define MAX_LENGTH (256)
#define HEDRON_TIMEOUT_MS (5000)

/*{"created_time":"2023-08-31","hostname":"snowsi","ip":"192.168.1.101","name":"network_monitor_log","level":"error","message":"执行成功：
IPV4/IPV6/DNS/网络代理变化 源IP、源DNS、源网络代理：10.41.20.255 新ip 21.20.25.255"}*/
typedef struct ST_HedronLogMessage
{
	char *createTime;
	char *hostName;
	char *IP;
	char *name;
	char *level;
	char *message;
}st_HedronLogMessage;

int hedronGetDataTime(char*backTime)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);//获取1970-1-1到现在的时间结果保存到tv中
    unsigned long long sec=tv.tv_sec;
    struct tm cur_tm;//保存转换后的时间结果
    localtime_r((time_t*)&sec,&cur_tm);
    snprintf(backTime,MAX_LENGTH,"%d-%02d-%02d %02d:%02d:%02d",cur_tm.tm_year+1900,cur_tm.tm_mon+1,cur_tm.tm_mday,cur_tm.tm_hour,cur_tm.tm_min,cur_tm.tm_sec);
    return 0;
}

int hedronGetDataHostname(char*backHostName)
{
    const char sn_str[] = "who | grep -v root | awk '{printf $1;}'";
	char cmdBackStr[MAX_LENGTH]={0};
    int str_result = 0; 
    str_result = kylinExeSystemCmd(sn_str,cmdBackStr);
    if (str_result)
    {
        KYLIN_COMMON_LOG(_LOGL_INFO, "Get uid error");
		return str_result;
    }
    else
    {
        KYLIN_COMMON_LOG(_LOGL_INFO, "Get uid information:%s", cmdBackStr);
    }

    snprintf(backHostName, MAX_LENGTH, "%s", cmdBackStr);
    return COMMON_SUCCESS;
}

int hedronGetDataIp(char*backIp)
{
    struct ifaddrs* ifAddrStruct = NULL;
    void * tmpAddrPtr = NULL;

    if (getifaddrs(&ifAddrStruct) != 0)
    {
        KYLIN_COMMON_LOG(_LOGL_ERR, "Somthing is Wrong!");
        return -1;
    }

    struct ifaddrs* iter = ifAddrStruct;
    while (iter != NULL) 
	{
        if (iter->ifa_addr != NULL)
        {
            if (iter->ifa_addr->sa_family == AF_INET) { 
                tmpAddrPtr = &((struct sockaddr_in *)iter->ifa_addr)->sin_addr;
                char addressBuffer[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
                if (strlen(backIp) + strlen(addressBuffer) < MAX_LENGTH - 1)
                {
                    strcpy(backIp, addressBuffer);
                }
                else
                {
                    syslog(LOG_INFO, "Too many ips!\n");
                    break;
                }
            }
        }
        iter = iter->ifa_next;
    }

    freeifaddrs(ifAddrStruct);
    return 0;
}

int hedronMessageFree(st_HedronLogMessage *pHedronLogMessage)
{
	CHECK_POINTER_NULL(pHedronLogMessage);
	if(pHedronLogMessage->createTime) g_free(pHedronLogMessage->createTime);
	if(pHedronLogMessage->hostName) g_free(pHedronLogMessage->hostName);
	if(pHedronLogMessage->IP) g_free(pHedronLogMessage->IP);
	if(pHedronLogMessage->name) g_free(pHedronLogMessage->name);
	if(pHedronLogMessage->level) g_free(pHedronLogMessage->level);
	if(pHedronLogMessage->message) g_free(pHedronLogMessage->message);
	g_free(pHedronLogMessage);
	return COMMON_SUCCESS;
}


st_HedronLogMessage* kylinLoadHedronMessage(char *level,char *message)
{
	if(!level || level[0]==0) 
	{
		return NULL;
	}
	if(!message || message[0]==0) 
	{
		return NULL;
	}
	st_HedronLogMessage *pHedronLogMessage=(st_HedronLogMessage*)g_malloc0(sizeof(st_HedronLogMessage));;
	pHedronLogMessage->createTime=g_malloc0(MAX_LENGTH);
	hedronGetDataTime(pHedronLogMessage->createTime);
	pHedronLogMessage->hostName=g_malloc0(MAX_LENGTH);
    hedronGetDataHostname(pHedronLogMessage->hostName);
	pHedronLogMessage->IP=g_malloc0(MAX_LENGTH);
	hedronGetDataIp(pHedronLogMessage->IP);
	pHedronLogMessage->name=g_strdup(KYLIN_HEDRON_SENDER_NAME);
	pHedronLogMessage->level=g_strdup(level);
	pHedronLogMessage->message=g_strdup(message);
	return pHedronLogMessage;
}

char *hedronDataTransformStr(st_HedronLogMessage *pHedronLogMessage)
{
    JsonBuilder *builder = json_builder_new();

    json_builder_begin_object(builder);
    json_builder_set_member_name(builder, "time");
    json_builder_add_string_value(builder, pHedronLogMessage->createTime);

    json_builder_set_member_name(builder, "hostname");
    json_builder_add_string_value(builder, pHedronLogMessage->hostName);

    json_builder_set_member_name(builder, "ip");
    json_builder_add_string_value(builder,pHedronLogMessage->IP);

    json_builder_set_member_name(builder, "name");
    json_builder_add_string_value(builder,pHedronLogMessage->name);

    json_builder_set_member_name(builder, "lv");
    json_builder_add_string_value(builder, pHedronLogMessage->level);

    json_builder_set_member_name(builder, "message");
    json_builder_add_string_value(builder,pHedronLogMessage->message);

    json_builder_end_object(builder);

    JsonNode *node = json_builder_get_root(builder);
    g_object_unref( builder);
    JsonGenerator *generator = json_generator_new();
    json_generator_set_root(generator, node);
    char *data = json_generator_to_data(generator, NULL);

    json_node_free(node);
    g_object_unref(generator);

    return data;
}

int kylinHedronDbusSendLog(char * sendInfo)//同步调用
{

    GDBusProxy *proxy=NULL;
    GVariant *result;
    GError *error = NULL;

    const char *res = NULL;


    proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                                           G_DBUS_PROXY_FLAGS_NONE,
                                           NULL,
                                           KYLIN_HEDRON_DBUS_SERVICE,
                                           KYLIN_HEDRON_DBUS_PATH,
                                           KYLIN_HEDRON_DBUS_INTERFACE,
                                           NULL, &error);

    CHECK_POINTER_NULL(proxy);
    result = g_dbus_proxy_call_sync (proxy,
                                  KYLIN_HEDRON_DBUS_METHOD,
                                  g_variant_new ("(s)", sendInfo),
                                  G_DBUS_CALL_FLAGS_NONE, HEDRON_TIMEOUT_MS,
                                  NULL, &error);
    if (!result) {
        g_dbus_error_strip_remote_error (error);
        KYLIN_COMMON_LOG (_LOGL_ERR,"Error adding connection: %s", error->message);
        g_clear_error (&error);
        goto out;
    }

    if(g_variant_is_of_type(result, G_VARIANT_TYPE_STRING))
    {
        g_variant_get(result, "(&s)",&res );
        KYLIN_COMMON_LOG(_LOGL_DEBUG, "recv hedron %s!",res);
    }

out:
    if (result) g_variant_unref (result);
    g_object_unref (proxy);

    return COMMON_SUCCESS;
}


int kylinSendHedronMessage(char *level,char *message)
{	
	int ret=0;
	st_HedronLogMessage* pHedronLogMessage=NULL;
	char *sendInfo=NULL;
    //KYLIN_COMMON_LOG(_LOGL_INFO, "send level:%s message %s!",level,message);//mqtest
	pHedronLogMessage=kylinLoadHedronMessage(level,message);
	if(!pHedronLogMessage)
	{
		KYLIN_COMMON_LOG(_LOGL_ERR, "load hedron message failed!");
		return COMMON_NULL_POINTER;
	}
	sendInfo=hedronDataTransformStr(pHedronLogMessage);
	hedronMessageFree(pHedronLogMessage);
    ret=kylinHedronDbusSendLog(sendInfo);
    KYLIN_COMMON_LOG(_LOGL_INFO, "send %s!",sendInfo);
	g_free(sendInfo);
	return ret;
}



#ifdef __cplusplus
}
#endif

