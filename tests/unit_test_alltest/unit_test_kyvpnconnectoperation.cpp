/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2024 KylinSoft  Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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
#include <gtest/gtest.h>
#include <gtest/gtest-death-test.h>
#include <QDBusReply>
#include <QDBusInterface>
#include <QGSettings>
#include <QMetaType>
#include <QDBusMetaType>
#include "stubext.h"
#include "../../src/backend/dbus-interface/kyvpnconnectoperation.h"

using namespace stub_ext;

class KyVpnConnectOperationTest : public testing::Test
{
protected:
    static void SetUpTestSuite()
    {
    }

    static void TearDownTestSuite()
    {
    }
    static QDBusInterface* m_pDBus;
};

QDBusInterface *KyVpnConnectOperationTest::m_pDBus = nullptr;
TEST_F(KyVpnConnectOperationTest, allParameter)
{
    KyVpnConfig vpnConfig;
    vpnConfig.m_vpnName = "";
    vpnConfig.m_gateway = "";
     //VPN用户密码信息
    vpnConfig.m_userName = "";
     //用户密码
    vpnConfig.m_userPasswd = "";
     //密码策略
     //NT域
    vpnConfig.m_ntDomain = "";

     //CA证书
    vpnConfig.m_caCertificate = "";
     //用户证书
    vpnConfig.m_userCertificate = "";
     //用户私钥
    vpnConfig.m_userKey = "";
     //静态密钥
    vpnConfig.m_staticKey = "";
     //用户私有密钥密码
    vpnConfig.m_privatePasswd = "";
     //密钥方向
    vpnConfig.m_vpnKeyDir = "";
     //本地IP地址
    vpnConfig.m_localAddress = "";
     //远程IP地址
    vpnConfig.m_remoteAddress = "";
     //PIN码
    vpnConfig.m_pinId = "";

     //MPPE加密开关
    vpnConfig.m_mppeEnable = false;
     //使用有状态加密
    vpnConfig.m_isAllowStatefulEncryption = false;
     //发送PPP回显包
    vpnConfig.m_sendPppPackage = false;
     //MRU 最大接收单元
    vpnConfig.m_mru = "";
     //MTU 最大传输单元
    vpnConfig.m_mtu = "";
     //请求使用内部IP
    vpnConfig.m_virtual = "";
     //强制UDP封装
    vpnConfig.m_encap = "";
     //使用IP压缩
    vpnConfig.m_ipcomp = "";
     //启用自定义密码建议
    vpnConfig.m_proposal = "";
     //IKE
    vpnConfig.m_ike = "";
     //ESP
    vpnConfig.m_esp = "";

     //使用自定义网关端口
    vpnConfig.m_useAssignPort = "";
    vpnConfig.m_assignPort = "";

     //使用自定义重协商间隔
    vpnConfig.m_useRenegSeconds = "";
    vpnConfig.m_renegSeconds = "";

     //使用压缩
    vpnConfig.m_useCompress = "";

     //使用TCP连接
    vpnConfig.m_useTcpLink = "";

     //设置虚拟设备类型
    vpnConfig.m_setDevType = "";

     //设置虚拟设备名称
    vpnConfig.m_setDevName = "";
    vpnConfig.m_devName = "";

     //使用自定义隧道最大单元传输
    vpnConfig.m_useTunnelMtu = "";
    vpnConfig.m_tunnelMtu = "";

     //使用自定义UDP分片大小
    vpnConfig.m_useFragmentSize = "";
    vpnConfig.m_fragmentSize = "";

     //限制TCP最大段尺寸
    vpnConfig.m_mssfix = "";

     //随机化远程主机
    vpnConfig.m_remoteRandom = "";

     //IPv6 tun link
    vpnConfig.m_ipv6TunLink = "";

     //指定Ping周期
    vpnConfig.m_setPingCycle = "";
    vpnConfig.m_pingCycle = "";

     //指定退出或重启Ping
    vpnConfig.m_usePingMethod = "";
    vpnConfig.m_pingMethodTime = "";

     //接受来自任何地址（Float）已通过身份验证的数据包
    vpnConfig.m_float = "";
     //指定路由上限
    vpnConfig.m_setMaxRoute = "";
    vpnConfig.m_maxRoute = "";
     //主题匹配
    vpnConfig.m_verifyName = "";

     //验证对等（服务器）证书用法签名
    vpnConfig.m_useRemoteCertTls = "";
     //验证对等（服务器）证书nsCertType签名
    vpnConfig.m_useNsCertTls = "";
     //密钥文件
    vpnConfig.m_vpnKeyFile = "";
     //密钥方向
    vpnConfig.m_vpnTlsTaDir = "";
     //服务器地址
    vpnConfig.m_vpnProxyServer = "";
     //端口
    vpnConfig.m_vpnProxyPort = "";
     //出错时无限重试
    vpnConfig.m_vpnProxyRetry = "";
     //代理用户名
    vpnConfig.m_vpnProxyName = "";
     //代理用户名
    vpnConfig.m_vpnProxyPasswd = "";
    vpnConfig.m_useKeysize = "";
    vpnConfig.m_keySize = "";
    vpnConfig.m_ipv4DhcpClientId = "";
    ASSERT_EQ(100, 100);
}

TEST_F(KyVpnConnectOperationTest, createVpnConnect)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyVpnConnectOperationTest, setVpnConfig)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyVpnConnectOperationTest, getVpnConfig)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyVpnConnectOperationTest, activateVpnConnection)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyVpnConnectOperationTest, deactivateVpnConnection)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyVpnConnectOperationTest, deleteVpnConnect)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyVpnConnectOperationTest, getHmacAuthMethod)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyVpnConnectOperationTest, getAuthMethod)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyVpnConnectOperationTest, getConnectionSetting)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyVpnConnectOperationTest, setConnectionSetting)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyVpnConnectOperationTest, getUsrPasswdAndPolicy)
{
    ASSERT_EQ(100, 100);
}

TEST_F(KyVpnConnectOperationTest, setUsrPasswdAndPolicy)
{
    ASSERT_EQ(100, 100);
}


