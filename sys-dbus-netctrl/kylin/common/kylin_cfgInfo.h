#ifndef _KYLIN_CONFIGINFO_H
#define _KYLIN_CONFIGINFO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <glib.h>

#define KYLIN_CFG_HANDLE  void*
#ifndef KYLIN_CFG_API
#define  KYLIN_CFG_API
#endif

#define KYLIN_CFG_MAX_NAMESIZE (1024)

typedef enum E_KylinCfgDataType{
    KEYLIN_CFG_VARIANT= 0, //通用数据
    KEYLIN_CFG_USER,//用户自定义
}e_KylinCfgDataType;

typedef struct ST_ItemCfgInfo
{
   char* modName;//模块名
   char* itemName;//项名
   void* data;//数据
   e_KylinCfgDataType dataType;//数据类型
   int (*fromCfgCb) (void *cfgParm,void **realParm);//将配置文件读取数据自定义方法转成参数 不配置不调用
   int (*toCfgCb) (void **cfgParm,void *realParm);//将参数自定义方法转成配置参数
   int (*verifyCfgCb)(void* data);//参数检查
   int (*userDataFreeCb)(void* data);//用户自定义数据释放回调 只有dataType为KEYLIN_CFG_USER使用
}st_ItemCfgInfo;


/*注意：接口线程安全
  限制1.模块名、项名不得使用特殊字符如制表位换行符等与空格
     2.字符串长度不得超过KYLIN_CFG_MAX_NAMESIZE
     3.自定义数据类型需要自己写解析、配置函数、释放函数等并kylinCfgRegisterUserItem注册
*/

/*************************************************
函数名称:     kylinCfgCreate
函数描述:     创建句柄
输入参数:
输出参数:     KYLIN_CFG_HANDLE:配置句柄
返回值:       其它：成功  NULL：失败
*************************************************/
KYLIN_CFG_API KYLIN_CFG_HANDLE kylinCfgCreate();

/*************************************************
函数名称:     kylinCfgDestroy
函数描述:
输入参数:     KYLIN_CFG_HANDLE:配置句柄
输出参数:
返回值:       见KYLIN_COMMON_ERR_CODE
*************************************************/
KYLIN_CFG_API int kylinCfgDestroy(KYLIN_CFG_HANDLE handle);

/*************************************************
函数名称:     kylinCfgSetItem
函数描述:     插入一条配置到内存
输入参数:     KYLIN_CFG_HANDLE:配置句柄
             st_ItemCfgInfo 条目值
输出参数:
返回值:       见KYLIN_COMMON_ERR_CODE
*************************************************/
KYLIN_CFG_API int kylinCfgSetItem(KYLIN_CFG_HANDLE handle,st_ItemCfgInfo * itemData);

/*************************************************
函数名称:     kylinCfgGetItem
函数描述:     从内存获取到指定条目配置
输入参数:     KYLIN_CFG_HANDLE:配置句柄
             st_ItemCfgInfo 条目值
输出参数:
返回值:       其它：成功  NULL：失败
*************************************************/
KYLIN_CFG_API st_ItemCfgInfo * kylinCfgGetItem(KYLIN_CFG_HANDLE handle,char* modName,char* itemName);

/*************************************************
函数名称:     kylinCfgGetModItems
函数描述:     从内存获取到指模块的所有条目配置
输入参数:     KYLIN_CFG_HANDLE:配置句柄
输出参数:       GList*
返回值:       其它：成功  NULL：失败
*************************************************/
KYLIN_CFG_API GList * kylinCfgGetModItems(KYLIN_CFG_HANDLE handle,char* modName);

/*************************************************
函数名称:     kylinCfgRegisterUserItem
函数描述:     注册指定条目的解析函数等，用于load时解析数据等
输入参数:     KYLIN_CFG_HANDLE:配置句柄
             st_ItemCfgInfo 条目值
输出参数:
返回值:       见KYLIN_COMMON_ERR_CODE
*************************************************/
KYLIN_CFG_API int kylinCfgRegisterUserItem(KYLIN_CFG_HANDLE handle,st_ItemCfgInfo * itemData);

/*************************************************
函数名称:     kylinCfgFlush
函数描述:     下刷内存配置到文件
输入参数:     KYLIN_CFG_HANDLE:配置句柄
输出参数:
返回值:       见KYLIN_COMMON_ERR_CODE
*************************************************/
KYLIN_CFG_API int kylinCfgFlush(KYLIN_CFG_HANDLE handle);

/*************************************************
函数名称:     kylinCfgLoad
函数描述:     从文件读配置到内存
输入参数:     KYLIN_CFG_HANDLE:配置句柄
            path:配置文件完整路径
输出参数:
返回值:       见KYLIN_COMMON_ERR_CODE
*************************************************/
KYLIN_CFG_API int kylinCfgLoad(KYLIN_CFG_HANDLE handle,char *path);

/*************************************************
函数名称:     kylinCfgShow
函数描述:     显示当前配置
输入参数:     KYLIN_CFG_HANDLE:配置句柄
输出参数:
返回值:       见KYLIN_COMMON_ERR_CODE
*************************************************/
KYLIN_CFG_API int kylinCfgShow(KYLIN_CFG_HANDLE handle);

/*************************************************
函数名称:     kylinCfgNewCommonItem
函数描述:     创建一个通用数据的配置条目
输入参数:     modName：模块名称
            itemName：条目名称
            data：数据
输出参数:     st_ItemCfgInfo*
返回值:       0：成功  其它：失败
*************************************************/
KYLIN_CFG_API st_ItemCfgInfo* kylinCfgNewCommonItem(char *modName,char *itemName,GVariant *data);

#ifdef __cplusplus
}
#endif

#endif
