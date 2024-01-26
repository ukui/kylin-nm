// SPDX-License-Identifier: GPL-2.0+
/* NetworkManager Applet -- allow user control over networking
 *
 * Dan Williams <dcbw@redhat.com>
 *
 * Copyright 2004 - 2019 Red Hat, Inc.
 * (C) Copyright 2018 Lubomir Rintel
 */

#ifndef KYLIN_VPN_REQUEST_H
#define KYLIN_VPN_REQUEST_H

#include <gtk/gtk.h>
#include "kylinagent.h"

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


size_t applet_vpn_request_get_secrets_size(void);

gboolean applet_vpn_request_get_secrets(SecretsRequest *req, GError **error);

#endif  /* APPLET_VPN_REQUEST_H */
