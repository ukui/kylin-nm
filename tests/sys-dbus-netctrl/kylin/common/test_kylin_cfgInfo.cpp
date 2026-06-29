#include "test_kylin_cfgInfo.h"

TEST_F(KylinCfgInfoTest, CreateCfg_Success) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    EXPECT_NE(handle, nullptr);
    if (handle) {
        int ret = kylinCfgDestroy(handle);
        EXPECT_EQ(ret, COMMON_SUCCESS);
    }
}

TEST_F(KylinCfgInfoTest, CreateCfg_MallocFailure) {
    // Note: Hard to test malloc failure without modifying source
    // Skipping this test case
}

TEST_F(KylinCfgInfoTest, DestroyCfg_NullHandle) {
    int ret = kylinCfgDestroy(nullptr);
    EXPECT_EQ(ret, COMMON_NULL_POINTER);
}

TEST_F(KylinCfgInfoTest, SetItem_Success) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    GVariant *data = g_variant_new_string("test_value");
    st_ItemCfgInfo *item = kylinCfgNewCommonItem("test_mod", "test_item", data);
    ASSERT_NE(item, nullptr);

    int ret = kylinCfgSetItem(handle, item);
    EXPECT_EQ(ret, COMMON_SUCCESS);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, SetItem_NullHandle) {
    GVariant *data = g_variant_new_string("test_value");
    st_ItemCfgInfo *item = kylinCfgNewCommonItem("test_mod", "test_item", data);
    ASSERT_NE(item, nullptr);

    int ret = kylinCfgSetItem(nullptr, item);
    EXPECT_EQ(ret, COMMON_NULL_POINTER);

    g_variant_unref(data);
    g_free(item->modName);
    g_free(item->itemName);
    g_free(item);
}

TEST_F(KylinCfgInfoTest, SetItem_NullItem) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    int ret = kylinCfgSetItem(handle, nullptr);
    EXPECT_EQ(ret, COMMON_NULL_POINTER);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, SetItem_MultipleItems) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    GVariant *data1 = g_variant_new_string("value1");
    st_ItemCfgInfo *item1 = kylinCfgNewCommonItem("mod1", "item1", data1);
    ASSERT_NE(item1, nullptr);
    EXPECT_EQ(kylinCfgSetItem(handle, item1), COMMON_SUCCESS);

    GVariant *data2 = g_variant_new_int32(123);
    st_ItemCfgInfo *item2 = kylinCfgNewCommonItem("mod1", "item2", data2);
    ASSERT_NE(item2, nullptr);
    EXPECT_EQ(kylinCfgSetItem(handle, item2), COMMON_SUCCESS);

    GVariant *data3 = g_variant_new_boolean(TRUE);
    st_ItemCfgInfo *item3 = kylinCfgNewCommonItem("mod2", "item1", data3);
    ASSERT_NE(item3, nullptr);
    EXPECT_EQ(kylinCfgSetItem(handle, item3), COMMON_SUCCESS);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, SetItem_ReplaceExisting) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    GVariant *data1 = g_variant_new_string("value1");
    st_ItemCfgInfo *item1 = kylinCfgNewCommonItem("mod1", "item1", data1);
    ASSERT_NE(item1, nullptr);
    EXPECT_EQ(kylinCfgSetItem(handle, item1), COMMON_SUCCESS);

    GVariant *data2 = g_variant_new_string("value2");
    st_ItemCfgInfo *item2 = kylinCfgNewCommonItem("mod1", "item1", data2);
    ASSERT_NE(item2, nullptr);
    EXPECT_EQ(kylinCfgSetItem(handle, item2), COMMON_SUCCESS);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, GetItem_Success) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    GVariant *data = g_variant_new_string("test_value");
    st_ItemCfgInfo *item = kylinCfgNewCommonItem("test_mod", "test_item", data);
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(kylinCfgSetItem(handle, item), COMMON_SUCCESS);

    st_ItemCfgInfo *retItem = kylinCfgGetItem(handle, "test_mod", "test_item");
    EXPECT_NE(retItem, nullptr);
    EXPECT_STREQ(retItem->modName, "test_mod");
    EXPECT_STREQ(retItem->itemName, "test_item");

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, GetItem_NullHandle) {
    st_ItemCfgInfo *retItem = kylinCfgGetItem(nullptr, "test_mod", "test_item");
    EXPECT_EQ(retItem, nullptr);
}

