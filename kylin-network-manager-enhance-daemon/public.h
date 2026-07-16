#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#include <syslog.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <pthread.h>
#include <sys/select.h>    
#include <sys/inotify.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h> /* for glib main loop */
#include <stdbool.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include "dbus-server.h"
#include "file_monitor.h"
#include <gio/gio.h>
#include "dbus-server.h"
#include <netdb.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define EXTRA_DNS_CONF_FILE "/etc/nm_enhance/"
#define NM_ENHANCE_DNS "nm_enhance_dns.conf"
#define DNS_CONFIG_FILE "/etc/resolv.conf"
#define DNS_RESOLV_DNSMASQ_FILE "/etc/resolv.dnsmasq.conf"
#define DNSMASQ_CONFIG_FILE "/etc/dnsmasq.conf"
extern int MONITOR_FLAG;
#define OPTION_FLAG "option"
#define FIRST_FLAG "first"
#define ALTER_FLAG "alternatives"
#define NECESSARY_FLAG "necessary"
#define NAMESERVER "nameserver"
#define OPTIONS "options"
#define DOMAIN "domian"
#define SEARCH "search"
extern pthread_t file_tid;
extern pthread_t con_tid;
#define MAXLINE 1024
#define FAIL 0
#define PASS 1
extern DBusConnection *BUS; //global variable
#define _GNU_SOURCE
void* con_monitor(void *arg);


#endif

