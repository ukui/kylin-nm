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

char *read_extra_dns_conf(GKeyFile *key_file, char *flags)
{
    GError *error = NULL;
    char *val = g_key_file_get_value(key_file, flags, "secondary", &error);
    
    if (val == NULL &&
        !g_error_matches(error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_KEY_NOT_FOUND))
    {
        g_warning("Error finding key in key file: %s", error->message);
        return NULL;
    }
    
    if (!strnlen(val,MAXLINE))
    {
        val = NULL;
    }
    
    return val;
}
char *read_extra_dns_options(GKeyFile *key_file, char *flags)
{
    GError *error = NULL;
    char *timeout=NULL,*attempts=NULL,*type=NULL;
    
    char val[200]={0};
    char *options_val = calloc(200, sizeof(char *));
    timeout = g_key_file_get_string(key_file, flags, "timeout", &error);
    if(!timeout || !strnlen(timeout,200))
        timeout="5";
    attempts = g_key_file_get_string(key_file, flags, "attempts", &error);
    if(!attempts || !strnlen(attempts,200))
        attempts="2";
    type = g_key_file_get_string(key_file, flags, "type", &error); 
    snprintf(val,200,"%s:%s %s:%s %s","timeout",timeout,"attempts",attempts,type);
    snprintf(options_val,200,"%s",val);
    return options_val;
}
char *read_extra_dns_domian(GKeyFile *key_file, char *flags)
{
    GError *error = NULL;
    char *val = g_key_file_get_value(key_file, flags, "domian", &error);
    if (val == NULL &&
        !g_error_matches(error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_KEY_NOT_FOUND))
    {
        g_warning("Error finding key in key file: %s", error->message);
        return NULL;
    }
    if (!val || !strnlen(val,MAXLINE))
    {
        val = NULL;
    }
    return val;
}
char *read_extra_dns_search(GKeyFile *key_file, char *flags)
{
    GError *error = NULL;
    char *val = NULL;
    val = g_key_file_get_value(key_file, flags, "search", &error);
    if (val == NULL &&
        !g_error_matches(error, G_KEY_FILE_ERROR, G_KEY_FILE_ERROR_KEY_NOT_FOUND))
    {
        g_warning("Error finding key in key file: %s", error->message);
        return NULL;
    }
    if (!val || !strnlen(val,MAXLINE))
    {
        val = NULL;
    }
    return val;
}
char *get_file_path(char *con_name)
{
    char file_path[300] = {0};

    if (con_name == NULL)
        return NULL;

    // 安全检测：防止目录跳转攻击

    // 检测是否包含 "/" (路径分隔符)
    if (strstr(con_name, "/") != NULL) {
        syslog(LOG_ERR, "Security alert: con_name contains path separator '/'");
        return NULL;
    }

    char *file = calloc(300, sizeof(char *));
    snprintf(file_path,300, "%s%s-%s", EXTRA_DNS_CONF_FILE, con_name, NM_ENHANCE_DNS);
    snprintf(file,300,"%s",file_path);
    return file;
}
bool extara_dns_conf_is_exist(char *config)
{
    if(!config) return false;

    if ((access(config, F_OK)) != -1)
    {
        syslog(LOG_INFO, "dns_extra_conf exist");
        return true;
    }
    return false;
}

