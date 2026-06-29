// SPDX-License-Identifier: GPL-2.0+
/* NetworkManager Applet -- allow user control over networking
 *
 * Dan Williams <dcbw@redhat.com>
 *
 * Copyright 2004 - 2019 Red Hat, Inc.
 * (C) Copyright 2018 Lubomir Rintel
 * Based on nm-applet's applet-vpn-request.[ch] files.
 */

#ifndef KYLIN_VPN_REQUEST_H
#define KYLIN_VPN_REQUEST_H

#ifdef __cplusplus
extern "C"{
#endif


#include "kylinvpnagent.h"


typedef struct _SecretsRequest SecretsRequest;
typedef void (*SecretsRequestFreeFunc) (SecretsRequest *req);
struct _SecretsRequest {
         size_t totsize;
         gpointer reqid;
         char *setting_name;
         char **hints;
         guint32 flags;
         AppletAgent *agent;
         AppletAgentSecretsCallback callback;
         gpointer callback_data;

         NMConnection *connection;

         /* Class-specific stuff */
         SecretsRequestFreeFunc free_func;
};


typedef struct {
    GVariant *settings;
    int requestSta;
} KylinVpnSecretsResult;

SecretsRequest *
applet_secrets_request_new (size_t totsize,
                            NMConnection *connection,
                            gpointer request_id,
                            const char *setting_name,
                            const char **hints,
                            guint32 flags,
                            AppletAgentSecretsCallback callback,
                            gpointer callback_data,
                            AppletAgent *agent);
size_t applet_vpn_request_get_secrets_size(void);
void applet_secrets_request_complete (SecretsRequest *req,
                                      GVariant *settings,
                                      GError *error);
void applet_secrets_request_free (SecretsRequest *req);
void applet_secrets_request_set_free_func (SecretsRequest *req,
                                           SecretsRequestFreeFunc free_func);
gboolean applet_vpn_request_get_secrets(SecretsRequest *req, GError **error);

KylinVpnSecretsResult *kylinVpnSecretsResultGet();//获取请求状态 命名与上游不同区别之

#ifdef __cplusplus
}
#endif

#endif  /* APPLET_VPN_REQUEST_H */
