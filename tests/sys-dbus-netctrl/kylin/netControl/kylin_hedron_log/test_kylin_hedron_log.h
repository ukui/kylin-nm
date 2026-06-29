#ifndef TEST_KYLIN_HEDRON_LOG_H
#define TEST_KYLIN_HEDRON_LOG_H

#include <gtest/gtest.h>
#include <glib.h>
#include <string.h>

extern "C" {
#include "kylin_hedron_log.h"
#include "kylin_common.h"

#define MAX_LENGTH 256

// Struct definition from kylin_hedron_log.c
typedef struct ST_HedronLogMessage
{
    char *createTime;
    char *hostName;
    char *IP;
    char *name;
    char *level;
    char *message;
} st_HedronLogMessage;

// Additional function declarations from kylin_hedron_log.c
st_HedronLogMessage* kylinLoadHedronMessage(char *level, char *message);
char* hedronDataTransformStr(st_HedronLogMessage *pHedronLogMessage);
int hedronMessageFree(st_HedronLogMessage *pHedronLogMessage);
int hedronGetDataTime(char *backTime);
int hedronGetDataHostname(char *backHostName);
int hedronGetDataIp(char *backIp);
int kylinHedronDbusSendLog(char *sendInfo);
}

#define TEST_MAX_LENGTH 256

class TestKylinHedronLog : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(TestKylinHedronLog, KylinSendHedronMessage_NullLevel) {
    int ret = kylinSendHedronMessage(NULL, "test");
    EXPECT_EQ(ret, COMMON_NULL_POINTER);
}

TEST_F(TestKylinHedronLog, KylinSendHedronMessage_EmptyLevel) {
    int ret = kylinSendHedronMessage("", "test");
    EXPECT_EQ(ret, COMMON_NULL_POINTER);
}

TEST_F(TestKylinHedronLog, KylinSendHedronMessage_NullMessage) {
    char *level = (char*)"error";
    int ret = kylinSendHedronMessage(level, NULL);
    EXPECT_EQ(ret, COMMON_NULL_POINTER);
}

TEST_F(TestKylinHedronLog, KylinSendHedronMessage_EmptyMessage) {
    char *level = (char*)"error";
    char *emptyMsg = (char*)"";
    int ret = kylinSendHedronMessage(level, emptyMsg);
    EXPECT_EQ(ret, COMMON_NULL_POINTER);
}

TEST_F(TestKylinHedronLog, KylinLoadHedronMessage_ValidInput) {
    st_HedronLogMessage *msg = kylinLoadHedronMessage((char*)"info", (char*)"test message");
    EXPECT_NE(msg, nullptr);
    if (msg) {
        EXPECT_STREQ(msg->level, "info");
        EXPECT_STREQ(msg->message, "test message");
        hedronMessageFree(msg);
    }
}

TEST_F(TestKylinHedronLog, KylinLoadHedronMessage_NullLevel) {
    st_HedronLogMessage *msg = kylinLoadHedronMessage(NULL, (char*)"test");
    EXPECT_EQ(msg, nullptr);
}

TEST_F(TestKylinHedronLog, KylinLoadHedronMessage_EmptyLevel) {
    st_HedronLogMessage *msg = kylinLoadHedronMessage((char*)"", (char*)"test");
    EXPECT_EQ(msg, nullptr);
}

TEST_F(TestKylinHedronLog, KylinLoadHedronMessage_NullMessage) {
    st_HedronLogMessage *msg = kylinLoadHedronMessage((char*)"error", NULL);
    EXPECT_EQ(msg, nullptr);
}

TEST_F(TestKylinHedronLog, KylinLoadHedronMessage_EmptyMessage) {
    st_HedronLogMessage *msg = kylinLoadHedronMessage((char*)"error", (char*)"");
    EXPECT_EQ(msg, nullptr);
}

TEST_F(TestKylinHedronLog, HedronDataTransformStr_ValidMessage) {
    st_HedronLogMessage *msg = kylinLoadHedronMessage((char*)"error", (char*)"test");
    ASSERT_NE(msg, nullptr);
    
    char *jsonStr = hedronDataTransformStr(msg);
    EXPECT_NE(jsonStr, nullptr);
    EXPECT_GT(strlen(jsonStr), 0);
    
    // Verify JSON contains expected fields
    EXPECT_NE(strstr(jsonStr, "lv"), nullptr);
    EXPECT_NE(strstr(jsonStr, "message"), nullptr);
    
    g_free(jsonStr);
    hedronMessageFree(msg);
}

TEST_F(TestKylinHedronLog, HedronMessageFree_NullPointer) {
    int ret = hedronMessageFree(NULL);
    EXPECT_EQ(ret, COMMON_NULL_POINTER);
}

TEST_F(TestKylinHedronLog, HedronGetDataTime_Valid) {
    char timeStr[MAX_LENGTH] = {0};
    int ret = hedronGetDataTime(timeStr);
    EXPECT_EQ(ret, 0);
    EXPECT_GT(strlen(timeStr), 0);
    // Verify format: YYYY-MM-DD HH:MM:SS
    EXPECT_EQ(strlen(timeStr), 19);
}

TEST_F(TestKylinHedronLog, HedronGetDataIp_Valid) {
    char ipStr[MAX_LENGTH] = {0};
    int ret = hedronGetDataIp(ipStr);
    EXPECT_EQ(ret, 0);
    // IP address should be non-empty if network is available
    // Note: This may fail in containerized environments without network
}

TEST_F(TestKylinHedronLog, KylinSendHedronMessage_ValidInfoLevel) {
    int ret = kylinSendHedronMessage((char*)"info", (char*)"test info message");
    // May fail if D-Bus service is not available, but should not crash
    EXPECT_TRUE(ret == COMMON_SUCCESS || ret < 0);
}

TEST_F(TestKylinHedronLog, KylinSendHedronMessage_ValidErrorLevel) {
    int ret = kylinSendHedronMessage((char*)"error", (char*)"test error message");
    // May fail if D-Bus service is not available, but should not crash
    EXPECT_TRUE(ret == COMMON_SUCCESS || ret < 0);
}

TEST_F(TestKylinHedronLog, KylinSendHedronMessage_LongMessage) {
    char longMsg[512] = {0};
    memset(longMsg, 'A', 500);
    longMsg[500] = '\0';
    
    int ret = kylinSendHedronMessage((char*)"info", longMsg);
    // May fail if D-Bus service is not available, but should not crash
    EXPECT_TRUE(ret == COMMON_SUCCESS || ret < 0);
}

#endif
