/*
 * @Author: 武丹 wudan@kylinos.cn
 * @Date: 2023-04-17 13:53:28
 * @LastEditors: 武丹 wudan@kylinos.cn
 * @LastEditTime: 2023-04-17 17:56:44
 * @FilePath: /dns_en/dns-xml.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
//对外提供的接口列表
#ifndef __NET_ENHANCE_XML_H__
#define __NET_ENHANCE_XML_H__

#include <dbus/dbus.h>
#include <dbus/dbus-glib-lowlevel.h> /* for glib main loop */

//对外提供的接口列表
//manager接口列表
static char *server_introspection_xml =
    DBUS_INTROSPECT_1_0_XML_DOCTYPE_DECL_NODE
    "<node name='/com/kylin/network/enhancement/optimization'>\n"
    "  <interface name='org.freedesktop.DBus.Introspectable'>\n"
    "    <method name='Introspect'>\n"
    "      <arg name='data' type='s' direction='out' />\n"
    "    </method>\n"
    "   <method name='Quit'>\n"
    "   </method>\n"
    "  </interface>\n"
    // "  <interface name='com.kylin.network.enhancement.optimization'>\n"
    // "   <method name='GetSettings'>\n"
    // "       <arg name='setting' type='s' direction='out'/>\n"
    // "   </method>\n"
    // "  </interface>\n"
    "<node name='DNS'/>\n"
    "</node>\n";

static char *dns_xml =
    DBUS_INTROSPECT_1_0_XML_DOCTYPE_DECL_NODE
    "<node name ='/com/kylin/network/enhancement/optimization/DNS'>\n"
    "  <interface name='org.freedesktop.DBus.Introspectable'>\n"
    "    <method name='Introspect'>\n"
    "      <arg name='data' type='s' direction='out' />\n"
    "    </method>\n"
    "  </interface>\n"
    "  <interface name='com.kylin.network.enhancement.optimization.DNS'>\n"
    "   <method name='GetAllExtraDns'>\n"
    "       <arg name='name' type='s' direction='in'/>\n"
    "       <arg name='result' type='s' direction='out'/>\n"
    "   </method>\n"
    "   <method name='GetExtraDns'>\n"
    "       <arg name='name' type='s' direction='in'/>\n"
    "       <arg name='result' type='a{sv}' direction='out'/>\n"
    "   </method>\n"
    "   <method name='SetExtraDns'>\n"
    "       <arg name='name' type='s' direction='in'/>\n"
    "       <arg name='extradns' type='s' direction='in'/>\n"
    "       <arg name='result' type='b' direction='out'/>\n"
    "   </method>\n"
    "   <method name='SetOptions'>\n"
    "       <arg name='name' type='s' direction='in'/>\n"
    "       <arg name='timeout' type='s' direction='in'/>\n"
    "       <arg name='attempts' type='s' direction='in'/>\n"
    "       <arg name='type' type='s' direction='in'/>\n"
    "       <arg name='result' type='b' direction='out'/>\n"
    "   </method>\n"
    "   <method name='SetExtraDomain'>\n"
    "       <arg name='name' type='s' direction='in'/>\n"
    "       <arg name='domain' type='s' direction='in'/>\n"
    "       <arg name='result' type='b' direction='out'/>\n"
    "   </method>\n"
    "   <method name='SetExtraSearch'>\n"
    "       <arg name='name' type='s' direction='in'/>\n"
    "       <arg name='search' type='s' direction='in'/>\n"
    "       <arg name='result' type='b' direction='out'/>\n"
    "   </method>\n"
    "  </interface>\n"
    "</node>\n";
#endif //__NET_ENHANCE_XML_H__
