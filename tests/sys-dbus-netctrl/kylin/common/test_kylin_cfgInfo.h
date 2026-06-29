#ifndef TEST_KYLIN_CFGINFO_H
#define TEST_KYLIN_CFGINFO_H

#include <gtest/gtest.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "kylin_cfgInfo.h"
#include "kylin_common.h"
}

class KylinCfgInfoTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

#endif // TEST_KYLIN_CFGINFO_H
