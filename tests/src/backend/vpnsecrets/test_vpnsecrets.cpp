/*
 * Kylin-NM Unit Test for vpnsecrets
 * Test functions: kylinvpnrequest functions, kylinvpnagent functions
 */

#include "test_vpnsecrets.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to create test VPN connection
static NMConnection* createTestVpnConnection()
{
    NMSettingConnection *s_con;
    NMSettingVpn *s_vpn;
    NMConnection *connection;

    connection = nm_simple_connection_new();

    // Add connection setting
    s_con = (NMSettingConnection *)nm_setting_connection_new();
    nm_connection_add_setting(connection, NM_SETTING(s_con));
    g_object_set(G_OBJECT(s_con),
                 NM_SETTING_CONNECTION_ID, "Test VPN Connection",
                 NM_SETTING_CONNECTION_UUID, "test-uuid-12345",
                 NM_SETTING_CONNECTION_TYPE, NM_SETTING_VPN_SETTING_NAME,
                 NULL);

    // Add VPN setting
    s_vpn = (NMSettingVpn *)nm_setting_vpn_new();
    g_object_set(s_vpn,
                 NM_SETTING_VPN_SERVICE_TYPE,
                 "awesomevpn",
                 NM_SETTING_VPN_USER_NAME,
                 "Bill Smith",
                 NULL);

    nm_setting_vpn_add_data_item(s_vpn, "server", "vpn.somewhere.com");
    nm_setting_vpn_add_secret(s_vpn, "password", "sup3rs3cr3t");
    nm_connection_add_setting(connection, NM_SETTING(s_vpn));

    return connection;
}


// TestVpnSecretsResultGet implementation
void TestVpnSecretsResultGet::SetUp()
{
    testConnection = createTestVpnConnection();
}

void TestVpnSecretsResultGet::TearDown()
{
        if (testConnection) {
        g_object_unref(testConnection);
        testConnection = NULL;
    }
}

// Test kylinVpnSecretsResultGet returns non-NULL
TEST_F(TestVpnSecretsResultGet, get_secrets)
{
    GError *p_secretError = NULL;

    QVector<const char*> cstrList;
    cstrList.reserve(1);
    cstrList.append(nullptr);  // NULL 终
    const char **nmHints = const_cast<const char**>(cstrList.data());

    SecretsRequest * req = applet_secrets_request_new (applet_vpn_request_get_secrets_size(),
                                    testConnection,
                                    0,
                                    "Test VPN Connection",
                                    nmHints,
                                    4,
                                    NULL,
                                    NULL,
                                    NULL);

    if (!applet_vpn_request_get_secrets (req, &p_secretError)){
        qDebug()  << " failed vpn get secrets";
    }


    KylinVpnSecretsResult *result = kylinVpnSecretsResultGet();
    EXPECT_NE(result, nullptr);
}


// TestVpnAgentVpnSave implementation
void TestKylinAgentVpnSave::SetUp()
{
    testConnection = createTestVpnConnection();
}

void TestKylinAgentVpnSave::TearDown()
{
    if (testConnection) {
        g_object_unref(testConnection);
        testConnection = NULL;
    }
}

// Test kylinAgentVpnSave with valid connection
TEST_F(TestKylinAgentVpnSave, ValidConnection)
{
    GCancellable *cancellable = g_cancellable_new();
    ASSERT_NE(cancellable, nullptr);

    // This should not crash even if keyring operations fail
    kylinAgentVpnSave(testConnection, "/test/path", cancellable);

    g_object_unref(cancellable);
}

// Test kylinAgentVpnSave with NULL cancellable
TEST_F(TestKylinAgentVpnSave, inValidConnection)
{
    // This should not crash
    kylinAgentVpnSave(testConnection,NULL, NULL);
}