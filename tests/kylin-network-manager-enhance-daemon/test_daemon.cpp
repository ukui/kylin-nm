/*
 * Kylin-NM Unit Test for kylin-network-manager-enhance-daemon
 * Test functions: file_monitor functions, dbus-server functions
 */

#include "test_daemon.h"
#include <QDir>
#include <QFileInfo>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/wait.h>

#define TEST_CONN_NAME "test-connection"

extern "C" {

pthread_t file_tid;
pthread_t con_tid;

const struct DBusObjectPathVTable server_vtable = {.message_function = server_message_handler};
const struct DBusObjectPathVTable server_vtable_dns = {.message_function = server_message_handler_dns};
int CHILDPRO_LEN = 0;   //子进程列表实际长度
#define MAX_CHILD_PROCESS_CNT  20
pid_t child_process[MAX_CHILD_PROCESS_CNT]; //子进程列表
DBusConnection *BUS = NULL;
int MONITOR_FLAG = -1;
}

// Wrapper function for pthread_create to call con_monitor with correct signature
static void* con_monitor_wrapper(void* arg) {
    con_monitor((DBusConnection*)arg);
    return NULL;
}

// Wrapper function for pthread_create to call file_monitor with correct signature
static void* file_monitor_wrapper(void* arg) {
    file_monitor();
    return NULL;
}

// Local implementation of strstartswith for testing
// Returns 0 if str starts with prefix, 1 otherwise
static inline int strstartswith(const char *str, const char *prefix)
{
    size_t sz = prefix ? strnlen(prefix, 1024) : 0;

    if (str && sz && strncmp(str, prefix, sz) == 0)
        return 0;
    return 1;
}

// Helper function to create test config file
static void createTestConfigFile(const QString &filePath, const QString &content)
{
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << content;
        file.close();
    }
}


// TestGetFilePath implementation
void TestGetFilePath::SetUp()
{
}

void TestGetFilePath::TearDown()
{
}

// Test get_file_path with NULL input
TEST_F(TestGetFilePath, NullInput)
{
    char *result = get_file_path(NULL);
    EXPECT_EQ(result, nullptr);
}

// Test get_file_path with valid connection name
TEST_F(TestGetFilePath, ValidConnectionName)
{
    char *result = get_file_path(const_cast<char*>("test-connection"));
    ASSERT_NE(result, nullptr);
    EXPECT_TRUE(QString(result).contains("test-connection"));
    EXPECT_TRUE(QString(result).contains("nm_enhance_dns.conf"));
    free(result);
}

// Test get_file_path with empty string
TEST_F(TestGetFilePath, EmptyConnectionName)
{
    char *result = get_file_path(const_cast<char*>(""));
    // Empty string should still produce a path (may be invalid)
    if (result != nullptr) {
        free(result);
    }
}

// Test get_file_path with special characters
TEST_F(TestGetFilePath, SpecialCharacters)
{
    char *result = get_file_path(const_cast<char*>("test@#$%connection"));
    if (result != nullptr) {
        free(result);
    }
}

// TestExtraDnsConfExist implementation
void TestExtraDnsConfExist::SetUp()
{
}

void TestExtraDnsConfExist::TearDown()
{
}

// Test extara_dns_conf_is_exist with non-existent file
TEST_F(TestExtraDnsConfExist, NonExistentFile)
{
    bool result = extara_dns_conf_is_exist(const_cast<char*>("/nonexistent/path/config.conf"));
    EXPECT_EQ(result, false);
}

// Test extara_dns_conf_is_exist with existing file
TEST_F(TestExtraDnsConfExist, ExistingFile)
{
    if (!tempDir.isValid()) {
        GTEST_SKIP() << "Temp directory not available";
    }
    
    QString testFile = tempDir.path() + "/test.conf";
    createTestConfigFile(testFile, "[nameserver]\nsecondary=8.8.8.8\n");
    
    bool result = extara_dns_conf_is_exist(const_cast<char*>(testFile.toUtf8().data()));
    EXPECT_EQ(result, true);
}

// Test extara_dns_conf_is_exist with empty path
TEST_F(TestExtraDnsConfExist, EmptyPath)
{
    bool result = extara_dns_conf_is_exist(const_cast<char*>(""));
    EXPECT_EQ(result, false);
}

// TestReadExtraDns implementation
void TestReadExtraDns::SetUp()
{
    if (tempDir.isValid()) {
        testConfigFile = tempDir.path() + "/test_connection-nm_enhance_dns.conf";
        QString content = "[nameserver]\n"
                         "secondary=8.8.8.8\n\n"
                         "[options]\n"
                         "timeout=5\n"
                         "attempts=2\n"
                         "type=order\n\n"
                         "[domain]\n"
                         "domain=example.com\n\n"
                         "[search]\n"
                         "search=localdomain\n";
        createTestConfigFile(testConfigFile, content);
    }
}

