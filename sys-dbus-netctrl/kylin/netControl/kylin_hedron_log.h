#ifndef _KYLIN_HEDRON_H
#define _KYLIN_HEDRON_H

#ifdef __cplusplus
extern "C" {
#endif


/*************************************************
函数名称:     kylinSendHedronMessage
函数描述:     给域控发送日志信息
输入参数:     日志等级level:"error" "info"
		  message：消息内容
输出参数:
返回值:       见KYLIN_COMMON_ERR_CODE
*************************************************/
int kylinSendHedronMessage(char *level,char *message);//同步调用

#ifdef __cplusplus
}
#endif


#endif /* _KYLIN_HEDRON_H */

