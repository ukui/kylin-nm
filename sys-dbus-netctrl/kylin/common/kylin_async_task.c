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
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <pthread.h>
#include <time.h>
#include <stdatomic.h>
#include <unistd.h>
#include  <string.h>



#include "kylin_common.h"
#include "kylin_async_task.h"


typedef enum {
    TASK_PENDING,
    TASK_RUNNING,
    TASK_COMPLETED,
    TASK_TIMEOUT,
    TASK_CANCELED
} TaskState;


typedef struct ST_KylinAsyncTask {
    int id;
    task_func func;
    void *arg;//用户参数
    volatile TaskState state;
} st_KylinAsyncTask;

typedef struct QueueNode {
    st_KylinAsyncTask *task;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *head;
    QueueNode *tail;
    void* hMutex;
    pthread_cond_t cond;
    int thread_count;
    pthread_t *threads;
    volatile int running;
    atomic_int task_id_counter;
} st_KylinAsyncThreadPool;


void queue_push(st_KylinAsyncThreadPool *pool, st_KylinAsyncTask *task) {
    QueueNode *node = malloc(sizeof(QueueNode));
    node->task = task;
    node->next = NULL;

    kylinMutexlock(pool->hMutex,-1);
    if (pool->tail) {
        pool->tail->next = node;
    } else {
        pool->head = node;
    }
    pool->tail = node;
    pthread_cond_signal(&pool->cond);
    kylinMutexUnlock(pool->hMutex);
}

st_KylinAsyncTask *queue_pop(st_KylinAsyncThreadPool *pool) 
{

    kylinMutexlock(pool->hMutex,-1);
	
    while (pool->running && !pool->head) {
        pthread_cond_wait(&pool->cond, pool->hMutex);
    }

    if (!pool->running && !pool->head) {
        kylinMutexUnlock(pool->hMutex);
        return NULL;
    }

    QueueNode *node = pool->head;
    st_KylinAsyncTask *task = node->task;
    pool->head = node->next;
    if (!pool->head) pool->tail = NULL;

	kylinMutexUnlock(pool->hMutex);
    
    free(node);
    return task;
}

void* kylinAsyncWorkerThread(void *arg) 
{
    st_KylinAsyncThreadPool *pool = (st_KylinAsyncThreadPool*)arg;
    
    while (pool->running) {
        st_KylinAsyncTask *task = queue_pop(pool);//已阻塞休眠，无需手动sleep
        if (!task) 
		{
			KYLIN_COMMON_LOG(_LOGL_INFO,"invalid task drop it");
			continue;
        }

        // 检查任务状态
        if (task->state == TASK_CANCELED) {
            free(task);
            continue;
        }

        // 执行任务
        task->state = TASK_RUNNING;
        task->func(task->arg);
        task->state = TASK_COMPLETED;
        free(task);
    }
    return NULL;
}

KYLIN_ASYNC_API KYLIN_ASYNC_HANDLE kylinAsyncTaskCreate(int numThreads) 
{
	st_KylinAsyncThreadPool *pool=malloc(sizeof(st_KylinAsyncThreadPool));

	if(!pool) 
	{
		KYLIN_COMMON_LOG(_LOGL_ERR,"malloc failed");
        return NULL;
	}
	memset(pool,0,sizeof(st_KylinAsyncThreadPool));
	pool->thread_count = numThreads;
    pool->threads = malloc(numThreads * sizeof(pthread_t));
	if(!pool->threads) 
	{
		KYLIN_COMMON_LOG(_LOGL_ERR,"malloc thread failed");
		free(pool);
        return NULL;
	}
    pool->running = 1;
    atomic_init(&pool->task_id_counter, 0);

	pool->hMutex=kylinMuterCreate();
	if(!pool->hMutex) 
	{
		KYLIN_COMMON_LOG(_LOGL_ERR,"create thread hMutex failed");
		free(pool->threads);
		free(pool);
		return NULL;
	}

    pthread_cond_init(&pool->cond, NULL);
    
    for (int i = 0; i < numThreads; i++) {
        pthread_create(&pool->threads[i], NULL, kylinAsyncWorkerThread, pool);
    }
	KYLIN_COMMON_LOG(_LOGL_INFO,"create kylin aysnc task pool:%d success",numThreads);
	return pool;
}

KYLIN_ASYNC_API int kylinAsyncTaskAdd(KYLIN_ASYNC_HANDLE handle, task_func func, void *arg) 
{
	st_KylinAsyncThreadPool *pool=(st_KylinAsyncThreadPool *)handle;
	CHECK_POINTER_NULL(pool);
	
    st_KylinAsyncTask *task = malloc(sizeof(st_KylinAsyncTask));
    task->id = atomic_fetch_add(&pool->task_id_counter, 1);
    task->func = func;
    task->arg = arg;
    task->state = TASK_PENDING;
    
    queue_push(pool, task);
	
	KYLIN_COMMON_LOG(_LOGL_INFO,"add kylin aysnc taskid:%d success",task->id);
    return task->id;
}

KYLIN_ASYNC_API int kylinAsyncTaskRemove(KYLIN_ASYNC_HANDLE handle, int task_id) 
{
	st_KylinAsyncThreadPool *pool=(st_KylinAsyncThreadPool *)handle;
	CHECK_POINTER_NULL(pool);
	kylinMutexlock(pool->hMutex,-1);
	
	QueueNode** prev = &pool->head;
	QueueNode* curr = pool->head;
    
    while (curr) {
        if (curr->task->id == task_id && curr->task->state == TASK_PENDING) {
            *prev = curr->next;
            if (!curr->next) pool->tail = *prev;
            
            curr->task->state = TASK_CANCELED;
            free(curr->task);
            free(curr);
            break;
        }
        prev = &curr->next;
        curr = curr->next;
    }
	kylinMutexUnlock(pool->hMutex);
	
	KYLIN_COMMON_LOG(_LOGL_INFO,"remove kylin aysnc taskid:%d success",task_id);
	return COMMON_SUCCESS;
}

KYLIN_ASYNC_API void kylinAsyncTaskRelease(KYLIN_ASYNC_HANDLE handle) 
{
	st_KylinAsyncThreadPool *pool=(st_KylinAsyncThreadPool *)handle;
	if(!pool)
	{	
		KYLIN_COMMON_LOG(_LOGL_INFO,"duplicate release");
		return ;
	}

    pool->running = 0;
    pthread_cond_broadcast(&pool->cond);
    
    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }
    
    kylinMutexRelease(pool->hMutex);
    pthread_cond_destroy(&pool->cond);
    free(pool->threads);
	free(pool);
	KYLIN_COMMON_LOG(_LOGL_INFO,"kylin async task release success");
}

#ifdef __cplusplus
}
#endif


