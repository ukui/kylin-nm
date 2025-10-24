
#ifndef __KYLIN_ASYNC_TASK_H__
#define __KYLIN_ASYNC_TASK_H__

#ifdef __cplusplus
extern "C" {
#endif

#define KYLIN_ASYNC_HANDLE  void*
#ifndef KYLIN_ASYNC_API
#define  KYLIN_ASYNC_API
#endif

/*用来代替gtask做异步任务，gtask不便拓展调试不便*/
/*注意：1.线程池大小自己按需定义，没有检查非法参数
	  2.执行函数中的参数传递，严禁使用栈上的，且需自己管理参数的生命周期
*/
#define KYLIN_ASYNC_DEFAULT_THREAD_NUM (2) //默认线程池2个线程

typedef void (*task_func)(void *arg);

/*************************************************
函数名称:     kylinAsyncTaskCreate
函数描述:     创建句柄
输入参数:
输出参数:     KYLIN_ASYNC_HANDLE:异步任务句柄
返回值:       其它：成功    NULL：失败
*************************************************/
KYLIN_ASYNC_API KYLIN_ASYNC_HANDLE kylinAsyncTaskCreate(int numThreads);

/*************************************************
函数名称:     kylinAsyncTaskAdd
函数描述:     创建句柄
输入参数:     KYLIN_ASYNC_HANDLE:配置句柄
		  task_func func:需要异步执行的函数
		  void *arg:需要异步执行的函数参数

输出参数:     
返回值:       返回任务号
*************************************************/
KYLIN_ASYNC_API int kylinAsyncTaskAdd(KYLIN_ASYNC_HANDLE handle, task_func func, void *arg) ;

/*************************************************
函数名称:     kylinAsyncTaskRemove
函数描述:     创建句柄
输入参数:	  KYLIN_ASYNC_HANDLE:配置句柄
		  task_id :需要移除的任务号由taskadd返回
输出参数:     
返回值:       0：成功   其它：失败
*************************************************/
KYLIN_ASYNC_API int kylinAsyncTaskRemove(KYLIN_ASYNC_HANDLE handle, int task_id);

/*************************************************
函数名称:     kylinAsyncTaskRelease
函数描述:     销毁所有异步任务并释放资源，销毁时会等待正在执行的任务执行完
输入参数:     KYLIN_ASYNC_HANDLE:配置句柄
输出参数:    
返回值:       
*************************************************/
KYLIN_ASYNC_API void kylinAsyncTaskRelease(KYLIN_ASYNC_HANDLE handle);

#ifdef __cplusplus
}
#endif

#endif /* __KYLIN_ASYNC_TASK_H__ */
