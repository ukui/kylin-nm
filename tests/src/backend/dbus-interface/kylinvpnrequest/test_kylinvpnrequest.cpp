#include "test_kylinvpnrequest.h"

void TestKylinVpnRequest::SetUp()
{
}

void TestKylinVpnRequest::TearDown()
{
}

TEST_F(TestKylinVpnRequest, testGetSecretsSize)
{
    size_t size = applet_vpn_request_get_secrets_size();
    EXPECT_GT(size, 0);
}

TEST_F(TestKylinVpnRequest, testSecretsRequestComplete_NullRequest)
{
    GVariant *settings = NULL;
    GError *error = NULL;
    SecretsRequest *req = NULL;
    applet_secrets_request_complete(req, settings, error);
    SUCCEED() << "Completed without crash";
}

TEST_F(TestKylinVpnRequest, testSecretsRequestFree_NullRequest)
{
    SecretsRequest *req = NULL;
    applet_secrets_request_free(req);
    SUCCEED() << "Free completed without crash";
}

TEST_F(TestKylinVpnRequest, testSecretsRequestSetFreeFunc)
{
    SecretsRequest req;
    req.free_func = NULL;
    applet_secrets_request_set_free_func(&req, NULL);
    EXPECT_EQ(req.free_func, NULL);
    SUCCEED() << "Set free func completed without crash";
}
