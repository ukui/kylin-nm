#ifndef TEST_KYLIN_ASYNC_TASK_H
#define TEST_KYLIN_ASYNC_TASK_H

#include <gtest/gtest.h>

extern "C" {
#include "kylin_async_task.h"
#include "kylin_common.h"
}

class KylinAsyncTaskTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

    static void simpleTaskFunc(void *arg) {
        if (arg) {
            int *value = (int*)arg;
            *value = 100;
        }
    }
};

#endif // TEST_KYLIN_ASYNC_TASK_H
