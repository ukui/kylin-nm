/*
 * @Author: 武丹 wudan@kylinos.cn
 * @Date: 2023-04-17 10:27:43
 * @LastEditors: 武丹 wudan@kylinos.cn
 * @LastEditTime: 2023-04-17 17:52:59
 * @FilePath: /dns-en/dbus-server.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NET_ENHANCE_DBUS_SER__
#define __NET_ENHANCE_DBUS_SER__

#include <string.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h> /* for glib main loop */
#include <stdbool.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>


#define NETWORK_ENHANCEMENT_OPTIMIZATION_DBUS_NAME            "com.kylin.network.enhancement.optimization"
#define NETWORK_ENHANCEMENT_OPTIMIZATION_DBUS_OBJECT_PATH     "/com/kylin/network/enhancement/optimization"
#define NETWORK_ENHANCEMENT_OPTIMIZATION_INTERFACE            "com.kylin.network.enhancement.optimization"

#define DNS_OPTIMIZATION_DBUS_OBJECT_PATH                 "/com/kylin/network/enhancement/optimization/DNS"
#define DNS_OPTIMIZATION_INTERFACE_PATH                   "com.kylin.network.enhancement.optimization.DNS"

#define METHOD_LST_INTROSPECT   "Introspect"    
#define METHOD_LIST_ENANCEMENT_SETTING "GetSettings"     //列出网络增强的选项
#define METHOD_LIST_SET_EXTRA_DNS "SetExtraDns"         //设置extardns
#define METHOD_LIST_GET_ALL_EXTRA_DNS "GetAllExtraDns"        //获取ALLExtradns
#define METHOD_LIST_GET_EXTRA_DNS "GetExtraDns"        //获取extradns
#define METHOD_LIST_SET_EXTRA_DOMAIN "SetExtraDomain" //设置extra的domain
#define METHOD_LIST_SET_EXTRA_SEARCH "SetExtraSearch" // 设置extra的search
#define METHOD_LIST_SET_OPTIONS "SetOptions" //设置extra的options
#define METHOD_QUIT     "Quit"  //自我脱离Dbus服务

extern GMainLoop* mainloop;  


DBusHandlerResult server_message_handler(DBusConnection *conn, DBusMessage *message, void *data);
DBusHandlerResult server_message_handler_dns(DBusConnection *conn, DBusMessage *message, void *data);
void write_options_to_resolv(char *options);
char *get_active_connection_uuid(char *path);
char* get_primaryconnection();
int write_nameserver_to_resolv();
void write_server_to_resolv();
#endif