void TestReadExtraDns::TearDown()
{
}

// Test read_extra_dns_conf with valid key file
TEST_F(TestReadExtraDns, ReadExtraDnsConfValid)
{
    if (testConfigFile.isEmpty()) {
        GTEST_SKIP() << "Test config file not available";
    }
    
    GKeyFile *key_file = g_key_file_new();
    GError *error = NULL;
    
    ASSERT_TRUE(g_key_file_load_from_file(key_file, testConfigFile.toUtf8().data(), 
                                          G_KEY_FILE_KEEP_COMMENTS, &error));
    
    char *result = read_extra_dns_conf(key_file, const_cast<char*>("nameserver"));
    if (result != NULL) {
        free(result);
    }
    
    g_key_file_free(key_file);
}

// Test read_extra_dns_conf with non-existent group
TEST_F(TestReadExtraDns, ReadExtraDnsConfNonExistentGroup)
{
    if (testConfigFile.isEmpty()) {
        GTEST_SKIP() << "Test config file not available";
    }
    
    GKeyFile *key_file = g_key_file_new();
    GError *error = NULL;
    
    ASSERT_TRUE(g_key_file_load_from_file(key_file, testConfigFile.toUtf8().data(), 
                                          G_KEY_FILE_KEEP_COMMENTS, &error));
    
    char *result = read_extra_dns_conf(key_file, const_cast<char*>("nonexistent"));
    // Should return NULL for non-existent group
    
    g_key_file_free(key_file);
}

// Test read_extra_dns_options with valid key file
TEST_F(TestReadExtraDns, ReadExtraDnsOptionsValid)
{
    if (testConfigFile.isEmpty()) {
        GTEST_SKIP() << "Test config file not available";
    }
    
    GKeyFile *key_file = g_key_file_new();
    GError *error = NULL;
    
    ASSERT_TRUE(g_key_file_load_from_file(key_file, testConfigFile.toUtf8().data(), 
                                          G_KEY_FILE_KEEP_COMMENTS, &error));
    
    char *result = read_extra_dns_options(key_file, const_cast<char*>("options"));
    if (result != NULL) {
        EXPECT_TRUE(strlen(result) > 0);
        free(result);
    }
    
    g_key_file_free(key_file);
}

// Test read_extra_dns_options with non-existent group
TEST_F(TestReadExtraDns, ReadExtraDnsOptionsNonExistent)
{
    GKeyFile *key_file = g_key_file_new();
    
    char *result = read_extra_dns_options(key_file, const_cast<char*>("nonexistent"));
    // Function should handle gracefully
    
    g_key_file_free(key_file);
}

// Test read_extra_dns_domian with valid key file
TEST_F(TestReadExtraDns, ReadExtraDnsDomainValid)
{
    if (testConfigFile.isEmpty()) {
        GTEST_SKIP() << "Test config file not available";
    }
    
    GKeyFile *key_file = g_key_file_new();
    GError *error = NULL;
    
    ASSERT_TRUE(g_key_file_load_from_file(key_file, testConfigFile.toUtf8().data(), 
                                          G_KEY_FILE_KEEP_COMMENTS, &error));
    
    char *result = read_extra_dns_domian(key_file, const_cast<char*>("domain"));
    if (result != NULL) {
        free(result);
    }
    
    g_key_file_free(key_file);
}

// Test read_extra_dns_search with valid key file
TEST_F(TestReadExtraDns, ReadExtraDnsSearchValid)
{
    if (testConfigFile.isEmpty()) {
        GTEST_SKIP() << "Test config file not available";
    }
    
    GKeyFile *key_file = g_key_file_new();
    GError *error = NULL;
    
    ASSERT_TRUE(g_key_file_load_from_file(key_file, testConfigFile.toUtf8().data(), 
                                          G_KEY_FILE_KEEP_COMMENTS, &error));
    
    char *result = read_extra_dns_search(key_file, const_cast<char*>("search"));
    if (result != NULL) {
        free(result);
    }
    
    g_key_file_free(key_file);
}

// TestSetExtraDns implementation
void TestSetExtraDns::SetUp()
{
    if (tempDir.isValid()) {
        testConfigFile = tempDir.path() + "/test_connection-nm_enhance_dns.conf";
        QString content = "[nameserver]\n"
                         "secondary=8.8.8.8\n\n"
                         "[options]\n"
                         "timeout=5\n"
                         "attempts=2\n"
                         "type=order\n";
        createTestConfigFile(testConfigFile, content);
    }
}

void TestSetExtraDns::TearDown()
{
}

// Test set_extra_dns_options with NULL file
TEST_F(TestSetExtraDns, SetExtraDnsOptionsNullFile)
{
    int result = set_extra_dns_options(TEST_CONN_NAME, const_cast<char*>("5"), const_cast<char*>("2"), const_cast<char*>("order"));
    //EXPECT_EQ(result, 1); // PASS
}


