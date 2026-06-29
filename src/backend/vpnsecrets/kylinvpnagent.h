// SPDX-License-Identifier: GPL-2.0+
/*
 * Dan Williams <dcbw@redhat.com>
 *
 * Copyright 2011 - 2014 Red Hat, Inc.
 * Based on nm-applet's applet-agent.[ch] files.
 */

#ifndef __KYLIN_VPN_AGENT_H__
#define __KYLIN_VPN_AGENT_H__

#ifdef __cplusplus
extern "C"{
#endif

#include <libnm/NetworkManager.h>
#include <libnm/nm-secret-agent-old.h>
#include <gio/ginitable.h>
#include <gio/gdbusintrospection.h>
#include <libsecret/secret.h>

#define APPLET_TYPE_AGENT            (applet_agent_get_type ())
#define APPLET_AGENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), APPLET_TYPE_AGENT, AppletAgent))
#define APPLET_AGENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), APPLET_TYPE_AGENT, AppletAgentClass))
#define APPLET_IS_AGENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), APPLET_TYPE_AGENT))
#define APPLET_IS_AGENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), APPLET_TYPE_AGENT))
#define APPLET_AGENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), APPLET_TYPE_AGENT, AppletAgentClass))

#define APPLET_AGENT_GET_SECRETS "get-secrets"
#define APPLET_AGENT_CANCEL_SECRETS "cancel-secrets"

typedef struct {
    NMSecretAgentOld parent;
} AppletAgent;

typedef void (*AppletAgentSecretsCallback) (AppletAgent *self,
                                            GVariant *secrets,
                                            GError *error,
                                            gpointer user_data);

typedef struct {
    NMSecretAgentOldClass parent_class;

    void (*get_secrets)        (AppletAgent *self,
                                void *request_id,
                                NMConnection *connection,
                                const char *setting_name,
                                const char **hints,
                                guint32 flags,
                                AppletAgentSecretsCallback callback,
                                gpointer callback_data);

    void (*cancel_secrets)     (AppletAgent *self,
                                void *request_id);
} AppletAgentClass;


GType applet_agent_get_type (void) G_GNUC_CONST;


void applet_agent_handle_vpn_only (AppletAgent *agent, gboolean vpn_only);

void kylinAgentVpnSave (NMConnection *connection,char *path,GCancellable *cancellable);
#ifdef __cplusplus
}
#endif

#endif
