#include "test_kylin_async_task.h"

TEST_F(KylinAsyncTaskTest, CreateTaskPool_Success) {
    KYLIN_ASYNC_HANDLE handle = kylinAsyncTaskCreate(2);
    EXPECT_NE(handle, nullptr);
    if (handle) {
        kylinAsyncTaskRelease(handle);
    }
}

TEST_F(KylinAsyncTaskTest, CreateTaskPool_ZeroThreads) {
    KYLIN_ASYNC_HANDLE handle = kylinAsyncTaskCreate(0);
    EXPECT_NE(handle, nullptr);
    if (handle) {
        kylinAsyncTaskRelease(handle);
    }
}

TEST_F(KylinAsyncTaskTest, AddTask_Success) {
    KYLIN_ASYNC_HANDLE handle = kylinAsyncTaskCreate(2);
    ASSERT_NE(handle, nullptr);

    int arg = 0;
    int taskId = kylinAsyncTaskAdd(handle, simpleTaskFunc, &arg);
    EXPECT_GE(taskId, 0);

    usleep(100000);
    EXPECT_EQ(arg, 100);

    kylinAsyncTaskRelease(handle);
}

TEST_F(KylinAsyncTaskTest, AddTask_MultipleTasks) {
    KYLIN_ASYNC_HANDLE handle = kylinAsyncTaskCreate(2);
    ASSERT_NE(handle, nullptr);

    int arg1 = 0, arg2 = 0, arg3 = 0;
    int taskId1 = kylinAsyncTaskAdd(handle, simpleTaskFunc, &arg1);
    int taskId2 = kylinAsyncTaskAdd(handle, simpleTaskFunc, &arg2);
    int taskId3 = kylinAsyncTaskAdd(handle, simpleTaskFunc, &arg3);

    EXPECT_GE(taskId1, 0);
    EXPECT_GE(taskId2, 0);
    EXPECT_GE(taskId3, 0);

    usleep(200000);
    EXPECT_EQ(arg1, 100);
    EXPECT_EQ(arg2, 100);
    EXPECT_EQ(arg3, 100);

    kylinAsyncTaskRelease(handle);
}

TEST_F(KylinAsyncTaskTest, RemoveTask_Success) {
    KYLIN_ASYNC_HANDLE handle = kylinAsyncTaskCreate(2);
    ASSERT_NE(handle, nullptr);

    int arg = 0;
    int taskId = kylinAsyncTaskAdd(handle, simpleTaskFunc, &arg);
    EXPECT_GE(taskId, 0);

    int ret = kylinAsyncTaskRemove(handle, taskId);
    EXPECT_EQ(ret, COMMON_SUCCESS);

    usleep(100000);
    EXPECT_EQ(arg, 0);

    kylinAsyncTaskRelease(handle);
}

TEST_F(KylinAsyncTaskTest, RemoveTask_InvalidTaskId) {
    KYLIN_ASYNC_HANDLE handle = kylinAsyncTaskCreate(2);
    ASSERT_NE(handle, nullptr);

    int ret = kylinAsyncTaskRemove(handle, 9999);
    EXPECT_EQ(ret, COMMON_SUCCESS);

    kylinAsyncTaskRelease(handle);
}

//TEST_F(KylinAsyncTaskTest, AddTask_NullHandle) {
//    int taskId = kylinAsyncTaskAdd(nullptr, simpleTaskFunc, nullptr);
//    EXPECT_LT(taskId, 0);
//}

TEST_F(KylinAsyncTaskTest, RemoveTask_NullHandle) {
    int ret = kylinAsyncTaskRemove(nullptr, 0);
    EXPECT_NE(ret, COMMON_SUCCESS);
}

TEST_F(KylinAsyncTaskTest, ReleaseTask_NullHandle) {
    kylinAsyncTaskRelease(nullptr);
}
