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
#include "dns-xml.h"
#include "public.h"
#include "dbus-server.h"

GMainLoop* mainloop = NULL;
int checkEnviron(DBusConnection *conn,int pid);
gboolean isAllowedCaller(DBusConnection *conn, pid_t pid);
int dbus_security_check(DBusConnection* conn, DBusMessage* message,DBusError *error);
static int dbus_security_get_client_pid(DBusConnection* conn, DBusMessage* m, pid_t* pit_out);
#define MAX_FILE_LENGTH 256
const char *whiteList[] = {"/usr/bin/kylin-nm",NULL};

DBusHandlerResult server_message_handler(DBusConnection *conn, DBusMessage *message, void *data)
{
    DBusHandlerResult result;
    DBusMessage *reply = NULL;

    DBusError err;
    bool quit = false;
    syslog(LOG_INFO, "Got D-Bus request: %s.%s on %s,destination is:%s,sender is:%s\n",
            dbus_message_get_interface(message),
            dbus_message_get_member(message),
            dbus_message_get_path(message),
            dbus_message_get_destination(message),
            dbus_message_get_sender(message));

    if (!(reply = dbus_message_new_method_return(message))) //申请内存适配则事先退出
    {
        result = DBUS_HANDLER_RESULT_NEED_MEMORY;
        return result;
    }

    //漫长的调用接口判断，归类调用的功能
    if (dbus_message_is_method_call(message, DBUS_INTERFACE_INTROSPECTABLE, METHOD_LST_INTROSPECT))
    {
        dbus_message_append_args(reply,
                                 DBUS_TYPE_STRING, &server_introspection_xml,
                                 DBUS_TYPE_INVALID);   
    }
    else if (dbus_message_is_method_call(message, NETWORK_ENHANCEMENT_OPTIMIZATION_INTERFACE, METHOD_QUIT))
    {
        g_main_loop_quit(mainloop); //用于退出主循环
     }
    // else if (dbus_message_is_method_call(message, NETWORK_ENHANCEMENT_OPTIMIZATION_INTERFACE, METHOD_LIST_ENANCEMENT_SETTING))
    // {
    //     // if(!dbus_security_check(conn,message,&err)){
    //     //     goto fail;
    //     // }
    //     syslog(LOG_INFO,"Getsetting");
    // }
    //消息回复、分发
    if (!dbus_connection_send(conn, reply, NULL))
        result = DBUS_HANDLER_RESULT_NEED_MEMORY;

clean:   
    dbus_message_unref(reply);
    return result;
// fail:
//    if (dbus_error_is_set(&err)) {
// 		if (reply)
// 			dbus_message_unref(reply);
// 		reply = dbus_message_new_error(message, err.name, err.message);
// 		dbus_error_free(&err);
// 	}

// 	if (!reply)
// 		return DBUS_HANDLER_RESULT_NEED_MEMORY;

// 	result = DBUS_HANDLER_RESULT_HANDLED;

// 	if (!dbus_connection_send(conn, reply, NULL))
// 		result = DBUS_HANDLER_RESULT_NEED_MEMORY;

// 	dbus_message_unref(reply);

// 	return result;
    
}
DBusHandlerResult server_message_handler_dns(DBusConnection *conn, DBusMessage *message, void *data)
{
    DBusError err;
    DBusHandlerResult result;
    DBusMessage *reply = NULL; //创建消息对象指针

    dbus_error_init(&err);
    syslog(LOG_INFO, "Got D-Bus request: %s.%s on %s,destination is:%s,sender is:%s\n",
            dbus_message_get_interface(message),
            dbus_message_get_member(message),
            dbus_message_get_path(message),
            dbus_message_get_destination(message),
            dbus_message_get_sender(message));
    if (!(reply = dbus_message_new_method_return(message))) //申请内存适配则事先退出(创建返回消息)
    {
        result = DBUS_HANDLER_RESULT_NEED_MEMORY;
        return result;
    }
    //漫长的调用接口判断，归类调用的功能
    if (dbus_message_is_method_call(message, DBUS_INTERFACE_INTROSPECTABLE, METHOD_LST_INTROSPECT))
    {
        dbus_message_append_args(reply,
                                 DBUS_TYPE_STRING, &dns_xml,
                                 DBUS_TYPE_INVALID); //将dns_xml定义为消息
    }
    else if (dbus_message_is_method_call(message, DNS_OPTIMIZATION_INTERFACE_PATH, METHOD_LIST_GET_ALL_EXTRA_DNS))
    {
         if(!dbus_security_check(conn,message,&err)){
             goto fail;
         }
        DBusMessageIter iter;
        char * extra_secondary=NULL,*options=NULL,*domian=NULL,*search=NULL,*con_name=NULL;
        char *result_value = NULL;
        GError *error = NULL;
        if (!dbus_message_get_args(message, &err,
                                   DBUS_TYPE_STRING, &con_name,
                                   DBUS_TYPE_INVALID))
        {
            goto fail;
        }
        dbus_message_iter_init_append(reply, &iter); //在消息中加入参数
        char *file=get_file_path(con_name);
        if (!extara_dns_conf_is_exist(file))
        {
            result_value = "no config file";
            dbus_message_iter_append_basic(&iter,
                                           DBUS_TYPE_STRING,
                                           &result_value);
            goto send;
        }
        char value_secondary[200] = {0};
        char value_options[200] = {0};
        char value_domian[200] = {0};
        char value_search[200]={0};
        GKeyFile *key_file = g_key_file_new();
        if (!g_key_file_load_from_file(key_file, file, G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS, &error))
        {
            if (!g_error_matches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
                g_warning("Error loading key file: %s", error->message);

            if(file)
            {
               free(file);
               file=NULL;
            }
            return false;
        }
        extra_secondary = read_extra_dns_conf(key_file, NAMESERVER);
        options = read_extra_dns_options(key_file,OPTIONS);
        domian = read_extra_dns_domian(key_file,DOMAIN);
        search = read_extra_dns_search(key_file,SEARCH);
        
        snprintf(value_secondary,200,"%s:%s",NAMESERVER,extra_secondary);
        result_value = value_secondary;
        dbus_message_iter_append_basic(&iter,
                                       DBUS_TYPE_STRING,
                                       &result_value);
        snprintf(value_options,200,"%s:%s",OPTIONS,options);
        result_value = value_options;
        dbus_message_iter_append_basic(&iter,
                                       DBUS_TYPE_STRING,
                                       &result_value);
        snprintf(value_domian,200,"%s:%s",DOMAIN,domian);
        result_value = value_domian;
        dbus_message_iter_append_basic(&iter,
                                       DBUS_TYPE_STRING,
                                       &result_value);
        snprintf(value_search,200,"%s:%s",SEARCH,search);
        result_value = value_search;
        dbus_message_iter_append_basic(&iter,
                                       DBUS_TYPE_STRING,
                                       &result_value);                            
        g_key_file_free(key_file);
        free(file);
        free(options);
        goto send;
    }else if (dbus_message_is_method_call(message, DNS_OPTIMIZATION_INTERFACE_PATH, METHOD_LIST_GET_EXTRA_DNS))
    {
         if(!dbus_security_check(conn,message,&err)){
             goto fail;
         }
        DBusMessageIter iter,v,entry_iter,dict;
        char *con_name=NULL,*timeout = NULL, *attempts = NULL, *type = NULL;
        GError *error = NULL;
        if (!dbus_message_get_args(message, &err,
                                   DBUS_TYPE_STRING, &con_name,
                                   DBUS_TYPE_INVALID))
        {
            goto fail;
        }
        dbus_message_iter_init_append(reply, &iter); //在消息中加入参数
        dbus_message_iter_open_container(&iter,DBUS_TYPE_ARRAY,"{sv}", &v);
        
        char *file=get_file_path(con_name);
        if (!extara_dns_conf_is_exist(file))
        {
            dbus_message_iter_close_container(&iter,&v);
            goto send;
        }
        GKeyFile *key_file = g_key_file_new();
        if (!g_key_file_load_from_file(key_file, file, G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS, &error))
        {
            if (!g_error_matches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
                g_warning("Error loading key file: %s", error->message);

            if(file)
            {
               free(file);
               file=NULL;
            }
            return false;
        }
        
        timeout = g_key_file_get_string(key_file, "options", "timeout", &error);
        if(timeout==NULL)
            timeout="";
        char *timeout_s="timeout";
        dbus_message_iter_open_container(&v,DBUS_TYPE_DICT_ENTRY,NULL,&entry_iter);
        dbus_message_iter_append_basic(&entry_iter,DBUS_TYPE_STRING,&timeout_s);
        dbus_message_iter_open_container(&entry_iter,DBUS_TYPE_VARIANT,DBUS_TYPE_STRING_AS_STRING,&dict);
        dbus_message_iter_append_basic(&dict,DBUS_TYPE_STRING,&timeout);
        dbus_message_iter_close_container(&entry_iter,&dict);
        dbus_message_iter_close_container(&v,&entry_iter);

        attempts = g_key_file_get_string(key_file, "options", "attempts", &error);
        if(attempts==NULL)
            attempts="";
        char *attempts_s="attempts";
        dbus_message_iter_open_container(&v,DBUS_TYPE_DICT_ENTRY,NULL,&entry_iter);
        dbus_message_iter_append_basic(&entry_iter,DBUS_TYPE_STRING,&attempts_s);
        dbus_message_iter_open_container(&entry_iter,DBUS_TYPE_VARIANT,DBUS_TYPE_STRING_AS_STRING,&dict);
        dbus_message_iter_append_basic(&dict,DBUS_TYPE_STRING,&attempts);
        dbus_message_iter_close_container(&entry_iter,&dict);
        dbus_message_iter_close_container(&v,&entry_iter);

        type = g_key_file_get_string(key_file, "options", "type", &error);
        if(type==NULL)
            type="";
        char *type_s="type";
        dbus_message_iter_open_container(&v,DBUS_TYPE_DICT_ENTRY,NULL,&entry_iter);
        dbus_message_iter_append_basic(&entry_iter,DBUS_TYPE_STRING,&type_s);
        dbus_message_iter_open_container(&entry_iter,DBUS_TYPE_VARIANT,DBUS_TYPE_STRING_AS_STRING,&dict);
        dbus_message_iter_append_basic(&dict,DBUS_TYPE_STRING,&type);
        dbus_message_iter_close_container(&entry_iter,&dict);
        dbus_message_iter_close_container(&v,&entry_iter);
    
        dbus_message_iter_close_container(&iter,&v);                        
        g_key_file_free(key_file);
        free(file);
        goto send;
    }
    else if (dbus_message_is_method_call(message, DNS_OPTIMIZATION_INTERFACE_PATH, METHOD_LIST_SET_EXTRA_DNS))
    {
         if(!dbus_security_check(conn,message,&err)){
             goto fail;
         }
        DBusMessageIter iter;
        char *con_name,*extradns;
        dbus_bool_t set_result;
        if (!dbus_message_get_args(message, &err, 
                                   DBUS_TYPE_STRING, &con_name,
                                   DBUS_TYPE_STRING, &extradns,
                                   DBUS_TYPE_INVALID))
        {
            goto fail;
        }
        if (con_name !=NULL && set_extra_dns(con_name,extradns))
        {
            set_result = true;
            result=DBUS_HANDLER_RESULT_HANDLED;
            syslog(LOG_INFO,"set extra dns success!/n");            
        }else
        {
            set_result = false;
            result=DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
            syslog(LOG_INFO,"set extra dns failed!/n");
        }
        reply = dbus_message_new_method_return(message);
        dbus_message_iter_init_append(reply, &iter);
        if (!dbus_message_iter_append_basic(&iter, DBUS_TYPE_BOOLEAN, &set_result))
        {
            syslog(LOG_INFO,"Out of Memory!/n");

        }
        goto send;
    }
    else if (dbus_message_is_method_call(message, DNS_OPTIMIZATION_INTERFACE_PATH, METHOD_LIST_SET_EXTRA_DOMAIN))
    {
         if(!dbus_security_check(conn,message,&err)){
             goto fail;
         }
        DBusMessageIter iter;
        char *con_name,*domian;
        dbus_bool_t set_result;
        if (!dbus_message_get_args(message, &err, 
                                   DBUS_TYPE_STRING, &con_name,
                                   DBUS_TYPE_STRING, &domian,
                                   DBUS_TYPE_INVALID))
        {
            goto fail;
        }
        if (con_name !=NULL && set_extra_dns_domian(con_name,domian))
        {
            set_result = true;
            result=DBUS_HANDLER_RESULT_HANDLED;
            syslog(LOG_INFO,"set extra dns domain success!/n");
            
        }else
        {
            set_result = false;
            result=DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
            syslog(LOG_INFO,"set extra dns domain failed!/n");
        }
        reply = dbus_message_new_method_return(message);
        dbus_message_iter_init_append(reply, &iter);
        if (!dbus_message_iter_append_basic(&iter, DBUS_TYPE_BOOLEAN, &set_result))
        {
            syslog(LOG_INFO,"Out of Memory!/n");

        }
        goto send;
    }else if (dbus_message_is_method_call(message, DNS_OPTIMIZATION_INTERFACE_PATH, METHOD_LIST_SET_EXTRA_SEARCH))
    {
         if(!dbus_security_check(conn,message,&err)){
             goto fail;
         }
        DBusMessageIter iter;
        char *con_name,*search;
        dbus_bool_t set_result;
        if (!dbus_message_get_args(message, &err, 
                                   DBUS_TYPE_STRING, &con_name,
                                   DBUS_TYPE_STRING, &search,
                                   DBUS_TYPE_INVALID))
        {
            goto fail;
        }
        if (con_name !=NULL && set_extra_dns_search(con_name,search))
        {
            set_result = true;
            result=DBUS_HANDLER_RESULT_HANDLED;
            syslog(LOG_INFO,"set extra dns search success!/n");
            
        }else
        {
            set_result = false;
            result=DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
            syslog(LOG_INFO,"set extra dns search failed!/n");
        }
        reply = dbus_message_new_method_return(message);
        dbus_message_iter_init_append(reply, &iter);
        if (!dbus_message_iter_append_basic(&iter, DBUS_TYPE_BOOLEAN, &set_result))
        {
            syslog(LOG_INFO,"Out of Memory!/n");

        }
        goto send;
    }
    else if (dbus_message_is_method_call(message, DNS_OPTIMIZATION_INTERFACE_PATH, METHOD_LIST_SET_OPTIONS))
    {
        
         if(!dbus_security_check(conn,message,&err)){
              goto fail;
         }
             
        DBusMessageIter iter;
        char* timeout=NULL,*attempts=NULL,*type=NULL;
        char *con_name=NULL;
        dbus_bool_t set_result;
        if (!dbus_message_get_args(message, &err,
                                   DBUS_TYPE_STRING, &con_name,
                                   DBUS_TYPE_STRING, &timeout,
                                   DBUS_TYPE_STRING, &attempts,
                                   DBUS_TYPE_STRING,&type,
                                   DBUS_TYPE_INVALID))
        {
            goto fail;
        }
        if (con_name !=NULL && set_extra_dns_options(con_name,timeout,attempts,type))
        {
            set_result = true;
            result=DBUS_HANDLER_RESULT_HANDLED;
            syslog(LOG_INFO,"set extra dns options success!/n");
        }else
        {
            set_result = false;
            result=DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
            syslog(LOG_INFO,"set extra dns options failed!/n");
        }
        reply = dbus_message_new_method_return(message);
        dbus_message_iter_init_append(reply, &iter);
        if (!dbus_message_iter_append_basic(&iter, DBUS_TYPE_BOOLEAN, &set_result))
        {
            syslog(LOG_INFO,"Out of Memory!/n");

        }
        goto send;
    }
    else
    {
        result = DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }

fail:
   if (dbus_error_is_set(&err)) {
		if (reply)
			dbus_message_unref(reply);
		reply = dbus_message_new_error(message, err.name, err.message);
		dbus_error_free(&err);
	}

	if (!reply)
		return DBUS_HANDLER_RESULT_NEED_MEMORY;

	result = DBUS_HANDLER_RESULT_HANDLED;

	if (!dbus_connection_send(conn, reply, NULL))
		result = DBUS_HANDLER_RESULT_NEED_MEMORY;

	dbus_message_unref(reply);

	return result;
send:
    //消息回复、分发
    if (!dbus_connection_send(conn, reply, NULL)) // 发送该消息
        result = DBUS_HANDLER_RESULT_NEED_MEMORY;
    dbus_message_unref(reply); // 释放消息对象
    return result;
}
char *get_active_connection_uuid(char *path)
{
    g_type_init();

    GDBusConnection *connection;
    GDBusProxy *proxy;
    GError *error = NULL;
    GVariant *ret = NULL, *uuid = NULL;
    char *path_uuid = NULL;
    connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
    if (error != NULL)
    {
        g_print("Failed to get system bus: %s\n", error->message);
        g_error_free(error);
        return NULL;
    }

    proxy = g_dbus_proxy_new_sync(connection,
                                  G_DBUS_PROXY_FLAGS_NONE,
                                  NULL,
                                  "org.freedesktop.NetworkManager",
                                  path,
                                  "org.freedesktop.DBus.Properties",
                                  NULL,
                                  &error);
    if (error != NULL)
    {
        g_print("Failed to create proxy: %s\n", error->message);
        g_error_free(error);
        return NULL;
    }
    /* Get the object path of the Connection details */
    ret = g_dbus_proxy_call_sync(proxy,
                                 "Get",
                                 g_variant_new("(ss)",
                                               "org.freedesktop.NetworkManager.Connection.Active",
                                               "Uuid"),
                                 G_DBUS_CALL_FLAGS_NONE, -1,
                                 NULL, &error);
    if (!ret)
    {
        g_dbus_error_strip_remote_error(error);
        g_warning("Failed to get active connection Connection property: %s\n",
                  error->message);
        g_error_free(error);
        goto out;
    }

    g_variant_get(ret, "(v)", &uuid);
    if (!g_variant_is_of_type(uuid, G_VARIANT_TYPE_STRING))
    {
        g_warning("Unexpected type returned getting Connection property: %s",
                  g_variant_get_type_string(uuid));
        goto out;
    }

    path_uuid = g_variant_get_string(uuid, NULL);
    //syslog(LOG_INFO, "Primary connection uuid: %s\n", path_uuid);
    if (strnlen(path_uuid,MAXLINE) < 2)
    {
        path_uuid = NULL;
    }
    g_variant_unref(ret);
    g_object_unref (proxy);
    return path_uuid;
out:
    if (uuid)
        g_variant_unref(uuid);
    if (ret)
        g_variant_unref(ret);
    g_object_unref(proxy);
    return path_uuid;
}

void delete_options_form_conf(FILE *temp_fp,char*path)
{
    FILE *fp;
    char line_str[MAXLINE];
    fp = fopen(path, "w"); // 以只写方式打开文件，将原文件清空
    if (fp == NULL)
    {
        perror("Failed to open file for writing"); // 打开文件失败
        fclose(temp_fp);
        fclose(fp);
        return;
    }
    rewind(temp_fp); // 重置临时文件的读取位置
    while (fgets(line_str, MAXLINE, temp_fp) != NULL)
    {                    // 逐行读取临时文件内容
        fputs(line_str, fp); // 将临时文件的内容写回原文件
    }
    fclose(fp);
    fclose(temp_fp);
    return;
}
void delete_server_to_resolv()
{
    FILE *fp,*temp_fp;
    char line[MAXLINE];
    char line_str[MAXLINE];
    int found_first_nameserver = 0;

    if ((fp = fopen(DNS_CONFIG_FILE, "r")) == NULL)
    {
        perror("fopen");
        return;
    }

    temp_fp = tmpfile(); // 创建临时文件
    if (temp_fp == NULL)
    {
        perror("Failed to create temporary file"); // 创建临时文件失败
        fclose(fp);
        return;
    }

    while (fgets(line, MAXLINE, fp) != NULL)
    {
        size_t len = strlen(line);
        if (len == MAXLINE - 1 && line[len - 1] != '\n')
        {
            int c;
            while ((c = fgetc(fp)) != '\n' && c != EOF);
            continue;
        }
        if (strncmp(line, "nameserver 127.0.0.1", 20) != 0)
        {
            fputs(line, temp_fp);
        }
    }

    fclose(fp);
    fp = fopen(DNS_CONFIG_FILE, "w"); // 以只写方式打开文件，将原文件清空
    if (fp == NULL)
    {
        perror("Failed to open file for writing"); // 打开文件失败
        fclose(temp_fp);
        fclose(fp);
        return;
    }
    rewind(temp_fp); // 重置临时文件的读取位置
    while (fgets(line_str, MAXLINE, temp_fp) != NULL)
    {                    // 逐行读取临时文件内容
        fputs(line_str, fp); // 将临时文件的内容写回原文件
    }
    fclose(fp);
    fclose(temp_fp);

    return;
}
void write_server_to_resolv()
{
    FILE *fp,*temp_fp;
    char line[MAXLINE];
    char line_str[MAXLINE];
    // char *temp_file = "/tmp/resolv.conf.temp";
    int found_first_nameserver = 0;

    if ((fp = fopen(DNS_CONFIG_FILE, "r")) == NULL)
    {
        perror("fopen");
        return;
    }
    temp_fp = tmpfile(); // 创建临时文件
    if (temp_fp == NULL)
    {
        perror("Failed to create temporary file"); // 创建临时文件失败
        fclose(fp);
        return;
    }
    
    while (fgets(line, MAXLINE, fp) != NULL)
    {
        if (strncmp(line, "nameserver ", 11) == 0 && !found_first_nameserver)
        {
            if (strncmp(line, "nameserver 127.0.0.1", 20) != 0)
            {
                fprintf(temp_fp, "nameserver 127.0.0.1\n%s", line);
                
            }else{
                fputs(line, temp_fp);
            }
            found_first_nameserver = 1;
        }
        else
        {
            fputs(line, temp_fp);
        }
    }

    fclose(fp);
    syslog(LOG_INFO,"write_server_to_resolv");
    fp = fopen(DNS_CONFIG_FILE, "w"); // 以只写方式打开文件，将原文件清空
    if (fp == NULL)
    {
        perror("Failed to open file for writing"); // 打开文件失败
        fclose(temp_fp);
        fclose(fp);
        return;
    }
    rewind(temp_fp); // 重置临时文件的读取位置
    while (fgets(line_str, MAXLINE, temp_fp) != NULL)
    {        
        syslog(LOG_INFO,"write_server_to_resolv line_str=%s",line_str);            // 逐行读取临时文件内容
        fputs(line_str, fp); // 将临时文件的内容写回原文件
    }
    fclose(fp);
    fclose(temp_fp);
    return;
}
int write_nameserver_to_resolv(){
    FILE *fp,*resolv_fp,*temp_fp;
    char line_str[MAXLINE];
    int found=0;
    fp = fopen(DNS_RESOLV_DNSMASQ_FILE, "w");
    if (fp == NULL)
    {
        syslog(LOG_INFO, "DON NOT OPEN FILE");
        return 0;
    }
    fclose(fp);
    resolv_fp = fopen(DNS_CONFIG_FILE, "r+");
    if (resolv_fp == NULL)
    {
        syslog(LOG_INFO, "DON NOT OPEN FILE");
        return 0;
    }
    fp = fopen(DNS_RESOLV_DNSMASQ_FILE, "a");
    if (fp == NULL)
    {
        syslog(LOG_INFO, "DON NOT OPEN FILE");
        return 0;
    }
  
    while (fgets(line_str, MAXLINE, resolv_fp) != NULL)
    {
        if (strstr(line_str, "nameserver") != NULL )
        {
            if(strstr(line_str,"nameserver 127.0.0.1")==NULL && found==0)
            {
                fputs("nameserver 127.0.0.1\n",fp);
                fputs(line_str, fp);
                found=1;
            }else{
                fputs(line_str, fp);
            }    
             
        }
    }
    fclose(fp);
    fclose(resolv_fp);
    
    return 0;

}

void write_options_to_resolv(char *options)
{
    FILE *fp,*temp_fp;
    char line_str[MAXLINE];
    int found;
    fp = fopen(DNS_CONFIG_FILE, "a+");
    if (fp == NULL)
    {
        syslog(LOG_INFO, "DON NOT OPEN FILE");
        return;
    }
    temp_fp = tmpfile(); // 创建临时文件
    if (temp_fp == NULL)
    {
        perror("Failed to create temporary file"); // 创建临时文件失败
        fclose(fp);
        return;
    }
    while (fgets(line_str, MAXLINE, fp) != NULL)
    {
        if (strstr(line_str, "options") == NULL)
        {
            fputs(line_str, temp_fp); 
        }else{
            found =1;
        }
    }
    if(found)
    {
        fclose(fp);
        delete_options_form_conf(temp_fp,DNS_CONFIG_FILE);
        fp = fopen(DNS_CONFIG_FILE, "a+");
        if (fp == NULL)
        {
        syslog(LOG_INFO, "DON NOT OPEN FILE");
        return;
        }
    }
    fseek(fp, 0, SEEK_END);
    fprintf(fp, "%s %s\n", "options", options);
    fclose(fp);
}

void restart_dnsmasq()
{
    pid_t pid;
    int status;
    pid = fork();
    if (pid == -1)
    {
        perror("fork error");
        return;
    }
    else if (pid == 0)
    { // Child process
        char *args[] = {"/etc/init.d/dnsmasq", "restart", NULL};
        if (execve(args[0], args, NULL) == -1)
        {
            perror("execve error");
            return;
        }
    }
    else
    { // Parent process
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            syslog(LOG_INFO,"dnsmasq service restarted successfully\n");
        }
        else
        {
            syslog(LOG_INFO,"dnsmasq service restart failed\n");
        }
        return;
    }
}
void write_options_to_dnsmasq(char *options)
{
    FILE *fp,*temp_fp;
    
    char line_str[MAXLINE];
    fp = fopen(DNSMASQ_CONFIG_FILE, "a+");
    int found = 0;  // 标记是否找到关键字
    if (fp == NULL)
    {
        syslog(LOG_INFO, "DON NOT OPEN FILE");
        return;
    }
    temp_fp = tmpfile(); // 创建临时文件
    if (temp_fp == NULL)
    {
        perror("Failed to create temporary file"); // 创建临时文件失败
        fclose(fp);
        return;
    }
    
    while (fgets(line_str, MAXLINE, fp) != NULL)
    {
        if (strncmp(options, "all-servers",12) == 0 && strstr(line_str, "all-servers") != NULL)
        {
            fclose(fp);
            return;
        }
        else if (strncmp(options, "all-servers",12) != 0 && strstr(line_str, "all-servers") == NULL)
        {
            fputs(line_str, temp_fp); // 将当前行写入临时文件
        }
        else if (strncmp(options, "all-servers",12) != 0 && strstr(line_str, "all-servers") != NULL)
        {
            found = 1;
        }
    }
   
    if (found)
    {
        fclose(fp);
        delete_options_form_conf(temp_fp,DNSMASQ_CONFIG_FILE);
        restart_dnsmasq();
        return;
    }
    
    if(strncmp(options, "all-servers",12) == 0)
    {
        fseek(fp, 0, SEEK_END);
        fprintf(fp, "%s\n", options);
        restart_dnsmasq();
    }
    fclose(temp_fp);
    fclose(fp);
    return;
}

void update_extra_conf_to_resolv(char *path)
{
    char *type=NULL,*options=NULL,*timeout=NULL,*attempts=NULL;
    GError *error = NULL;
    char val[200]={0};
    char *options_val = calloc(200, sizeof(char *));
    char *uuid =get_active_connection_uuid(path);
    char *file =get_file_path(uuid);
    if(path == NULL)
        return NULL;
    if (extara_dns_conf_is_exist(file))
    {
        syslog(LOG_INFO,"EXTARA DNS CONF");
        MONITOR_FLAG=0;
        GKeyFile *key_file = g_key_file_new();
        if (!g_key_file_load_from_file(key_file, file, G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS, &error))
        {
            if (!g_error_matches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
                g_warning("Error loading key file: %s", error->message);
            return NULL;
        }
        type = g_key_file_get_string(key_file,OPTIONS, "type", &error);
        if(!strnlen(type,MAX_FILE_LENGTH))
            type="order";
        if (strncmp(type, "rotate",7)==0)
        {
            options = read_extra_dns_options(key_file,OPTIONS);
            write_options_to_resolv(options);
            write_nameserver_to_resolv();
            delete_server_to_resolv();
            write_options_to_dnsmasq("rotate");
        }
        else
        {
            timeout = g_key_file_get_string(key_file, OPTIONS, "timeout", &error);
            if(!strnlen(timeout,MAX_FILE_LENGTH))
                timeout="5";
            attempts = g_key_file_get_string(key_file, OPTIONS, "attempts", &error);
            if(!strnlen(attempts,MAX_FILE_LENGTH))
                attempts="2";
            snprintf(val, 200,"%s:%s %s:%s", "timeout", timeout, "attempts", attempts);
            snprintf(options_val,200,"%s",val);
            write_options_to_resolv(options_val);
            write_nameserver_to_resolv();
            if (strncmp(type, "order", 6) != 0)
            {
                write_server_to_resolv();
                write_options_to_dnsmasq("all-servers");
            }
            else
            {
                delete_server_to_resolv();
                write_options_to_dnsmasq("strict-order");
            }
        }
        g_key_file_free(key_file);
        free(options_val);
    }else{
        syslog(LOG_INFO,"DON NOT EXTARA DNS CONF");
        
    }
    return;
}
static DBusHandlerResult filter_func(DBusConnection *BUS, DBusMessage *message, void *user_data)
{
    dbus_bool_t handled = FALSE;
    char * path=NULL;
    if (dbus_message_is_signal(message, "org.freedesktop.NetworkManager.Connection.Active", "StateChanged"))
    {
        DBusMessageIter iter;
        int state, reason;
        if (!dbus_message_iter_init(message, &iter))
        {
            syslog(LOG_INFO, "MessageHas no Param");
        }
        else
        {
            if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_UINT32)
            {
                dbus_message_iter_get_basic(&iter, &state);
                dbus_message_iter_next(&iter);
                dbus_message_iter_get_basic(&iter, &reason);
                syslog(LOG_INFO, "state: %d reason=%d\n", state, reason);
                MONITOR_FLAG=1;
                if (state == 2)
                {
                    path = dbus_message_get_path(message);
                    if (path != NULL)
                    {
                        update_extra_conf_to_resolv(path);
                        //syslog(LOG_INFO, "path  %s\n", path);
                        handled = TRUE;
                        MONITOR_FLAG=0;
                    }
                }
            }
        }
    }
    return (handled ? DBUS_HANDLER_RESULT_HANDLED : DBUS_HANDLER_RESULT_NOT_YET_HANDLED);
}
void con_monitor(DBusConnection *conn)
{
    DBusError error;
    DBusMessage *message;
    dbus_error_init(&error);
    if (!conn)
    {
        dbus_error_free(&error);
        return;
    }
    dbus_bus_add_match(conn, "type='signal',interface='org.freedesktop.NetworkManager.Connection.Active'", &error);
    if (!dbus_connection_add_filter(conn, filter_func, NULL, NULL))
        return;
    if (dbus_error_is_set(&error))
    {
        dbus_error_free(&error);
    }
}

char *get_primaryconnection()
{
    g_type_init();

    GDBusConnection *connection;
    GDBusProxy *proxy;
    GError *error = NULL;
    GVariant *ret = NULL, *primary_connection=NULL;  
    char *path =NULL;
    connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
    if (error != NULL)
    {
        g_print("Failed to get system bus: %s\n", error->message);
        g_error_free(error);
        return NULL;
    }

    proxy = g_dbus_proxy_new_sync(connection,
                                  G_DBUS_PROXY_FLAGS_NONE,
                                  NULL,
                                  "org.freedesktop.NetworkManager",
                                  "/org/freedesktop/NetworkManager",
                                  "org.freedesktop.DBus.Properties",
                                  NULL,
                                  &error);
    if (error != NULL)
    {
        g_print("Failed to create proxy: %s\n", error->message);
        g_error_free(error);
        return NULL;
    }
    /* Get the object path of the Connection details */
	ret = g_dbus_proxy_call_sync (proxy,
	                              "Get",
	                              g_variant_new ("(ss)",
	                                             "org.freedesktop.NetworkManager",
	                                             "PrimaryConnection"),
	                              G_DBUS_CALL_FLAGS_NONE, -1,
	                              NULL, &error);
	if (!ret) {
		g_dbus_error_strip_remote_error (error);
		g_warning ("Failed to get active connection Connection property: %s\n",
		           error->message);
		g_error_free (error);
		goto out;
	}

    g_variant_get(ret, "(v)", &primary_connection);
    if (!g_variant_is_of_type (primary_connection, G_VARIANT_TYPE_OBJECT_PATH)) {
		g_warning ("Unexpected type returned getting Connection property: %s",
		           g_variant_get_type_string (primary_connection));
		goto out;
	}
    
	path = g_variant_get_string (primary_connection, NULL);
    //syslog(LOG_INFO,"Primary connection: %s\n", path);
    if(strnlen(path,MAX_FILE_LENGTH)<2)
    {
        path =NULL;
    }
    g_variant_unref(ret);
    g_object_unref (proxy);
    return path;
out:
	if (primary_connection)
		g_variant_unref (primary_connection);
	if (ret)
		g_variant_unref (ret);
	g_object_unref (proxy);
    return path;
}
int checkEnviron(DBusConnection *conn,int pid) {
    char filePath[64];
    snprintf(filePath, sizeof(filePath), "/proc/%d/environ", pid);
    if ( !conn || pid <=0)
		return 0;

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        fprintf(stderr, "file.open failed: %s\n", filePath);
        return 0;
    }

    char data[1024];
    fread(data, sizeof(char), sizeof(data) - 1, file);
    fclose(file);

    data[sizeof(data) - 1] = '\0';
    char *saveptr = NULL;
    char *environVars = strtok_r(data, "\0", &saveptr);

    while (environVars != NULL) {
        if (strnlen(environVars,1023) > 0) {
            char *pos = strchr(environVars, '=');
            if (pos != NULL) {
                char key[64];

                snprintf(key, pos - environVars,"%s",environVars);

                if (strcmp(key, "LD_PRELOAD") == 0 || strcmp(key, "LD_LIBRARY_PATH") == 0 || strcmp(key, "LD_AUDIT") == 0) {
                    return 0;
                }
            }
        }
        environVars = strtok_r(NULL, "\0", &saveptr);
    }

    return 1;
}
static int dbus_security_get_client_by_proc_exe(const char *exe_path, char buf[])
{
  int result = readlink(exe_path, buf, MAX_FILE_LENGTH - 1);

  if (result < 0 || (result > MAX_FILE_LENGTH - 1))
  {
    return -1;
  }

  buf[result] = '\0';

  return 0;
}
static inline int strstartswith(const char *str, const char *prefix)
{
    size_t sz = prefix ? strnlen(prefix,MAXLINE) : 0;

    if (str && sz && strncmp(str, prefix, sz) == 0)
        return 0;
    return 1;
}
static int dbus_security_get_client_by_proc_cmdline(const char *cmdline, char buf[])
{
  FILE *fp = NULL;

  //---------读取cmdline
  if ((fp = fopen(cmdline, "rb")) == NULL)
  {
    return -1;
  }

  //fscanf(fp, "%s", buf);
  if (!fgets(buf, MAX_FILE_LENGTH, fp))
  {
      fclose(fp);
      return -1;
  }
  fclose(fp);

  return 0;
}