TEST_F(KylinCfgInfoTest, GetItem_NullModName) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    st_ItemCfgInfo *retItem = kylinCfgGetItem(handle, nullptr, "test_item");
    EXPECT_EQ(retItem, nullptr);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, GetItem_NullItemName) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    st_ItemCfgInfo *retItem = kylinCfgGetItem(handle, "test_mod", nullptr);
    EXPECT_EQ(retItem, nullptr);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, GetItem_NotFound) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    st_ItemCfgInfo *retItem = kylinCfgGetItem(handle, "nonexistent_mod", "nonexistent_item");
    EXPECT_EQ(retItem, nullptr);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, GetItem_OnlyModExists) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    GVariant *data = g_variant_new_string("test_value");
    st_ItemCfgInfo *item = kylinCfgNewCommonItem("test_mod", "test_item", data);
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(kylinCfgSetItem(handle, item), COMMON_SUCCESS);

    st_ItemCfgInfo *retItem = kylinCfgGetItem(handle, "test_mod", "wrong_item");
    EXPECT_EQ(retItem, nullptr);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, GetModItems_Success) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    GVariant *data1 = g_variant_new_string("value1");
    st_ItemCfgInfo *item1 = kylinCfgNewCommonItem("test_mod", "item1", data1);
    ASSERT_NE(item1, nullptr);
    EXPECT_EQ(kylinCfgSetItem(handle, item1), COMMON_SUCCESS);

    GVariant *data2 = g_variant_new_int32(123);
    st_ItemCfgInfo *item2 = kylinCfgNewCommonItem("test_mod", "item2", data2);
    ASSERT_NE(item2, nullptr);
    EXPECT_EQ(kylinCfgSetItem(handle, item2), COMMON_SUCCESS);

    GList *list = kylinCfgGetModItems(handle, "test_mod");
    EXPECT_NE(list, nullptr);
    EXPECT_GE(g_list_length(list), 2);

    g_list_free(list);
    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, GetModItems_NullHandle) {
    GList *list = kylinCfgGetModItems(nullptr, "test_mod");
    EXPECT_EQ(list, nullptr);
}

TEST_F(KylinCfgInfoTest, GetModItems_NullModName) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    GList *list = kylinCfgGetModItems(handle, nullptr);
    EXPECT_EQ(list, nullptr);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, GetModItems_NotFound) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    GList *list = kylinCfgGetModItems(handle, "nonexistent_mod");
    EXPECT_EQ(list, nullptr);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, RegisterUserItem_Success) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    // Use heap allocation to ensure the data persists after function returns
    st_ItemCfgInfo *itemData = (st_ItemCfgInfo*)g_malloc0(sizeof(st_ItemCfgInfo));
    itemData->modName = g_strdup("user_mod");
    itemData->itemName = g_strdup("user_item");
    itemData->dataType = KEYLIN_CFG_USER;
    itemData->data = g_strdup("user_data");

    int ret = kylinCfgRegisterUserItem(handle, itemData);
    EXPECT_EQ(ret, COMMON_SUCCESS);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, RegisterUserItem_NullHandle) {
    st_ItemCfgInfo itemData;
    itemData.modName = g_strdup("user_mod");
    itemData.itemName = g_strdup("user_item");
    itemData.dataType = KEYLIN_CFG_USER;
    itemData.userDataFreeCb = nullptr;

    int ret = kylinCfgRegisterUserItem(nullptr, &itemData);
    EXPECT_EQ(ret, COMMON_NULL_POINTER);

    g_free(itemData.modName);
    g_free(itemData.itemName);
}

TEST_F(KylinCfgInfoTest, RegisterUserItem_NullItem) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    int ret = kylinCfgRegisterUserItem(handle, nullptr);
    EXPECT_EQ(ret, COMMON_NULL_POINTER);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, RegisterUserItem_MultipleItems) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    st_ItemCfgInfo *itemData1 = (st_ItemCfgInfo*)g_malloc0(sizeof(st_ItemCfgInfo));
    itemData1->modName = g_strdup("mod1");
    itemData1->itemName = g_strdup("item1");
    itemData1->dataType = KEYLIN_CFG_USER;
    EXPECT_EQ(kylinCfgRegisterUserItem(handle, itemData1), COMMON_SUCCESS);

    st_ItemCfgInfo *itemData2 = (st_ItemCfgInfo*)g_malloc0(sizeof(st_ItemCfgInfo));
    itemData2->modName = g_strdup("mod2");
    itemData2->itemName = g_strdup("item2");
    itemData2->dataType = KEYLIN_CFG_USER;
    EXPECT_EQ(kylinCfgRegisterUserItem(handle, itemData2), COMMON_SUCCESS);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, NewCommonItem_Success) {
    GVariant *data = g_variant_new_string("test_value");
    st_ItemCfgInfo *item = kylinCfgNewCommonItem("mod", "item", data);
    ASSERT_NE(item, nullptr);
    EXPECT_STREQ(item->modName, "mod");
    EXPECT_STREQ(item->itemName, "item");
    EXPECT_EQ(item->dataType, KEYLIN_CFG_VARIANT);

    g_variant_unref(data);
    g_free(item->modName);
    g_free(item->itemName);
    g_variant_unref((GVariant*)item->data);
    g_free(item);
}

