/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2022 Tianjin KYLIN Information Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>  
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#ifdef __linux__
#include <fcntl.h>
#include <sys/prctl.h>
#endif /* __linux__ */
#include <pthread.h>

#include "kylin_common.h"
#include "qdebug.h"



#ifdef __cplusplus
extern "C" {
#endif


#include <syslog.h>
#define OUTPUT_BUF_MAX 4096


void _KYLIN_COMMON_LOG(int level,int shellLevel,const char *format,...)//日志打印对接接口，不同平台改这个接口即可
{
     char    acLogBuf[2048] = {0};
     va_list vaList;

     va_start(vaList, format);
     vsnprintf(acLogBuf, 2048, format, vaList);
     va_end(vaList);

    if(KYLIN_SHELL_LEVEL>=shellLevel) printf("%s \n", acLogBuf);//todo 改这，未来可对接sdk的日志模块
    syslog(level,"%s", acLogBuf);//todo 改这，未来可对接sdk的日志模块
    /*
    switch (level) {
    case _LOGL_DEBUG:
        qDebug()<<acLogBuf;
        break;
    case _LOGL_INFO:
        qInfo()<<acLogBuf;
        break;
    case _LOGL_WARN:
        qWarning()<<acLogBuf;
        break;
    case _LOGL_ERR:
        qCritical()<<acLogBuf;
    default:
        qDebug()<<acLogBuf;
        break;
    }
    */
    return ;
}


void * kylinMuterCreate(void)
{
    void * hMutex = NULL;

    pthread_mutex_t * pMutex =(pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    if(pthread_mutex_init(pMutex, NULL))
    {
        free(pMutex);
        return(NULL);
    }
    hMutex = pMutex;

    return hMutex;
}

int kylinMutexRelease( void * hMutex )
{
	if (!hMutex)
	{
		return -1;
	}
    pthread_mutex_destroy((pthread_mutex_t *)hMutex);
    free(hMutex);
    return 0;
}

int kylinMutexlock( void * hMutex , int time )
{
	(void)time;//接口设计,其他平台有时间,占位
	if(hMutex == NULL)
	{
		return -1;
	}
    pthread_mutex_lock((pthread_mutex_t *)hMutex);
    return 0;
}

int kylinMutexUnlock( void * hMutex )
{
	if(hMutex == NULL)
	{
		return -1;
	}
    pthread_mutex_unlock((pthread_mutex_t *)hMutex);
    return 0;
}

unsigned long long kylinGetSysRtime(void)
{
	unsigned long long runTimeMs=0;
    struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC,&ts);
	runTimeMs=ts.tv_sec*1000+ts.tv_nsec/1000000;
	return runTimeMs;
}


int kylinExeSystemCmd(const char *strCmd, char *strOutPut)
{
    FILE *fstream = NULL;
    char buff[2048]={0};

#if 1
    if (NULL == (fstream = popen(strCmd, "r")))
#else
    if ( NULL == (fstream = vpopen(strCmd, "r")) )
#endif
    {
        return errno;
    }

    if (strOutPut != NULL)
    {
        while (NULL!= fgets(buff, sizeof(buff), fstream))
        {
            snprintf(strOutPut + strnlen(strOutPut, OUTPUT_BUF_MAX), OUTPUT_BUF_MAX - strnlen(strOutPut, OUTPUT_BUF_MAX), "%s", buff);
        }
    }
#if 1
    pclose(fstream);
#else
    vpclose(fstream);
#endif
    return 0;
}


#ifdef __cplusplus
}
#endif
