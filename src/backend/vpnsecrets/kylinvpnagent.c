// SPDX-License-Identifier: GPL-2.0+
/*
 * Dan Williams <dcbw@redhat.com>
 *
 * Copyright 2011 - 2014 Red Hat, Inc.
 * Based on nm-applet's applet-agent.[ch] files.
 */

#include "kylinvpnagent.h"
#include <string.h>

#define KEYRING_UUID_TAG "connection-uuid"
#define KEYRING_SN_TAG "setting-name"
#define KEYRING_SK_TAG "setting-key"

static const SecretSchema network_manager_secret_schema = {
    "org.freedesktop.NetworkManager.Connection",
    SECRET_SCHEMA_DONT_MATCH_NAME,
    {
        { KEYRING_UUID_TAG, SECRET_SCHEMA_ATTRIBUTE_STRING },
        { KEYRING_SN_TAG, SECRET_SCHEMA_ATTRIBUTE_STRING },
        { KEYRING_SK_TAG, SECRET_SCHEMA_ATTRIBUTE_STRING },
        { NULL, 0 },
    }
};

G_DEFINE_TYPE (AppletAgent, applet_agent, NM_TYPE_SECRET_AGENT_OLD);

#define APPLET_AGENT_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), APPLET_TYPE_AGENT, AppletAgentPrivate))

typedef struct {
    GHashTable *requests;
    gboolean vpn_only;

    gboolean disposed;
} AppletAgentPrivate;

enum {
    GET_SECRETS,
    CANCEL_SECRETS,
    LAST_SIGNAL
};
static guint signals[LAST_SIGNAL] = { 0 };


/*******************************************************/

typedef struct {
    guint id;

    NMSecretAgentOld *agent;
    NMConnection *connection;
    char *path;
    char *setting_name;
    char **hints;
    guint32 flags;
    NMSecretAgentOldGetSecretsFunc get_callback;
    NMSecretAgentOldSaveSecretsFunc save_callback;
    NMSecretAgentOldDeleteSecretsFunc delete_callback;
    gpointer callback_data;

    GCancellable *cancellable;
    gint keyring_calls;
} Request;

// LCOV_EXCL_START
static Request *
request_new (NMSecretAgentOld *agent,
             NMConnection *connection,
             const char *connection_path,
             const char *setting_name,
             const char **hints,
             guint32 flags,
             NMSecretAgentOldGetSecretsFunc get_callback,
             NMSecretAgentOldSaveSecretsFunc save_callback,
             NMSecretAgentOldDeleteSecretsFunc delete_callback,
             gpointer callback_data)
{
    static guint32 counter = 1;
    Request *r;

    r = g_slice_new0 (Request);
    r->id = counter++;
    r->agent = agent;
    r->connection = g_object_ref (connection);
    r->path = g_strdup (connection_path);
    r->setting_name = g_strdup (setting_name);
    if (hints)
        r->hints = g_strdupv ((gchar **) hints);
    r->flags = flags;
    r->get_callback = get_callback;
    r->save_callback = save_callback;
    r->delete_callback = delete_callback;
    r->callback_data = callback_data;
    //r->cancellable = g_cancellable_new ();//外部指定
    return r;
}

static void
request_free (Request *r)
{
    /*
    if (!g_cancellable_is_cancelled (r->cancellable))
        g_hash_table_remove (APPLET_AGENT_GET_PRIVATE (r->agent)->requests, GUINT_TO_POINTER (r->id));
        */

    /* By the time the request is freed, all keyring calls should be completed */
    g_warn_if_fail (r->keyring_calls == 0);

    g_object_unref (r->connection);
    g_free (r->path);
    g_free (r->setting_name);
    g_strfreev (r->hints);
    g_object_unref (r->cancellable);
    memset (r, 0, sizeof (*r));
    g_slice_free (Request, r);
}

/*******************************************************/

static void
save_request_try_complete (Request *r)
{
    /* Only call the SaveSecrets callback and free the request when all the
     * secrets have been saved to the keyring.
     */
    if (r->keyring_calls == 0) {
        /* //mqtest 不用dbus回复 libnmqt做了
        if (!g_cancellable_is_cancelled (r->cancellable))
            r->save_callback (NM_SECRET_AGENT_OLD (r->agent), r->connection, NULL, r->callback_data);
            */
        request_free (r);
    }
}

static void
save_secret_cb (GObject *source,
                GAsyncResult *result,
                gpointer user_data)
{
    secret_password_store_finish (result, NULL);
    save_request_try_complete (user_data);
}



