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
#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <syslog.h> //mqtest

#include "kylin_cfgInfo.h"
#include "kylin_common.h"


#ifdef __cplusplus
extern "C" {
#endif

#define KYLIN_USERTYPE_NAME "kylinUser"
#define KYLIN_CFG_MAX_LINESIZE (4096)


typedef struct ST_CfgInfo
{
   FILE* fd;//文件句柄，预留
   char* cfgPath;//配置文件路径
   GHashTable *modHash;//二级表;
   GHashTable *registerHash;//表;
   void *mutex;
}st_CfgInfo;

static void kylinCfgFreeCb(void *itemData)
{
    st_ItemCfgInfo * pItemData=(st_ItemCfgInfo*)itemData;

     switch(pItemData->dataType)
     {
        case KEYLIN_CFG_USER:
         if(pItemData->userDataFreeCb) pItemData->userDataFreeCb(pItemData->data);
         break;
     default:
         g_variant_unref((GVariant*)pItemData->data);
        break;
     }
    g_free(pItemData->modName);
    g_free(pItemData->itemName);
    g_free(pItemData);
    return;
}

KYLIN_CFG_API st_ItemCfgInfo* kylinCfgNewCommonItem(char *modName,char *itemName,GVariant *data)
{
    st_ItemCfgInfo * pItemCfgInfo=(st_ItemCfgInfo*)g_malloc0(sizeof(st_ItemCfgInfo));
    pItemCfgInfo->dataType=KEYLIN_CFG_VARIANT;
    pItemCfgInfo->modName=g_strdup(modName);
    pItemCfgInfo->itemName=g_strdup(itemName);
    pItemCfgInfo->data=data;
    return pItemCfgInfo;
}


KYLIN_CFG_API KYLIN_CFG_HANDLE kylinCfgCreate()
{
    st_CfgInfo *pCfgInfo=NULL;

    pCfgInfo=(st_CfgInfo*)g_malloc0(sizeof(st_CfgInfo));
    if(!pCfgInfo)
    {
        KYLIN_COMMON_LOG(_LOGL_ERR,"malloc failed");
        return NULL;
    }
    pCfgInfo->mutex=kylinMuterCreate();
    if(!pCfgInfo->mutex)
    {
        KYLIN_COMMON_LOG(_LOGL_ERR,"malloc mutex failed");
        g_free(pCfgInfo);
        return NULL;
    }

    pCfgInfo->modHash =g_hash_table_new_full(g_str_hash, g_str_equal,g_free,(GDestroyNotify)g_hash_table_destroy);

    return (KYLIN_CFG_HANDLE)pCfgInfo;
}


KYLIN_CFG_API int kylinCfgDestroy(KYLIN_CFG_HANDLE handle)
{
    st_CfgInfo *pCfgInfo=(st_CfgInfo*)handle;
    CHECK_POINTER_NULL(pCfgInfo);

    //kylinCfgFlush(handle);//销毁还是先不flush，因为会覆盖配置文件，覆盖权限应该让调用者自己把握

    if(pCfgInfo->cfgPath) g_free(pCfgInfo->cfgPath);
    g_hash_table_destroy(pCfgInfo->modHash);
    g_hash_table_destroy(pCfgInfo->registerHash);
    kylinMutexRelease(pCfgInfo->mutex);
    if(pCfgInfo->fd) fclose(pCfgInfo->fd);
    g_free(pCfgInfo);

    return COMMON_SUCCESS;
}

/*kylinCfgRegisterUserItem注册item的解析方法用于上电读取配置文件时自定义解析不一定在该配置文件中有该配置项；
  kylinCfgSetItem是配置该配置文件的配置项，只要flush，配置文件就一定会有该配置项
  注意使用自定义数据类型时要先register或set不然load时无法解析
*/
KYLIN_CFG_API int kylinCfgRegisterUserItem(KYLIN_CFG_HANDLE handle,st_ItemCfgInfo * itemData)
{
    st_CfgInfo *pCfgInfo=(st_CfgInfo*)handle;
    CHECK_POINTER_NULL(pCfgInfo);
    CHECK_POINTER_NULL(itemData);
    GString *str = g_string_sized_new (KYLIN_CFG_MAX_NAMESIZE);
    if(!pCfgInfo->registerHash)
    {
        pCfgInfo->registerHash =g_hash_table_new_full(g_str_hash, g_str_equal,g_free,(GDestroyNotify)kylinCfgFreeCb);
    }
    g_string_append_printf (str, "%s.%s,", itemData->modName, itemData->itemName);

    g_hash_table_replace( pCfgInfo->registerHash,g_strdup(str->str),(gpointer)itemData);

    KYLIN_COMMON_LOG(_LOGL_INFO, "set modname:%s itemName:%s success!",itemData->modName,itemData->itemName);
    g_string_free(str,true);
    return COMMON_SUCCESS;
}

KYLIN_CFG_API int kylinCfgSetItem(KYLIN_CFG_HANDLE handle,st_ItemCfgInfo * itemData)
{
    GHashTable * ruleHash=NULL;
    st_CfgInfo *pCfgInfo=(st_CfgInfo*)handle;

    CHECK_POINTER_NULL(itemData);
    CHECK_POINTER_NULL(pCfgInfo);

    if (g_hash_table_lookup_extended (pCfgInfo->modHash,itemData->modName, NULL, (gpointer *) &ruleHash))
    {
        g_hash_table_replace(ruleHash,g_strdup(itemData->itemName),(gpointer)itemData);
        KYLIN_COMMON_LOG(_LOGL_INFO, "set modname:%s itemName:%s success!",itemData->modName,itemData->itemName);
        return COMMON_SUCCESS;
    }
    else
    {
        ruleHash =g_hash_table_new_full(g_str_hash, g_str_equal,g_free,(GDestroyNotify)kylinCfgFreeCb);
        g_hash_table_replace(pCfgInfo->modHash,g_strdup(itemData->modName), (gpointer)ruleHash);
        g_hash_table_replace(ruleHash,g_strdup(itemData->itemName),(gpointer)itemData);
        KYLIN_COMMON_LOG(_LOGL_INFO, "set modname:%s itemName:%s success!",itemData->modName,itemData->itemName);
        return COMMON_SUCCESS;

    }
    KYLIN_COMMON_LOG(_LOGL_ERR, "set modname:%s itemName:%s failed!",itemData->modName,itemData->itemName);
    return COMMON_DO_FAILED;
}

KYLIN_CFG_API st_ItemCfgInfo * kylinCfgGetItem(KYLIN_CFG_HANDLE handle,char* modName,char* itemName)
{
    GHashTable * ruleHash=NULL;
    st_CfgInfo *pCfgInfo=(st_CfgInfo*)handle;
    st_ItemCfgInfo* pItemCfgInfo=NULL;

    if(!modName || !itemName || !pCfgInfo)
    {
        KYLIN_COMMON_LOG(_LOGL_ERR, "pointer is null!");
        return NULL;
    }

    kylinMutexlock(pCfgInfo->mutex,-1);

    if (g_hash_table_lookup_extended (pCfgInfo->modHash, modName, NULL, (gpointer *) &ruleHash))
    {
        if(g_hash_table_lookup_extended (ruleHash,itemName, NULL, (gpointer *) &pItemCfgInfo))
        {
            kylinMutexUnlock(pCfgInfo->mutex);
            return pItemCfgInfo;
        }

        KYLIN_COMMON_LOG(_LOGL_ERR, "cant find itemName:%s!",itemName);
    }
    KYLIN_COMMON_LOG(_LOGL_ERR, "cant find modName:%s!",modName);
    kylinMutexUnlock(pCfgInfo->mutex);
    return NULL;
}


KYLIN_CFG_API GList * kylinCfgGetModItems(KYLIN_CFG_HANDLE handle,char* modName)
{
    GHashTable * ruleHash=NULL;
    st_CfgInfo *pCfgInfo=(st_CfgInfo*)handle;
    //st_ItemCfgInfo* pItemCfgInfo=NULL;
    GHashTableIter iter;
    GList *list = NULL;
    gpointer key, value;
    if(!modName || !pCfgInfo)
    {
        KYLIN_COMMON_LOG(_LOGL_ERR, "pointer is null!");
        return NULL;
    }

    kylinMutexlock(pCfgInfo->mutex,-1);

    if (g_hash_table_lookup_extended (pCfgInfo->modHash, modName, NULL, (gpointer *) &ruleHash))
    {
        g_hash_table_iter_init(&iter, ruleHash);
        while (g_hash_table_iter_next(&iter, &key,&value))
        {
            list=g_list_append(list,value);
            KYLIN_COMMON_LOG(_LOGL_INFO, "insert key:%s!",(char *)key);
        }

        kylinMutexUnlock(pCfgInfo->mutex);
        return list;
    }
    KYLIN_COMMON_LOG(_LOGL_ERR, "cant find modName:%s!",modName);
    kylinMutexUnlock(pCfgInfo->mutex);
    return NULL;
}

int kylinCfgVariantData2Str(void **cfgParm,void *realParm)
{
    CHECK_POINTER_NULL(realParm);
    CHECK_POINTER_NULL(cfgParm);
    GVariant *value=(GVariant *)realParm;
    *(char **)cfgParm=g_variant_print(value,false);
    return COMMON_SUCCESS;
}

static int kylinCfgStr2VariantData(void *cfgParm,void **realParm,char *typeStr)
{
    char *cfgStr=(char *)cfgParm;
    GVariant *variant=NULL;
    GError *error = NULL;
    GVariantType *variantType=g_variant_type_new(typeStr);
    variant = g_variant_parse (variantType,cfgStr,NULL,NULL, &error);
    *realParm=variant;
    g_variant_type_free(variantType);
    return COMMON_SUCCESS;
}

char* kylinCfgData2Str(st_ItemCfgInfo *pItemCfgInfo)
{
    char *valueStr=NULL;
    if(!pItemCfgInfo)
    {
        KYLIN_COMMON_LOG(_LOGL_ERR, "pItemCfgInfo is null!");
        return NULL;
    }

    if(pItemCfgInfo->toCfgCb)
    {
        pItemCfgInfo->toCfgCb((void **)&valueStr,pItemCfgInfo->data);
    }
    else
    {
        if(pItemCfgInfo->dataType==KEYLIN_CFG_VARIANT)
        {
            kylinCfgVariantData2Str((void **)&valueStr,pItemCfgInfo->data);
        }
    }
    return valueStr;
}

// LCOV_EXCL_START
st_ItemCfgInfo* kylinCfgStr2Item(KYLIN_CFG_HANDLE handle,char *modStr,char *nameStr,char* typeStr,char*  valueStr)
{
    st_ItemCfgInfo *pItemCfgInfo=NULL,*value=NULL;
    st_CfgInfo *pCfgInfo=(st_CfgInfo*)handle;

    GString *str = g_string_sized_new (KYLIN_CFG_MAX_NAMESIZE);

    pItemCfgInfo=(st_ItemCfgInfo*)g_malloc0(sizeof(st_ItemCfgInfo));
    if(!pItemCfgInfo)
    {
        KYLIN_COMMON_LOG(_LOGL_ERR, "pItemCfgInfo is null!");
        g_string_free(str,true);
        return NULL;
    }

    if(strcmp(typeStr,KYLIN_USERTYPE_NAME)==0)
    {
        g_string_append_printf (str, "%s.%s,", modStr, nameStr);
        if (g_hash_table_lookup_extended (pCfgInfo->registerHash, g_strdup(str->str), NULL, (gpointer *) &value))
        {
            memcpy(pItemCfgInfo,value,sizeof(st_ItemCfgInfo));
            if(pItemCfgInfo->fromCfgCb)
            {
                pItemCfgInfo->fromCfgCb(valueStr,&pItemCfgInfo->data);
                g_string_free(str,true);
                return pItemCfgInfo;
            }
        }
        KYLIN_COMMON_LOG(_LOGL_ERR, "cant find register %s %s !",modStr,nameStr);
    }
    else
    {
        pItemCfgInfo->modName=g_strdup(modStr);
        pItemCfgInfo->itemName=g_strdup(nameStr);
        pItemCfgInfo->dataType=KEYLIN_CFG_VARIANT;
        kylinCfgStr2VariantData(valueStr,&pItemCfgInfo->data,typeStr);
        g_string_free(str,true);
        return pItemCfgInfo;
    }

    g_free(pItemCfgInfo);
    g_string_free(str,true);
    return NULL;
}
// LCOV_EXCL_STOP


KYLIN_CFG_API int kylinCfgLoad(KYLIN_CFG_HANDLE handle,char *path)
{

    st_CfgInfo *pCfgInfo=(st_CfgInfo*)handle;
    st_ItemCfgInfo *pItemCfgInfo=NULL;
    FILE* fd=NULL;
    char nameStr[KYLIN_CFG_MAX_NAMESIZE]={0};
    char typeStr[KYLIN_CFG_MAX_NAMESIZE]={0};
    char valueStr[KYLIN_CFG_MAX_NAMESIZE]={0};
    char modName[KYLIN_CFG_MAX_NAMESIZE]={0};
    char buff[KYLIN_CFG_MAX_LINESIZE]={0};
    int ret=0;

    CHECK_POINTER_NULL(handle);

    if(!path)
    {
        KYLIN_COMMON_LOG(_LOGL_INFO,"path is null");
        return COMMON_NULL_POINTER;
    }

    KYLIN_COMMON_LOG(_LOGL_INFO,"load %s",path);
    pCfgInfo->cfgPath=g_strdup(path);
    if(!pCfgInfo->cfgPath)
    {
        KYLIN_COMMON_LOG(_LOGL_ERR,"malloc cfgPath failed");
        return COMMON_DO_FAILED;
    }

    if(access(path, F_OK)!=0)
    {
        KYLIN_COMMON_LOG(_LOGL_ERR,"path not exist %s",path);
        return COMMON_FILE_NOT_EXIST;
    }

    fd = fopen(pCfgInfo->cfgPath, "r");
    if (!fd)
    {
        KYLIN_COMMON_LOG(_LOGL_ERR,"load open file:%s failed",pCfgInfo->cfgPath);
        return COMMON_DO_FAILED;
    }

    if(!fgets(buff,KYLIN_CFG_MAX_LINESIZE-1,fd))
    {
        fclose(fd);
        KYLIN_COMMON_LOG(_LOGL_ERR,"file %s no content",pCfgInfo->cfgPath);
        return COMMON_DO_FAILED;
    }
    sscanf(buff, "%s %s %s\n", nameStr, typeStr, valueStr);
    KYLIN_COMMON_LOG(_LOGL_INFO,"%s %s %s", nameStr, typeStr, valueStr);

    if (strcmp(nameStr, "name") != 0)//检查文件头
    {
        fclose(fd);
        return -1;
    }

    while(fgets(buff,KYLIN_CFG_MAX_LINESIZE-1,fd)!=0)
    {
        ret=sscanf(buff,"%s %s %[^\n]", nameStr, typeStr, valueStr);
        KYLIN_COMMON_LOG(_LOGL_INFO,"%s %s %s ret:%d", nameStr, typeStr, valueStr,ret);
        if(ret==1)
        {
            if(strnlen(nameStr, KYLIN_CFG_MAX_NAMESIZE)>(KYLIN_CFG_MAX_NAMESIZE-1))
            {
                fclose(fd);
                return -1;
            }
            sscanf(nameStr,"[%[^]]",modName);//提取modname
        }
        else if(ret==3)
        {
            pItemCfgInfo=kylinCfgStr2Item(handle,modName,nameStr,typeStr,valueStr);
            kylinCfgSetItem(handle,pItemCfgInfo);
        }
        else
        {
            KYLIN_COMMON_LOG(_LOGL_ERR,"mismatch name:%s type:%s value:%s", nameStr, typeStr, valueStr);
            continue;
        }
    }

    fclose(fd);
    return 0;
}

KYLIN_CFG_API int kylinCfgFlush(KYLIN_CFG_HANDLE handle)
{
    FILE* fd;

    char *modName, *itemName;
    char *dataStr=NULL,*dataTypeStr=NULL;
    GHashTable *itemHash=NULL;
    GHashTableIter iter, iter2;
    gpointer value=NULL;
    st_ItemCfgInfo * itemData=NULL;
    CHECK_POINTER_NULL(handle);

    st_CfgInfo *pCfgInfo=(st_CfgInfo*)handle;
    kylinMutexlock(pCfgInfo->mutex,-1);

    KYLIN_COMMON_LOG(_LOGL_INFO,"flush %s",pCfgInfo->cfgPath);
    fd = fopen(pCfgInfo->cfgPath, "w");
    if (fd==NULL)
    {
        KYLIN_COMMON_LOG(_LOGL_ERR,"open file failed");
        kylinMutexUnlock(pCfgInfo->mutex);
        return COMMON_DO_FAILED;
    }

    fprintf(fd, "name type value\n");

    g_hash_table_iter_init (&iter, pCfgInfo->modHash);
    while (g_hash_table_iter_next (&iter, (gpointer *) &modName,(gpointer *) &itemHash))
    {
        if (!itemHash)
            continue;
         fprintf(fd, "[%s]\n",modName);

        g_hash_table_iter_init (&iter2, itemHash);
        while (g_hash_table_iter_next (&iter2, (gpointer *) &itemName,&value))
        {
            if (!value)
             continue;
            itemData=(st_ItemCfgInfo *)value;
            dataStr=kylinCfgData2Str(itemData);
            dataTypeStr=itemData->dataType==KEYLIN_CFG_VARIANT? (char*)g_variant_get_type_string((GVariant*)itemData->data):KYLIN_USERTYPE_NAME;
            if(dataStr){
                fprintf(fd, "%s %s %s\n",itemName,dataTypeStr,dataStr);
                g_free(dataStr);
                dataStr=NULL;
            }
        }
    }

    fflush(fd);
    fsync(fileno(fd));
    fclose(fd);
    pCfgInfo->fd=NULL;
    KYLIN_COMMON_LOG(_LOGL_INFO,"flush cfg to file:%s success",pCfgInfo->cfgPath);
    kylinMutexUnlock(pCfgInfo->mutex);
    return COMMON_SUCCESS;
}

int kylinCfgVerify(KYLIN_CFG_HANDLE handle,st_ItemCfgInfo *pItemCfgInfo)
{
    (void)handle;
    (void)pItemCfgInfo;

    /*TODO 没时间写完了，待完善，先留接口*/
	return 0;
}


KYLIN_CFG_API int kylinCfgShow(KYLIN_CFG_HANDLE handle)
{
    char *modName, *itemName;
    char *dataStr=NULL,*dataTypeStr=NULL;
    GHashTable *itemHash=NULL;
    GHashTableIter iter, iter2;
    gpointer value=NULL;
    st_ItemCfgInfo * itemData=NULL;
    CHECK_POINTER_NULL(handle);
    st_CfgInfo *pCfgInfo=(st_CfgInfo*)handle;

    KYLIN_COMMON_LOG(_LOGL_INFO,"###################################");
    g_hash_table_iter_init (&iter, pCfgInfo->modHash);
    while (g_hash_table_iter_next (&iter, (gpointer *) &modName,(gpointer *) &itemHash))
    {
        if (!itemHash)
            continue;
         KYLIN_COMMON_LOG(_LOGL_INFO,"[%s]",modName);

        g_hash_table_iter_init (&iter2, itemHash);
        while (g_hash_table_iter_next (&iter2, (gpointer *) &itemName,&value))
        {
            if (!value)
             continue;
            itemData=(st_ItemCfgInfo *)value;
            dataStr=kylinCfgData2Str(itemData);
            dataTypeStr=itemData->dataType==KEYLIN_CFG_VARIANT? (char*)g_variant_get_type_string((GVariant*)itemData->data):KYLIN_USERTYPE_NAME;
            if(dataStr){
                KYLIN_COMMON_LOG(_LOGL_INFO, "%s %s %s",itemName,dataTypeStr,dataStr);
                g_free(dataStr);
                dataStr=NULL;
            }
        }
    }
    KYLIN_COMMON_LOG(_LOGL_INFO,"###################################");
    return COMMON_SUCCESS;
}



#ifdef __cplusplus
}
#endif