TEST_F(KylinCfgInfoTest, FlushCfg_Success) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    // Write a valid config file first
    char tmpfile[] = "/tmp/test_kylin_cfg_XXXXXX";
    int fd = mkstemp(tmpfile);
    ASSERT_GE(fd, 0);

    // Write valid config content
    const char *content = "name type value\n[mod1]\nitem1 s testvalue\n";
    write(fd, content, strlen(content));
    close(fd);

    // Load then set item and flush to the file
    EXPECT_EQ(kylinCfgLoad(handle, tmpfile), 0);

    // Set a new item
    GVariant *data = g_variant_new_string("new_value");
    st_ItemCfgInfo *item = kylinCfgNewCommonItem("test_mod", "test_item", data);
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(kylinCfgSetItem(handle, item), COMMON_SUCCESS);

    EXPECT_EQ(kylinCfgFlush(handle), COMMON_SUCCESS);

    // Verify file was written
    FILE *fp = fopen(tmpfile, "r");
    ASSERT_NE(fp, nullptr);
    char buff[256];
    EXPECT_NE(fgets(buff, sizeof(buff), fp), nullptr);
    fclose(fp);

    // Clean up
    unlink(tmpfile);
    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, FlushCfg_NullHandle) {
    int ret = kylinCfgFlush(nullptr);
    EXPECT_EQ(ret, COMMON_NULL_POINTER);
}

TEST_F(KylinCfgInfoTest, FlushCfg_NoCfgPath) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    int ret = kylinCfgFlush(handle);
    // Should fail because no path was set via load
    EXPECT_EQ(ret, COMMON_DO_FAILED);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, LoadCfg_Success) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    // Create a test config file
    char tmpfile[] = "/tmp/test_kylin_cfg_XXXXXX";
    int fd = mkstemp(tmpfile);
    ASSERT_GE(fd, 0);

    const char *content = "name type value\n[mod1]\nitem1 s test_value\n";
    write(fd, content, strlen(content));
    close(fd);

    int ret = kylinCfgLoad(handle, tmpfile);
    EXPECT_EQ(ret, 0);

    // Verify item was loaded
    st_ItemCfgInfo *item = kylinCfgGetItem(handle, "mod1", "item1");
    EXPECT_NE(item, nullptr);

    unlink(tmpfile);
    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, LoadCfg_NullHandle) {
    char tmpfile[] = "/tmp/test_kylin_cfg_XXXXXX";
    int fd = mkstemp(tmpfile);
    ASSERT_GE(fd, 0);
    close(fd);

    int ret = kylinCfgLoad(nullptr, tmpfile);
    EXPECT_EQ(ret, COMMON_NULL_POINTER);

    unlink(tmpfile);
}

TEST_F(KylinCfgInfoTest, LoadCfg_NullPath) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    int ret = kylinCfgLoad(handle, nullptr);
    EXPECT_EQ(ret, COMMON_NULL_POINTER);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, LoadCfg_FileNotExist) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    int ret = kylinCfgLoad(handle, "/nonexistent/path/config.ini");
    EXPECT_EQ(ret, COMMON_FILE_NOT_EXIST);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, LoadCfg_InvalidHeader) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    char tmpfile[] = "/tmp/test_kylin_cfg_XXXXXX";
    int fd = mkstemp(tmpfile);
    ASSERT_GE(fd, 0);

    const char *content = "invalid_header type value\\n[mod1]\\nitem1 s test_value\\n";
    write(fd, content, strlen(content));
    close(fd);

    int ret = kylinCfgLoad(handle, tmpfile);
    EXPECT_LT(ret, 0);

    unlink(tmpfile);
    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, LoadCfg_EmptyFile) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    char tmpfile[] = "/tmp/test_kylin_cfg_XXXXXX";
    int fd = mkstemp(tmpfile);
    ASSERT_GE(fd, 0);
    close(fd);

    int ret = kylinCfgLoad(handle, tmpfile);
    // Source returns COMMON_DO_FAILED (2) for empty file, not negative
    EXPECT_NE(ret, 0);

    unlink(tmpfile);
    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, ShowCfg_Success) {
    KYLIN_CFG_HANDLE handle = kylinCfgCreate();
    ASSERT_NE(handle, nullptr);

    GVariant *data = g_variant_new_string("test_value");
    st_ItemCfgInfo *item = kylinCfgNewCommonItem("test_mod", "test_item", data);
    ASSERT_NE(item, nullptr);
    EXPECT_EQ(kylinCfgSetItem(handle, item), COMMON_SUCCESS);

    int ret = kylinCfgShow(handle);
    EXPECT_EQ(ret, COMMON_SUCCESS);

    kylinCfgDestroy(handle);
}

TEST_F(KylinCfgInfoTest, ShowCfg_NullHandle) {
    int ret = kylinCfgShow(nullptr);
    EXPECT_EQ(ret, COMMON_NULL_POINTER);
}
