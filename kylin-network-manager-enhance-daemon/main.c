/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "public.h"

DBusConnection *BUS = NULL;
int MONITOR_FLAG = -1;
pthread_t file_tid;
pthread_t con_tid;

const struct DBusObjectPathVTable server_vtable = {.message_function = server_message_handler};
const struct DBusObjectPathVTable server_vtable_dns = {.message_function = server_message_handler_dns};
int CHILDPRO_LEN = 0;   //子进程列表实际长度
#define MAX_CHILD_PROCESS_CNT  20
pid_t child_process[MAX_CHILD_PROCESS_CNT]; //子进程列表
int kill_enable_service()
{
    int ret = 0;
    int i   = 0;
    if(CHILDPRO_LEN<1)  return 0;

    for(i=0;i<CHILDPRO_LEN;i++)
    {
        ret = kill(child_process[i],SIGTERM);
       syslog(LOG_INFO,"进程ID=%d,主进程资源回收返回值:%d,ERR INFO=%s",child_process[i],ret,strerror(ret));
    }
    return 0;
}
void resource_recovery(int sig)
{
    syslog(LOG_INFO,"接受到终止请求，回收所有资源");
    kill_enable_service();
    exit(sig);
}


int main()
{
    DBusConnection *conn;
    DBusError err;
    int rv;
    void * in_data;


    syslog(LOG_INFO,"服务管理进程启动,开始拉起默认服务");
   // raise_all_enable_service(SERVICE_CONFIG_FILE);

    syslog(LOG_INFO,"主进程开始注册信号监听，接收异常退出!");
    signal(SIGTERM,resource_recovery);
    signal(SIGKILL,resource_recovery);
    signal(SIGINT,resource_recovery);
    

    syslog(LOG_INFO,"拉起DBUS服务以注册IPC渠道");

    dbus_error_init(&err);

    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (!conn)
    {
        goto fail;
    }
    BUS=conn;
    rv = dbus_bus_request_name(conn,
                               NETWORK_ENHANCEMENT_OPTIMIZATION_DBUS_NAME,
                               DBUS_NAME_FLAG_REPLACE_EXISTING, //one of the standard flag
                               &err);

    if (rv != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
    {
        syslog(LOG_INFO,"fail to request a dbus name!,err name=%s,err msg = %s\n",err.name,err.message);
        goto fail;
    }

    if (!dbus_connection_try_register_object_path(conn,
                                                  NETWORK_ENHANCEMENT_OPTIMIZATION_DBUS_OBJECT_PATH,
                                                  &server_vtable,
                                                  in_data,
                                                  &err))
    {

        syslog(LOG_INFO,"fail to register a object path!,err name=%s,err msg = %s\n",err.name,err.message);

        goto fail;
    }
    if (!dbus_connection_try_register_object_path(conn,
                                                  DNS_OPTIMIZATION_DBUS_OBJECT_PATH,
                                                  &server_vtable_dns,
                                                  in_data,
                                                  &err))
    {

        syslog(LOG_INFO,"fail to register a object path!,err name=%s,err msg = %s\n",err.name,err.message);

        goto fail;
    }
    syslog(LOG_INFO,"DBUS服务启动成功，进入DBUS消息循环监听状态...");
    
    if(!pthread_create(&con_tid,NULL,con_monitor,BUS))
    {
       syslog(LOG_INFO,"con monitor pthread_create");
    }
    if(!pthread_create(&file_tid,NULL,file_monitor,NULL))
    {
       syslog(LOG_INFO,"file monitor pthread_create");
    }
    mainloop = g_main_loop_new(NULL, false);

    dbus_connection_setup_with_g_main(conn, NULL);

    g_main_loop_run(mainloop);

    return FAIL;

fail:
    dbus_error_free(&err);
    return PASS;
}