int set_extra_dns(char *con_name, char *extradns)
{
    char *file = get_file_path(con_name);
    
    //syslog(LOG_INFO, "file=%s", file);
    if (extara_dns_conf_is_exist(file))
    {
        GError *error = NULL;
        GKeyFile *key_file = g_key_file_new();
        if (!g_key_file_load_from_file(key_file, file, G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS, &error))
        {
            if (!g_error_matches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
                g_warning("Error loading key file: %s", error->message);
            return FAIL;
        }
        if (g_key_file_has_group(key_file, NAMESERVER))
        {
            g_key_file_set_string(key_file, NAMESERVER, "secondary", extradns);
            // Save as a file.
            if (!g_key_file_save_to_file(key_file, file, &error))
            {
                g_warning("Error saving key file: %s", error->message);
                return FAIL;
            }
        }
        g_key_file_free(key_file);
        
        free(file);
        return PASS;
    }
    return FAIL;
}
int set_extra_dns_domian(char *con_name, char *domian)
{
    char *file = get_file_path(con_name);
    if (extara_dns_conf_is_exist(file))
    {
        GError *error = NULL;
        GKeyFile *key_file = g_key_file_new();
        if (!g_key_file_load_from_file(key_file, file, G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS, &error))
        {
            if (!g_error_matches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
                g_warning("Error loading key file: %s", error->message);
            return FAIL;
        }
        if (g_key_file_has_group(key_file, DOMAIN))
        {
            g_key_file_set_string(key_file, DOMAIN, "domain", domian);
            // Save as a file.
            if (!g_key_file_save_to_file(key_file, file, &error))
            {
                g_warning("Error saving key file: %s", error->message);
                return FAIL;
            }
        }
        g_key_file_free(key_file);
        free(file);
        return PASS;
    }
    return FAIL;
}
int set_extra_dns_search(char *con_name, char *search)
{
    char *file =NULL; 
    file = get_file_path(con_name);
    if (extara_dns_conf_is_exist(file))
    {
        GError *error = NULL;
        GKeyFile *key_file = g_key_file_new();
        if (!g_key_file_load_from_file(key_file, file, G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS, &error))
        {
            if (!g_error_matches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
                g_warning("Error loading key file: %s", error->message);
            return FAIL;
        }
        if (g_key_file_has_group(key_file, SEARCH))
        {
            g_key_file_set_string(key_file, SEARCH, "search", search);
            // Save as a file.
            if (!g_key_file_save_to_file(key_file, file, &error))
            {
                g_warning("Error saving key file: %s", error->message);
                return FAIL;
            }
        }
        g_key_file_free(key_file);

        free(file);
        return PASS;
    }
    return FAIL;
}

int set_extra_dns_options(char *con_name, char *timeout, char *attempts, char *type)
{
    char *file = get_file_path(con_name);
    //syslog(LOG_INFO,"FILE=%s",file);
    if (file != NULL)
    {
        GError *error = NULL;
        GKeyFile *key_file = g_key_file_new();
        if (extara_dns_conf_is_exist(file))
        {
            if (!g_key_file_load_from_file(key_file, file, G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS, &error))
            {
                if (!g_error_matches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
                    g_warning("Error loading key file: %s", error->message);
                return FAIL;
            }
            if (g_key_file_has_group(key_file, OPTIONS))
            {
                g_key_file_set_string(key_file, OPTIONS, "timeout", timeout);
                g_key_file_set_string(key_file, OPTIONS, "attempts", attempts);
                g_key_file_set_string(key_file, OPTIONS, "type", type);
                // Save as a file.
                if (!g_key_file_save_to_file(key_file, file, &error))
                {
                    g_warning("Error saving key file: %s", error->message);
                    g_key_file_free(key_file);
                    return FAIL;
                }
            }
        }
        else
        {
            g_key_file_set_string(key_file, OPTIONS, "timeout", timeout);
            g_key_file_set_string(key_file, OPTIONS, "attempts", attempts);
            g_key_file_set_string(key_file, OPTIONS, "type", type);
            // Save as a file.
            if (!g_key_file_save_to_file(key_file, file, &error))
            {
                g_warning("Error saving key file: %s", error->message);
                g_key_file_free(key_file);
                return FAIL;
            }
        }
        g_key_file_free(key_file);
        return PASS;
    }
    return FAIL;
}
void update_options_to_resolv(char *file)
{
    char *type=NULL,*options=NULL,*timeout=NULL,*attempts=NULL;
    GError *error = NULL;
    char val[200]={0};
    char *options_val = calloc(200, sizeof(char *));
    if(file == NULL)
        return;
    if (extara_dns_conf_is_exist(file))
    {
        syslog(LOG_INFO,"EXTARA DNS CONF");
        GError *error = NULL;
        GKeyFile *key_file = g_key_file_new();
        if (!g_key_file_load_from_file(key_file, file, G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS, &error))
        {
            if (!g_error_matches(error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
                g_warning("Error loading key file: %s", error->message);
            return;
        }
        type = g_key_file_get_string(key_file,OPTIONS, "type", &error);
        if(!strnlen(type,MAXLINE))
            type="order";
        if (strncmp(type, "rotate",7)==0)
        {
            options = read_extra_dns_options(key_file,OPTIONS);
            write_options_to_resolv(options);
            write_nameserver_to_resolv();
        }
        else
        {
            timeout = g_key_file_get_string(key_file, OPTIONS, "timeout", &error);
            if(!strnlen(timeout,200))
                timeout="5";
            attempts = g_key_file_get_string(key_file, OPTIONS, "attempts", &error);
            if(!strnlen(attempts,200))
                attempts="2";
            snprintf(val,200, "%s:%s %s:%s", "timeout", timeout, "attempts", attempts);
            snprintf(options_val,200,"%s",val);
            write_options_to_resolv(options_val);
            write_nameserver_to_resolv();
            if (strncmp(type, "order", 6) != 0)
                write_server_to_resolv();
        }
        g_key_file_free(key_file);
        free(options_val);
    }else{
        syslog(LOG_INFO,"DON NOT EXTARA DNS CONF");
        write_nameserver_to_resolv();
        
    }
}
//从buf中取出一个事件
static void _inotify_event_handler(struct inotify_event *event)
{
    syslog(LOG_INFO, "event->mask: 0x%08x\n", event->mask);
    syslog(LOG_INFO, "event->name: %s MONITOR_FLAG=%d\n", event->name,MONITOR_FLAG);
    if (event->mask & IN_IGNORED)
    {
        write_nameserver_to_resolv();
        if (!MONITOR_FLAG)
        {
            char *options = NULL, *active_con = NULL, *uuid = NULL;
            active_con = get_primaryconnection();
            if(active_con == NULL)
                return;
            uuid = get_active_connection_uuid(active_con);
            char *file = NULL;
            file = get_file_path(uuid);
            update_options_to_resolv(file);
        }
    }
}

void* file_monitor(void* arg)
{
    unsigned char buf[1024] = {0};
    struct inotify_event *event = NULL;
    //  int monitor_flag = IN_MOVE_SELF | IN_DELETE_SELF | IN_IGNORED | IN_ATTRIB | IN_MODIFY;
    int monitor_flag =  IN_IGNORED;
    int fd = inotify_init();                                       //初始化
    int wd = inotify_add_watch(fd, DNS_CONFIG_FILE, monitor_flag); //监控指定文件的ALL_EVENTS。
    for (;;)
    {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        if (select(fd + 1, &fds, NULL, NULL, NULL) > 0)
        { //监控fd的事件。当有事件发生时，返回值>0
            int len, index = 0;
            while (((len = read(fd, &buf, sizeof(buf))) < 0) && (errno == EINTR))
            {
                syslog(LOG_INFO, "没有读取到事件");
            } //没有读取到事件。
            while (index < len)
            {
                event = (struct inotify_event *)(buf + index);
                _inotify_event_handler(event);
                if (event->mask & IN_IGNORED)
                {
                    syslog(LOG_INFO, "file is IGNORED. \n");
                    wd = inotify_add_watch(fd, DNS_CONFIG_FILE, monitor_flag);
                    syslog(LOG_INFO, "file is MODIFYED.\n");
                }                                                   //获取事件。
                index += sizeof(struct inotify_event) + event->len; //移动index指向下一个事件。
            }
        }
    }
    syslog(LOG_INFO, "删除对指定文件的监控");
    inotify_rm_watch(fd, wd); //删除对指定文件的监控。

    return NULL;
}