// Test set_extra_dns with non-existent config file
TEST_F(TestSetExtraDns, SetExtraDnsNonExistent)
{
    int result = set_extra_dns(const_cast<char*>("nonexistent-connection"), const_cast<char*>("8.8.8.8"));
    EXPECT_EQ(result, 0); // FAIL
}

// Test set_extra_dns with  connection name
TEST_F(TestSetExtraDns, SetExtraDnsConnectionName)
{
    int result = set_extra_dns(TEST_CONN_NAME, const_cast<char*>("8.8.8.8"));
    result = set_extra_dns_domian(TEST_CONN_NAME, const_cast<char*>("example.com"));
    result = set_extra_dns_search(TEST_CONN_NAME, const_cast<char*>("localdomain"));

    EXPECT_EQ(result, 1); // PASS
}

// Test set_extra_dns_domian with non-existent config file
TEST_F(TestSetExtraDns, SetExtraDnsDomainNonExistent)
{
    int result = set_extra_dns_domian(const_cast<char*>("nonexistent-connection"), const_cast<char*>("example.com"));
    EXPECT_EQ(result, 0); // FAIL
}

// Test set_extra_dns_search with non-existent config file
TEST_F(TestSetExtraDns, SetExtraDnsSearchNonExistent)
{
    int result = set_extra_dns_search(const_cast<char*>("nonexistent-connection"), const_cast<char*>("localdomain"));
    EXPECT_EQ(result, 0); // FAIL
}


// TestCheckEnviron implementation
void DbusServerConstants::SetUp()
{
    #if 0
    DBusConnection *conn;
    DBusError err;
    int rv;
    void * in_data;


    printf("DBUS SetUp init\n");
    dbus_error_init(&err);

    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (!conn)
    {
        goto fail;
    }
    BUS=conn;
    rv = dbus_bus_request_name(conn,
                               "com.kylin.network.enhancement.unitest",
                               DBUS_NAME_FLAG_REPLACE_EXISTING, //one of the standard flag
                               &err);

    if (rv != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER)
    {
        printf("fail to request a dbus name!,err name=%s,err msg = %s\n",err.name,err.message);
        goto fail;
    }

    if (!dbus_connection_try_register_object_path(conn,
                                                  NETWORK_ENHANCEMENT_OPTIMIZATION_DBUS_OBJECT_PATH,
                                                  &server_vtable,
                                                  in_data,
                                                  &err))
    {

        printf("fail to register a object path!,err name=%s,err msg = %s\n",err.name,err.message);

        goto fail;
    }
    if (!dbus_connection_try_register_object_path(conn,
                                                  DNS_OPTIMIZATION_DBUS_OBJECT_PATH,
                                                  &server_vtable_dns,
                                                  in_data,
                                                  &err))
    {

        printf("fail to register a object path!,err name=%s,err msg = %s\n",err.name,err.message);

        goto fail;
    }
    printf("DBUS服务启动成功，进入DBUS消息循环监听状态...\n");
    
    if(!pthread_create(&con_tid,NULL,con_monitor_wrapper,BUS))
    {
       printf("con monitor pthread_create");
    }
    if(!pthread_create(&file_tid,NULL,file_monitor_wrapper,NULL))
    {
       printf("file monitor pthread_create");
    }
    mainloop = g_main_loop_new(NULL, false);

    dbus_connection_setup_with_g_main(conn, NULL);

    g_main_loop_run(mainloop);

    return ;

fail:
    dbus_error_free(&err);
    return ;
#endif
}


void DbusServerConstants::TearDown()
{
}

TEST_F(DbusServerConstants, DbusNameFormat)
{
#if 0
    
    // Execute gdbus call command
    const char* gdbus_cmd = "gdbus call --session --dest com.kylin.network.enhancement.unitest "
                            "--object-path /com/kylin/network/enhancement/optimization/DNS "
                            "--method com.kylin.network.enhancement.optimization.DNS.GetAllExtraDns "
                            "'test'";
    
    FILE* pipe = popen(gdbus_cmd, "r");
    ASSERT_NE(pipe, nullptr) << "Failed to execute gdbus command";
    
    char buffer[1024] = {0};
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    
    int return_status = pclose(pipe);
    
    // Verify the command executed successfully
    EXPECT_EQ(WEXITSTATUS(return_status), 0) << "gdbus command failed with exit code: " << WEXITSTATUS(return_status);
    #endif
}

TEST_F(DbusServerConstants, DbusGetPath)
{
     char *active_con = NULL, *uuid = NULL;
    active_con = get_primaryconnection();
    if(active_con == NULL)
        return;
    uuid = get_active_connection_uuid(active_con);
    EXPECT_TRUE(uuid);
}