#ifndef KYLIN_COMMON_H
#define KYLIN_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <syslog.h> //mqtest

#define KYLIN_SHELL_LEVEL (-1)  //控制台显示打印等级 

	
enum KYLIN_COMMON_ERR_CODE{
	COMMON_SUCCESS= 0,	//成功
	COMMON_UNKOWN_ERR=1,
	COMMON_DO_FAILED,//执行失败
	COMMON_NULL_POINTER,//空指针
	COMMON_INVALID_PARM,//无效参数
	COMMON_PARM_MISMATCH,//参数不匹配
    COMMON_FILE_NOT_EXIST,//文件不存在
};

	
#define _LOGL_TRACE LOG_TRACE
#define _LOGL_DEBUG LOG_DEBUG
#define _LOGL_INFO  LOG_INFO
#define _LOGL_WARN  LOG_WARNING
#define _LOGL_ERR   LOG_ERR

void _KYLIN_COMMON_LOG(int level,int shellLevel,const char *format,...);//日志打印对接接口，不同平台改这个接口即可

#define KYLIN_COMMON_LOG(level,format,...) \
    _KYLIN_COMMON_LOG(level,0,"[%s][%d]" format,__FUNCTION__,__LINE__,##__VA_ARGS__)

#define  CHECK_POINTER_NULL(pointer)              \
		do{ 								  \
			if (pointer==NULL) {				   \
               _KYLIN_COMMON_LOG(_LOGL_ERR,0,"err:invalid Pointer!");\
			   return COMMON_NULL_POINTER;\
			} \
		}while(0)



/*获取系统绝对线性运行时间，授时不影响，单位ms*/
unsigned long long kylinGetSysRtime(void);

/*创建锁*/
void * kylinMuterCreate(void);

/*释放锁*/
int kylinMutexRelease( void * hMutex );

/*锁*/
int kylinMutexlock( void * hMutex , int time );

/*解锁*/
int kylinMutexUnlock( void * hMutex );

int kylinExeSystemCmd(const char *strCmd, char *strOutPut);

#ifdef __cplusplus
}
#endif


#endif /* KYLIN_COMMON_H */
