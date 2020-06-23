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

#ifndef __KYLINNETWORKINTERFACE_H__
#define __KYLINNETWORKINTERFACE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct
{
    char *if_name;
}ifname;

typedef struct
{
    char *ssid;//wifi名称
    int signal;//信号强度
    char *safety;//安全性

}wifilist;//存放wifi信息的结构体

typedef struct
{
    char *con_name;//网络连接名称
    char *type;//网络连接类型
}conlist;//存放所有网络连接

typedef struct
{
    char *con_name;//活动网络连接名称
    char *type;//活动网络连接类型
    char *dev;//活动网络所属设备
}activecon;//存放当前活动网络连接

/*
 * Get the network interface name.
 * return the struct pointer.
 */
ifname *kylin_network_get_ifacename();

/*
 * Get the all network connection.
 *  return the struct pointer.
 */
conlist *kylin_network_get_conlist_info();

/*
 * Get the active network connection.
 * return the struct pointer.
 */
activecon *kylin_network_get_activecon_info();

/*
 * Create a new Ethernet connection.
 * @con_name is the connection name.
 * @if_name is the interface name.
 */
void kylin_network_create_new_ethernet(char *con_name,char *if_name);

/*
 * Create a new Wifi connection.
 * @con_name is the connection name.
 * @if_name is the interface name.
 */
void kylin_network_create_new_wifi(char *con_name, char *if_name);

/*
 * Delete ethernet connection.
 * @con_name is the connection name.
 */
void kylin_network_del_ethernet_con(char *con_name);

/*
 * Set up dynamic IP allocation.
 * @con_name is the connection name.
 */
void kylin_network_set_automethod(char *con_name);

/*
 * Set up manual IP assignment.
 * @con_name is the connection name.
 * @ip is the ip address(for example,"192.168.68.160/16")
 */
void kylin_network_set_manualmethod(char *con_name,char *ip);

/*
 * Set up manual all prop.
 */
void kylin_network_set_manualall(char *con_name, char *addr, char *mask, char *gateway, char *dns);

/*
 * Sets whether the connection is automatic.
 * @con_name is the connection name.
 * @autocon is the automatic connection option.
 */
void kylin_network_set_autoconnect(char *con_name,bool autocon);

/*
 * Modify the ip address.
 * @con_name is the connection name.
 * @ip is the ip address(for example,"192.168.68.160/16")
 */
void kylin_network_mod_ip(char *con_name,char *ip);

/*
 * Modify the gateway.
 * @con_name is the connection name.
 * @gw is the gateway address.
 */
void kylin_network_mod_gateway(char *con_name,char *gw);

/*
 * Modify the dns address.
 * @con_name is the connection name.
 * @dns is the dns address.
 */
void kylin_network_mod_dns(char *con_name,char *dns);

/*
 * Connect the ethernet.
 * @con_name is the connection name.
 */
void kylin_network_set_con_up(char *con_name);

/*
 * Disconnect the ethernet.
 * @con_name is the connection name.
 */
void kylin_network_set_con_down(char *con_name);

/*
 * Connect the wifi.
 * @con_name is the wifi name.
 * @passwd is the wifi password.
 */
void kylin_network_set_wifi_up(char *con_name,char *passwd);

/*
 * Disconnect the wifi.
 * @if_name is the network interface name.
 */
void kylin_network_set_wifi_down(char *if_name);

/*
 * Get wifi list information.
 * Return the struct pointer.
 */
wifilist *kylin_network_get_wifilist_info();

/*
 * Enable networking.
 */
void kylin_network_enable_networking();

/*
 * Disable networking.
 */
void kylin_network_disable_networking();

/*
 * Enable wifi.
 */
void kylin_network_enable_wifi();

/*
 * Disable wifi.
 */
void kylin_network_disable_wifi();

/* Get the ip address.
 * @if_name is the interface name.
 * @ipaddr is used to save the ip address.
 */
int kylin_network_get_ipaddr(char *if_name,char *ipaddr);

/*
 * Get the broadcast address.
 * @if_name is the interface name.
 * @brdaddr is used to save the broadcast address.
 */
int kylin_network_get_brdaddr(char *if_name,char *brdaddr);

/*
 * Get the subnet mask.
 * @if_name is the interface name.
 * @netmask is used to save the subnet mask.
 */
int kylin_network_get_netmask(char *if_name,char *netmask);

/*
 * Get MAC address.
 * @if_name is the interface name.
 * @macaddr is used to save the MAC address.
 */
int kylin_network_get_mac(char *if_name,char *macaddr);

/* Get the MTU.
 * @if_name is the interface name.
 * return the MTU value.
 */
int kylin_network_get_mtu(char *if_name);

/*
 * Total upload and download data volume.
 * @if_name is the network interface name.
 * return the address of the first element of a one-dimensional long integer array.
 */
long *kylin_network_get_bytes(char *if_name);

/*
 * Total upload and download data packets.
 * @if_name is the network interface name.
 * return the address of the first element of a one-dimensional long integer array.
 */
long *kylin_network_get_packets(char *if_name);

/*
 * Total wrong data packets number of uploading and downloading.
 * @if_name is the network interface name.
 * return the address of the first element of a one-dimensional long integer array.
 */
long *kylin_network_get_errs(char *if_name);

/*
 * Total discarded data packets number of uploading and downloading.
 * @if_name is the network interface name.
 * return the address of the first element of a one-dimensional long integer array.
 */
long *kylin_network_get_drop(char *if_name);

/*
 * Total overloaded data packets number of uploading and downloading.
 * @if_name is the network interface name.
 * return the address of the first element of a one-dimensional long integer array.
 */
long *kylin_network_get_fifo(char *if_name);

#ifdef __cplusplus
}
#endif

#endif