static int dbus_security_get_client_name_by_pid(int pid, char buf[], char **spos)
{

	char cmdline[MAX_FILE_LENGTH] = {0};
	char exe_path[MAX_FILE_LENGTH] = {0};
	char tmp1[MAX_FILE_LENGTH*2] = {0};
	char *new_spos = NULL;
	FILE *fp = NULL;

    snprintf(exe_path,sizeof(exe_path),"/%s/%d/%s", "proc", pid, "exe");

    snprintf(cmdline,sizeof(cmdline),"/%s/%d/%s", "proc", pid, "cmdline");

	if(dbus_security_get_client_by_proc_exe(exe_path, buf)
		&& dbus_security_get_client_by_proc_cmdline(cmdline, buf))
	{
		return -1;
	}
    syslog(LOG_INFO,"buf=%s",buf);
//如果在cmdline中读取到的是以python开头，则选取后续字符为进程。
  if (!strstartswith(buf, "/usr/bin/python"))
  {
    if ((fp = fopen(cmdline, "rb")) == NULL)
    {
      return -1;
    }
    if (!fgets(tmp1, sizeof(tmp1), fp))
    {
        fclose(fp);
        return -1;
    }
    fclose(fp);
    new_spos = tmp1;
    new_spos += strnlen(tmp1,MAX_FILE_LENGTH);
    while (*new_spos == '\0' || *new_spos == '.')
    {
      new_spos++;
    }
    if (strstartswith(new_spos, "/usr") && *new_spos == '/'){
      new_spos++;
    }
    *spos = strdup(new_spos);
  }
  return 0;
}
gboolean isAllowedCaller(DBusConnection *conn, pid_t pid)
{
    char buf[MAX_FILE_LENGTH] = {0};
    char *spos = NULL;
    gboolean ret=FALSE;
    if (!conn || pid <= 0)
        goto exit;
    if (dbus_security_get_client_name_by_pid(pid, buf, &spos) == -1)
    {
        
        goto exit;
    }
    for (int i = 0; whiteList[i]; i++)
    {
        if ((spos && !strcmp(spos, whiteList[i])) || !strcmp(buf, whiteList[i]))
        {
            ret = TRUE;
            goto exit;
        }
    }
exit:
    if (spos)
        free(spos);
    syslog(LOG_INFO,"RET=%d",ret);
    return ret;
}