static GHashTable *
_create_keyring_add_attr_list (NMConnection *connection,
                               const char *setting_name,
                               const char *setting_key,
                               char **out_display_name)
{
    const char *connection_id, *connection_uuid;

    g_return_val_if_fail (connection != NULL, NULL);
    g_return_val_if_fail (setting_name != NULL, NULL);
    g_return_val_if_fail (setting_key != NULL, NULL);

    connection_uuid = nm_connection_get_uuid (connection);
    g_assert (connection_uuid);
    connection_id = nm_connection_get_id (connection);
    g_assert (connection_id);

    if (out_display_name) {
        *out_display_name = g_strdup_printf ("Network secret for %s/%s/%s",
                                             connection_id,
                                             setting_name,
                                             setting_key);
    }

    return secret_attributes_build (&network_manager_secret_schema,
                                    KEYRING_UUID_TAG, connection_uuid,
                                    KEYRING_SN_TAG, setting_name,
                                    KEYRING_SK_TAG, setting_key,
                                    NULL);
}

static void
save_one_secret (Request *r,
                 NMSetting *setting,
                 const char *key,
                 const char *secret,
                 const char *display_name)
{
    GHashTable *attrs;
    char *alt_display_name = NULL;
    const char *setting_name;
    NMSettingSecretFlags secret_flags = NM_SETTING_SECRET_FLAG_NONE;

    /* Don't system-owned or always-ask secrets */
    if (!nm_setting_get_secret_flags (setting, key, &secret_flags, NULL))
        return;
    if (secret_flags != NM_SETTING_SECRET_FLAG_AGENT_OWNED)
        return;

    setting_name = nm_setting_get_name (setting);
    g_assert (setting_name);

    attrs = _create_keyring_add_attr_list (r->connection,
                                           setting_name,
                                           key,
                                           display_name ? NULL : &alt_display_name);
    g_assert (attrs);

    secret_password_storev (&network_manager_secret_schema, attrs, NULL,
                            display_name ? display_name : alt_display_name, secret,
                            r->cancellable, save_secret_cb, r);
    r->keyring_calls++;

    g_hash_table_unref (attrs);
    g_free (alt_display_name);
}

static void
vpn_secret_iter_cb (const char *key, const char *secret, gpointer user_data)
{
    Request *r = user_data;
    NMSetting *setting;
    const char *service_name, *id;
    char *display_name;

    if (secret && strnlen (secret, sizeof(secret))) {
        setting = nm_connection_get_setting (r->connection, NM_TYPE_SETTING_VPN);
        g_assert (setting);
        service_name = nm_setting_vpn_get_service_type (NM_SETTING_VPN (setting));
        g_assert (service_name);
        id = nm_connection_get_id (r->connection);
        g_assert (id);

        display_name = g_strdup_printf ("VPN %s secret for %s/%s/" NM_SETTING_VPN_SETTING_NAME,
                                        key,
                                        id,
                                        service_name);
        save_one_secret (r, setting, key, secret, display_name);
        g_free (display_name);
    }
}

static void
write_one_secret_to_keyring (NMSetting *setting,
                             const char *key,
                             const GValue *value,
                             GParamFlags flags,
                             gpointer user_data)
{
    Request *r = user_data;
    GType type = G_VALUE_TYPE (value);
    const char *secret;

    /* Non-secrets obviously don't get saved in the keyring */
    if (!(flags & NM_SETTING_PARAM_SECRET))
        return;

    if (NM_IS_SETTING_VPN (setting) && (g_strcmp0 (key, NM_SETTING_VPN_SECRETS) == 0)) {
        g_return_if_fail (type == G_TYPE_HASH_TABLE);

        /* Process VPN secrets specially since it's a hash of secrets, not just one */
        nm_setting_vpn_foreach_secret (NM_SETTING_VPN (setting), vpn_secret_iter_cb, r);
    }
}

static void
save_delete_cb (NMSecretAgentOld *agent,
                NMConnection *connection,
                GError *error,
                gpointer user_data)
{
    Request *r = user_data;

    /* Ignore errors; now save all new secrets */
    nm_connection_for_each_setting_value (connection, write_one_secret_to_keyring, r);

    /* If no secrets actually got saved there may be nothing to do so
     * try to complete the request here.  If there were secrets to save the
     * request will get completed when those keyring calls return.
     */
    save_request_try_complete (r);
}


static void
applet_agent_init (AppletAgent *self)
{
    //保留函数体，防止编译问题

}

static void applet_agent_class_init (AppletAgentClass *agent_class)
{
    //保留函数体，防止编译问题
}

void
kylinAgentVpnSave (NMConnection *connection,char *path,GCancellable *cancellable)
{
    Request *r;

    r = request_new (NULL, connection, path, NULL, NULL, FALSE, NULL, NULL, NULL, NULL);
    //g_hash_table_insert (priv->requests, GUINT_TO_POINTER (r->id), r);//同步 只管理vpn不需要表
    r->cancellable=cancellable;

    /* Ignore errors; now save all new secrets */
    nm_connection_for_each_setting_value (connection, write_one_secret_to_keyring, r);

    /* If no secrets actually got saved there may be nothing to do so
     * try to complete the request here.  If there were secrets to save the
     * request will get completed when those keyring calls return.
     */
    save_request_try_complete (r);
}
// LCOV_EXCL_STOP

