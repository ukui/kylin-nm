// Mock实现kdk_system_get_appScene函数
extern "C" {
#include <stdlib.h>
#include <string.h>
#include <cstring>

// 从环境变量获取模拟的应用场景
char* kdk_system_get_appScene() {
    const char* env = getenv("MOCK_APP_SCENE");
    if (env && strlen(env) > 0) {
        char* result = (char*)malloc(256);
        strncpy(result, env, 255);
        result[255] = '\0';
        return result;
    }
    return nullptr; // 模拟真实函数可能返回NULL
}
}