static int dbus_security_get_client_pid(DBusConnection* conn, DBusMessage* m, pid_t* pit_out)
{
	if(!conn || !m)
		return -1;

	int iRet = -1;
	DBusPendingCall *pending_call = NULL;
	DBusError err;
	DBusMessage *reply = NULL;
	DBusMessage *message = NULL;

	const char * sender = dbus_message_get_sender(m);
	if(sender)
	{
		message = dbus_message_new_method_call(
		"org.freedesktop.DBus",
		"/org/freedesktop/DBus",
		"org.freedesktop.DBus",
		"GetConnectionUnixProcessID");
		if(message)
		{
			DBusMessageIter iter;
			dbus_error_init(&err);
			dbus_message_iter_init_append(message, &iter);

			if(!dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &sender)){
				iRet = -1;
			};

			if(!dbus_connection_send_with_reply(conn, message, &pending_call, 3000)){
				iRet = -1;
			};

			if(!pending_call)
				goto exit;
			dbus_pending_call_block(pending_call);
			reply = dbus_pending_call_steal_reply(pending_call);
			if(!reply)
				goto exit;

			dbus_message_get_args(reply, &err, DBUS_TYPE_UINT32, pit_out, DBUS_TYPE_INVALID);
			if(*pit_out > 0)
			{
				iRet = 0;
			}
		}
	}
exit:
	if(pending_call)
		dbus_pending_call_unref(pending_call);
	if(reply)
		dbus_message_unref(reply);	
	if(dbus_error_is_set(&err))
		dbus_error_free(&err);
	dbus_message_unref(message);
	return iRet;
}
int dbus_security_check(DBusConnection* conn, DBusMessage* message,DBusError *error){
     // 获取当前进程的PID
        pid_t pid = 0;
        if(dbus_security_get_client_pid(conn,message,&pid)== -1){
            goto fail;
        }
        printf("当前进程的PID: %d", pid);
        if (!checkEnviron(conn,pid))
        {
           // err = "dbus method control,env forbidden";
            dbus_set_error(error,DBUS_ERROR_NOT_SUPPORTED,"dbus limit control,env forbidden");
		    syslog(LOG_ERR, "check env:forbidden\n");
            goto fail;
        }
        if(!isAllowedCaller(conn,pid))
        {
            dbus_set_error(error,DBUS_ERROR_NOT_SUPPORTED,"dbus method control,operation not permitted");
            syslog(LOG_ERR, "check list:forbidden\n");
            goto fail;
        }
        return 1;
fail:
    return 0;
}
