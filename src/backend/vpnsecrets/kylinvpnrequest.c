// SPDX-License-Identifier: GPL-2.0+
/* NetworkManager Applet -- allow user control over networking
 *
 * Dan Williams <dcbw@redhat.com>
 *
 * Copyright 2004 - 2019 Red Hat, Inc.
 * (C) Copyright 2018 Lubomir Rintel
 */

#define _GNU_SOURCE

#include "kylinvpnrequest.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/poll.h>
#include <unistd.h>
#include <errno.h>
#include <stddef.h>
#include <gtk/gtk.h>

#include "gsystem-local-alloc.h"
#include "nma-vpn-password-dialog.h"
#include "nm-macros-internal.h"
//#include "nm-compat.h"


/*****************************************************************************/

typedef struct {
	char *name;
	char *label;
	char *value;
	gboolean is_secret;
	gboolean should_ask;
} EuiSecret;

typedef struct {
	char *uuid;
	char *id;
	char *service_type;

	guint watch_id;
	GPid pid;

	int child_stdout;
	GString *child_response;
	GIOChannel *channel;
	guint channel_eventid;
	GVariantBuilder secrets_builder;
	gboolean external_ui_mode;

	/* These are just for the external UI mode */
	EuiSecret *eui_secrets;
	GtkDialog *dialog;
} RequestData;

typedef struct {
	SecretsRequest req;
	RequestData *req_data;
} VpnSecretsInfo;

int gVpnRequestSta=0;//vpn插件秘钥代理是否完成 0未开始 1已完成
KylinVpnSecretsResult gKylinVpnSecretsResult;

KylinVpnSecretsResult *kylinVpnSecretsResultGet()
{
    return &gKylinVpnSecretsResult;
}



gint
_nm_utils_ascii_str_to_bool (const char *str,
                             gint default_value)
{
    gsize len;
    char *s = NULL;

    if (!str)
        return default_value;

    while (str[0] && g_ascii_isspace (str[0]))
        str++;

    if (!str[0])
        return default_value;

    len = strnlen (str,4096);//此处实际是获取是否支持外部ui 实际远小于4096 4096是个极大值 解决安全热点问题
    if (g_ascii_isspace (str[len - 1])) {
        s = g_strdup (str);
        g_strchomp (s);
        str = s;
    }

    if (!g_ascii_strcasecmp (str, "true") || !g_ascii_strcasecmp (str, "yes") || !g_ascii_strcasecmp (str, "on") || !g_ascii_strcasecmp (str, "1"))
        default_value = TRUE;
    else if (!g_ascii_strcasecmp (str, "false") || !g_ascii_strcasecmp (str, "no") || !g_ascii_strcasecmp (str, "off") || !g_ascii_strcasecmp (str, "0"))
        default_value = FALSE;
    if (s)
        g_free (s);
    return default_value;
}

void
applet_secrets_request_complete (SecretsRequest *req,
                                  GVariant *settings,
                                  GError *error)
{




    //get_secrets_cb(NULL, error ? NULL : settings, error,req);
    //req->callback (req->agent, error ? NULL : settings, error, req->callback_data);
    gKylinVpnSecretsResult.requestSta=1;
    gKylinVpnSecretsResult.settings=settings;
}

void
applet_secrets_request_set_free_func (SecretsRequest *req,
                                      SecretsRequestFreeFunc free_func)
{
        req->free_func = free_func;
}

void
applet_secrets_request_free (SecretsRequest *req)
{
    g_return_if_fail (req != NULL);

    if (req->free_func)
        req->free_func (req);

    //secrets_reqs = g_slist_remove (secrets_reqs, req);//因为不是管理多个请求 只处理vpn 可以不处理链表

    g_object_unref (req->connection);
    g_free (req->setting_name);
    g_strfreev (req->hints);
    memset (req, 0, req->totsize);
    g_free (req);
}

SecretsRequest *
applet_secrets_request_new (size_t totsize,
                            NMConnection *connection,
                            gpointer request_id,
                            const char *setting_name,
                            const char **hints,
                            guint32 flags,
                            AppletAgentSecretsCallback callback,
                            gpointer callback_data,
                            AppletAgent *agent)
{
    SecretsRequest *req;

    g_return_val_if_fail (totsize >= sizeof (SecretsRequest), NULL);
    g_return_val_if_fail (connection != NULL, NULL);

    req = g_malloc0 (totsize);
    req->totsize = totsize;
    req->connection = g_object_ref (connection);
    req->reqid = request_id;
    req->setting_name = g_strdup (setting_name);
    req->hints = g_strdupv ((char **) hints);
    req->flags = flags;
    req->callback = callback;
    req->callback_data = callback_data;
    req->agent = agent;

    gKylinVpnSecretsResult.requestSta=0;
    if(gKylinVpnSecretsResult.settings){
        g_variant_unref(gKylinVpnSecretsResult.settings);
    }

#if GTK_CHECK_VERSION(3,90,0)
         gtk_init ();
#else
         int argc = 0;
         char **argv = NULL;
         gtk_init (&argc, &argv);
#endif
    return req;
}


/*****************************************************************************/

static void complete_request (VpnSecretsInfo *info);

/*****************************************************************************/

size_t
applet_vpn_request_get_secrets_size (void)
{
	return sizeof (VpnSecretsInfo);
}

/*****************************************************************************/

static void
external_ui_add_secrets (VpnSecretsInfo *info)
{
	RequestData *req_data = info->req_data;
	EuiSecret *secret;
	guint i;

	for (i = 0; req_data->eui_secrets[i].name; i++) {
        //printf("mqtest vpn come add name %s\n", req_data->eui_secrets[i].name);
		secret = &req_data->eui_secrets[i];
		if (   secret->is_secret
		    && secret->value
		    && secret->value[0]) {
			g_variant_builder_add (&req_data->secrets_builder, "{ss}",
			                       secret->name,
			                       secret->value);
            //printf("mqtest vpn add secrets name:%s value:%s\n",secret->name,secret->value);
		}
	}
}

static void
external_ui_dialog_response (GtkDialog *dialog, int response_id, gpointer user_data)
{
	VpnSecretsInfo *info = user_data;
	RequestData *req_data = info->req_data;
	NMAVpnPasswordDialog *vpn_dialog = NMA_VPN_PASSWORD_DIALOG (dialog);
	EuiSecret *secret;
	const char *value;
	guint i_secret, i_pw;

    printf("mqtest vpn come external_ui_dialog_response \n");
	for (i_secret = 0, i_pw = 0; req_data->eui_secrets[i_secret].name; i_secret++) {
		secret = &req_data->eui_secrets[i_secret];
		if (   secret->is_secret
		    && secret->should_ask) {
			switch (i_pw) {
			case 0:
				value = nma_vpn_password_dialog_get_password (vpn_dialog);
				break;
			case 1:
				value = nma_vpn_password_dialog_get_password_secondary (vpn_dialog);
				break;
			case 2:
				value = nma_vpn_password_dialog_get_password_ternary (vpn_dialog);
				break;
			default:
				continue;
			}
			g_free (secret->value);
			secret->value = g_strdup (value);
			i_pw++;
		}
	}

	gtk_widget_destroy (GTK_WIDGET (dialog));
	g_clear_object (&req_data->dialog);
	external_ui_add_secrets (info);
	complete_request (info);
}

static gboolean
external_ui_from_child_response (VpnSecretsInfo *info, GError **error)
{
	RequestData *req_data = info->req_data;
	gs_unref_keyfile GKeyFile *keyfile = NULL;
	gs_strfreev char **groups = NULL;
	NMAVpnPasswordDialog *dialog = NULL;
	gs_free char *version = NULL;
	gs_free char *title = NULL;
	gs_free char *message = NULL;
	gsize num_groups;
	guint num_ask = 0;
	guint i_group, i_secret, i_pw;
	gint response_id;
	EuiSecret *secret;
	const char *value;

	/* Parse response key file */
	keyfile = g_key_file_new ();

	if (!g_key_file_load_from_data (keyfile,
	                                req_data->child_response->str,
	                                req_data->child_response->len,
	                                G_KEY_FILE_NONE,
	                                error)) {
		return FALSE;
	}

	groups = g_key_file_get_groups (keyfile, &num_groups);
	if (g_strcmp0 (groups[0], "VPN Plugin UI") != 0) {
		g_set_error_literal (error,
		                     NM_SECRET_AGENT_ERROR,
		                     NM_SECRET_AGENT_ERROR_FAILED,
		                     "Expected [VPN Plugin UI]");
		return FALSE;
	}

	version = g_key_file_get_string (keyfile, "VPN Plugin UI", "Version", error);
	if (!version)
		return FALSE;
	if (strcmp (version, "2") != 0) {
		g_set_error_literal (error,
		                     NM_SECRET_AGENT_ERROR,
		                     NM_SECRET_AGENT_ERROR_FAILED,
		                     "Expected Version=2");
		return FALSE;
	}

	title = g_key_file_get_string (keyfile, "VPN Plugin UI", "Title", error);
	if (!title)
		return FALSE;

	message = g_key_file_get_string (keyfile, "VPN Plugin UI", "Description", error);
	if (!message)
		return FALSE;

	/* Create a secret instance for each group */
	req_data->eui_secrets = g_new0 (EuiSecret, num_groups);
	for (i_group = 1, i_secret = 0; i_group < num_groups; i_group++) {
		EuiSecret *secret = &req_data->eui_secrets[i_secret];
		const char *group = groups[i_group];
		char *label;

		label = g_key_file_get_string (keyfile, group, "Label", NULL);
		if (!label) {
			g_warning ("Skipping entry: no label\n");
			continue;
		}



		secret->name = g_strdup (group);
		secret->label = label;
		secret->value = g_key_file_get_string (keyfile, group, "Value", NULL);
		secret->is_secret = g_key_file_get_boolean (keyfile, group, "IsSecret", NULL);
		secret->should_ask = g_key_file_get_boolean (keyfile, group, "ShouldAsk", NULL);

        //printf("mqtest  external_ui_from_child_response group:%s secret->value:%s\n",group,secret->value);
		i_secret++;

		if (secret->is_secret && secret->should_ask)
			num_ask++;
	}

	/* If there are any secrets that must be asked to user,
	 * create a dialog and display it. */
	if (num_ask > 0) {
		dialog = (NMAVpnPasswordDialog *) nma_vpn_password_dialog_new (title, message, NULL);
        req_data->dialog = (GtkDialog *)g_object_ref_sink (dialog);

		nma_vpn_password_dialog_set_show_password (dialog, FALSE);
		nma_vpn_password_dialog_set_show_password_secondary (dialog, FALSE);
		nma_vpn_password_dialog_set_show_password_ternary (dialog, FALSE);

		for (i_secret = 0, i_pw = 0; req_data->eui_secrets[i_secret].name; i_secret++) {
			EuiSecret *secret = &req_data->eui_secrets[i_secret];

			if (   secret->is_secret
			    && secret->should_ask) {
				switch (i_pw) {
				case 0:
					nma_vpn_password_dialog_set_show_password (dialog, TRUE);
					nma_vpn_password_dialog_set_password_label (dialog, secret->label);
					if (secret->value)
						nma_vpn_password_dialog_set_password (dialog, secret->value);
					break;
				case 1:
					nma_vpn_password_dialog_set_show_password_secondary (dialog, TRUE);
					nma_vpn_password_dialog_set_password_secondary_label (dialog, secret->label);
					if (secret->value)
						nma_vpn_password_dialog_set_password_secondary (dialog, secret->value);
					break;
				case 2:
					nma_vpn_password_dialog_set_show_password_ternary (dialog, TRUE);
					nma_vpn_password_dialog_set_password_ternary_label (dialog, secret->label);
					if (secret->value)
						nma_vpn_password_dialog_set_password_ternary (dialog, secret->value);
					break;
				default:
					g_warning ("Skipping entry: more than 3 passwords not supported\n");
					continue;
				}
				i_pw++;
			}
		}
		gtk_widget_show (GTK_WIDGET (dialog));

		/* Synchronously wait for user response using gtk_dialog_run */
		response_id = gtk_dialog_run (GTK_DIALOG (dialog));

		/* User cancelled or dialog closed */
		if (response_id == GTK_RESPONSE_CANCEL || response_id == GTK_RESPONSE_DELETE_EVENT) {
			gtk_widget_destroy (GTK_WIDGET (dialog));
			g_clear_object (&req_data->dialog);
			return FALSE;
		}

		/* Process user input directly */
		for (i_secret = 0, i_pw = 0; req_data->eui_secrets[i_secret].name; i_secret++) {
			secret = &req_data->eui_secrets[i_secret];
			if (   secret->is_secret
			    && secret->should_ask) {
				switch (i_pw) {
				case 0:
					value = nma_vpn_password_dialog_get_password (dialog);
					break;
				case 1:
					value = nma_vpn_password_dialog_get_password_secondary (dialog);
					break;
				case 2:
					value = nma_vpn_password_dialog_get_password_ternary (dialog);
					break;
				default:
					continue;
				}
				g_free (secret->value);
				secret->value = g_strdup (value);
				i_pw++;
			}
		}

		gtk_widget_destroy (GTK_WIDGET (dialog));
		g_clear_object (&req_data->dialog);
		external_ui_add_secrets (info);
		complete_request (info);
		return TRUE;
	}

	/* Nothing to ask, return known secrets */
	external_ui_add_secrets (info);
	complete_request (info);
	return TRUE;
}

/*****************************************************************************/

static void
complete_request (VpnSecretsInfo *info)
{
	SecretsRequest *req = (SecretsRequest *) info;
	RequestData *req_data = info->req_data;
	GVariantBuilder settings_builder, vpn_builder;
    //gs_unref_variant GVariant *settings = NULL;
     GVariant *settings = NULL;

	g_variant_builder_init (&settings_builder, NM_VARIANT_TYPE_CONNECTION);
	g_variant_builder_init (&vpn_builder, NM_VARIANT_TYPE_SETTING);

	g_variant_builder_add (&vpn_builder, "{sv}",
	                       NM_SETTING_VPN_SECRETS,
	                       g_variant_builder_end (&req_data->secrets_builder));
	g_variant_builder_add (&settings_builder, "{sa{sv}}",
	                       NM_SETTING_VPN_SETTING_NAME,
	                       &vpn_builder);
	settings = g_variant_ref_sink (g_variant_builder_end (&settings_builder));

	applet_secrets_request_complete (req, settings, NULL);
    //applet_secrets_request_free (req);
}




/*****************************************************************************/

static void
_str_append (GString *str,
             const char *tag,
             const char *val)
{
	const char *s;
	gsize i;

	nm_assert (str);
	nm_assert (tag && tag[0]);
	nm_assert (val);

	g_string_append (str, tag);
	g_string_append_c (str, '=');

	s = strchr (val, '\n');
	if (s) {
		gs_free char *val2 = g_strdup (val);

		for (i = 0; val2[i]; i++) {
			if (val2[i] == '\n')
				val2[i] = ' ';
		}
		g_string_append (str, val2);
	} else
		g_string_append (str, val);
	g_string_append_c (str, '\n');
}

static char *
connection_to_data (NMConnection *connection,
                    gsize *out_length,
                    GError **error)
{
	NMSettingVpn *s_vpn;
	GString *buf;
	const char **keys;
	guint i, len;

	g_return_val_if_fail (NM_IS_CONNECTION (connection), NULL);

	s_vpn = nm_connection_get_setting_vpn (connection);
	if (!s_vpn) {
		g_set_error_literal (error,
		                     NM_SECRET_AGENT_ERROR,
		                     NM_SECRET_AGENT_ERROR_FAILED,
                             "Connection had no VPN setting");
		return NULL;
	}

	buf = g_string_new_len (NULL, 100);

	keys = nm_setting_vpn_get_data_keys (s_vpn, &len);
	for (i = 0; i < len; i++) {
		_str_append (buf, "DATA_KEY", keys[i]);
		_str_append (buf, "DATA_VAL", nm_setting_vpn_get_data_item (s_vpn, keys[i]));
	}
	nm_clear_g_free (&keys);

	keys = nm_setting_vpn_get_secret_keys (s_vpn, &len);
	for (i = 0; i < len; i++) {
		_str_append (buf, "SECRET_KEY", keys[i]);
		_str_append (buf, "SECRET_VAL", nm_setting_vpn_get_secret (s_vpn, keys[i]));
	}
	nm_clear_g_free (&keys);

	g_string_append (buf, "DONE\n\nQUIT\n\n");
	NM_SET_OUT (out_length, buf->len);
	return g_string_free (buf, FALSE);
}

/*****************************************************************************/

static gboolean
connection_to_fd (NMConnection *connection,
                  int fd,
                  GError **error)
{
	gs_free char *data = NULL;
	gsize data_len;
	gssize w;
	int errsv;

	data = connection_to_data (connection, &data_len, error);
	if (!data)
		return FALSE;

again:

    /*
   char tembuf[4096];
   snprintf(tembuf,sizeof(tembuf)-1,"%s",data);
   printf("mqtest connection_to_fd :%s \n",tembuf);
   */
	w = write (fd, data, data_len);
	if (w < 0) {
		errsv = errno;
		if (errsv == EINTR)
			goto again;
		g_set_error (error,
		             NM_SECRET_AGENT_ERROR,
		             NM_SECRET_AGENT_ERROR_FAILED,
                     "Failed to write connection to VPN UI: %s (%d)", g_strerror (errsv), errsv);
		return FALSE;
	}

	if ((gsize) w != data_len) {
		g_set_error_literal (error,
		                     NM_SECRET_AGENT_ERROR,
		                     NM_SECRET_AGENT_ERROR_FAILED,
                             "Failed to write connection to VPN UI: incomplete write");
		return FALSE;
	}

	return TRUE;
}

/*****************************************************************************/

static void
vpn_child_setup (gpointer user_data)
{
	/* We are in the child process at this point */
	pid_t pid = getpid ();
	setpgid (pid, pid);
}

static gboolean
auth_dialog_spawn (const char *con_id,
                   const char *con_uuid,
                   const char *const*hints,
                   const char *auth_dialog,
                   const char *service_type,
                   gboolean supports_hints,
                   gboolean external_ui_mode,
                   guint32 flags,
                   GPid *out_pid,
                   int *out_stdin,
                   int *out_stdout,
                   GError **error)
{
	gsize hints_len;
	gsize i, j;
	gs_free const char **argv = NULL;
	gs_free const char **envp = NULL;
	gsize environ_len;

	g_return_val_if_fail (con_id, FALSE);
	g_return_val_if_fail (con_uuid, FALSE);
	g_return_val_if_fail (auth_dialog, FALSE);
	g_return_val_if_fail (service_type, FALSE);
	g_return_val_if_fail (out_pid, FALSE);
	g_return_val_if_fail (out_stdin, FALSE);
	g_return_val_if_fail (out_stdout, FALSE);

	hints_len = NM_PTRARRAY_LEN (hints);
	argv = g_new (const char *, 11 + (2 * hints_len));
	i = 0;
	argv[i++] = auth_dialog;
	argv[i++] = "-u";
	argv[i++] = con_uuid;
	argv[i++] = "-n";
	argv[i++] = con_id;
	argv[i++] = "-s";
	argv[i++] = service_type;
	if (flags & NM_SECRET_AGENT_GET_SECRETS_FLAG_ALLOW_INTERACTION)
		argv[i++] = "-i";
	if (flags & NM_SECRET_AGENT_GET_SECRETS_FLAG_REQUEST_NEW)
		argv[i++] = "-r";
	for (j = 0; supports_hints && (j < hints_len); j++) {
		argv[i++] = "-t";
		argv[i++] = hints[j];
	}
	if (external_ui_mode)
		argv[i++] = "--external-ui-mode";
	nm_assert (i <= 10 + (2 * hints_len));
	argv[i++] = NULL;

	environ_len = NM_PTRARRAY_LEN (environ);
	envp = g_new (const char *, environ_len + 1);
	memcpy (envp, environ, sizeof (const char *) * environ_len);
	for (i = 0, j = 0; i < environ_len; i++) {
		const char *e = environ[i];

#if 1
		if (g_str_has_prefix (e, "G_MESSAGES_DEBUG=")) {
			/* skip this environment variable. We interact with the auth-dialog via stdout.
			 * G_MESSAGES_DEBUG may enable additional debugging messages from GTK. */
			continue;
		}
#endif
		envp[j++] = e;
	}
	envp[j] = NULL;

    /*
    printf("************\n");
    for(int k=0;argv[k];k++){
        printf("%s ",argv[k]);
    }
     printf("\n");
     printf("\n");
    for(int k=0;envp[k];k++){
        printf("%s ",envp[k]);
    }
    printf("************\n");
*/

	if (!g_spawn_async_with_pipes (NULL,
	                               (char **) argv,
	                               (char **) envp,
	                               G_SPAWN_DO_NOT_REAP_CHILD,
	                               vpn_child_setup,
	                               NULL,
	                               out_pid,
	                               out_stdin,
	                               out_stdout,
	                               NULL,
	                               error))
		return FALSE;

	return TRUE;
}

/*****************************************************************************/

static gboolean
ensure_killed (gpointer data)
{
	pid_t pid = GPOINTER_TO_INT (data);

	kill (pid, SIGKILL);
	waitpid (pid, NULL, 0);
	return FALSE;
}

static void
dialog_response_destroy (GtkDialog *dialog, int response_id, gpointer user_data)
{
	gtk_widget_destroy (GTK_WIDGET (dialog));
	g_object_unref (dialog);
}

static void
free_vpn_secrets_info (SecretsRequest *req)
{

    RequestData *req_data;
    guint i;

    req_data = ((VpnSecretsInfo *) req)->req_data;

    if (!req_data)
        return;

    g_free (req_data->uuid);
    g_free (req_data->id);
    g_free (req_data->service_type);

    nm_clear_g_source (&req_data->watch_id);

    nm_clear_g_source (&req_data->channel_eventid);
    if (req_data->channel)
        g_io_channel_unref (req_data->channel);

    if (req_data->pid) {
        if (kill (req_data->pid, SIGTERM) == 0)
            g_timeout_add_seconds (2, ensure_killed, GINT_TO_POINTER (req_data->pid));
        else {
            kill (req_data->pid, SIGKILL);
            waitpid (req_data->pid, NULL, 0);
        }
    }

    if (req_data->child_response)
        g_string_free (req_data->child_response, TRUE);

    g_variant_builder_clear (&req_data->secrets_builder);

    if (req_data->eui_secrets) {
        for (i = 0; req_data->eui_secrets[i].name; i++) {
            g_free (req_data->eui_secrets[i].name);
            g_free (req_data->eui_secrets[i].label);
            g_free (req_data->eui_secrets[i].value);
        }
        g_free (req_data->eui_secrets);
    }

    if (req_data->dialog) {
        g_signal_handlers_disconnect_by_func (req_data->dialog,
                                              external_ui_dialog_response,
                                              req);
        g_signal_connect (req_data->dialog,
                          "response",
                          G_CALLBACK (dialog_response_destroy),
                          NULL);
        req_data->dialog = NULL;
    }

    g_slice_free (RequestData, req_data);
}

gboolean
applet_vpn_request_get_secrets (SecretsRequest *req, GError **error)
{
	VpnSecretsInfo *info = (VpnSecretsInfo *) req;
	RequestData *req_data;
	NMSettingConnection *s_con;
	NMSettingVpn *s_vpn;
	const char *connection_type;
	const char *service_type;
	const char *auth_dialog;
	gs_unref_object NMVpnPluginInfo *plugin = NULL;
	int child_stdin;
	struct pollfd fds[1];
	int poll_ret;
	int status;
	int exit_status = 0;

	applet_secrets_request_set_free_func (req, free_vpn_secrets_info);

	s_con = nm_connection_get_setting_connection (req->connection);
	s_vpn = nm_connection_get_setting_vpn (req->connection);

	connection_type = nm_setting_connection_get_connection_type (s_con);
	g_return_val_if_fail (nm_streq0 (connection_type, NM_SETTING_VPN_SETTING_NAME), FALSE);

	service_type = nm_setting_vpn_get_service_type (s_vpn);
	g_return_val_if_fail (service_type, FALSE);

	plugin = nm_vpn_plugin_info_new_search_file (NULL, service_type);
	auth_dialog = plugin ? nm_vpn_plugin_info_get_auth_dialog (plugin) : NULL;
	if (!auth_dialog) {
		g_set_error (error,
		             NM_SECRET_AGENT_ERROR,
		             NM_SECRET_AGENT_ERROR_FAILED,
		             "Could not find the authentication dialog for VPN connection type '%s'",
		             service_type);
		return FALSE;
	}

	info->req_data = g_slice_new0 (RequestData);
	if (!info->req_data) {
		g_set_error_literal (error,
		                     NM_SECRET_AGENT_ERROR,
		                     NM_SECRET_AGENT_ERROR_FAILED,
		                     "Could not create VPN secrets request object");
		return FALSE;
	}
	req_data = info->req_data;

	g_variant_builder_init (&req_data->secrets_builder, G_VARIANT_TYPE ("a{ss}"));

	req_data->external_ui_mode = _nm_utils_ascii_str_to_bool (
		nm_vpn_plugin_info_lookup_property (plugin,
		                                    "GNOME",
		                                    "supports-external-ui-mode"),
		FALSE);



	if (!auth_dialog_spawn (nm_setting_connection_get_id (s_con),
	                        nm_setting_connection_get_uuid (s_con),
	                        (const char *const*) req->hints,
	                        auth_dialog,
	                        service_type,
	                        nm_vpn_plugin_info_supports_hints (plugin),
	                        req_data->external_ui_mode,
	                        req->flags,
	                        &req_data->pid,
	                        &child_stdin,
	                        &req_data->child_stdout,
	                        error))
		return FALSE;

	/* prepare stdout channel for reading */
	req_data->channel = g_io_channel_unix_new (req_data->child_stdout);
	req_data->child_response = g_string_sized_new (4096);
	g_io_channel_set_encoding (req_data->channel, NULL, NULL);

	/* write connection data to child stdin */
	if (!connection_to_fd (req->connection, child_stdin, error))
		return FALSE;
	close (child_stdin);

	/* setup poll for stdout */
	fds[0].fd = req_data->child_stdout;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	/* synchronous poll loop to read stdout and wait for child exit */
	while (TRUE) {
		/* check if child has exited using non-blocking waitpid */
		pid_t ret = waitpid (req_data->pid, &status, WNOHANG);
		if (ret == req_data->pid) {
			/* child has exited, break to process output */
			exit_status = status;
			break;
		}

		/* poll stdout with timeout */
		poll_ret = poll (fds, 1, 100);
		if (poll_ret > 0 && (fds[0].revents & (POLLIN | POLLHUP | POLLERR))) {
			GIOStatus io_status;
			char buf[4096];
			gsize bytes_read;

			io_status = g_io_channel_read_chars (req_data->channel, buf, sizeof(buf)-1, &bytes_read, NULL);
			if (io_status == G_IO_STATUS_NORMAL && bytes_read > 0) {
				g_string_append_len (req_data->child_response, buf, bytes_read);
			} else if (io_status == G_IO_STATUS_EOF || io_status == G_IO_STATUS_ERROR) {
				/* stdout closed unexpectedly, wait for child */
				break;
			}
			fds[0].revents = 0;
		} else if (poll_ret == -1 && errno != EINTR) {
			break;
		}
	}

	/* read any remaining output from stdout */
	while (TRUE) {
		char buf[4096];
		gsize bytes_read;
		GIOStatus io_status = g_io_channel_read_chars (req_data->channel, buf, sizeof(buf)-1, &bytes_read, NULL);
		if (io_status == G_IO_STATUS_NORMAL && bytes_read > 0) {
			g_string_append_len (req_data->child_response, buf, bytes_read);
		} else {
			break;
		}
	}

	/* wait for child to fully exit if not already */
	if (exit_status == 0) {
		waitpid (req_data->pid, &status, 0);
		exit_status = status;
	}

	/* mark child as exited so free_vpn_secrets_info won't try to kill it */
	req_data->pid = 0;

	/* process child response if successful */
	if (exit_status == 0) {
		if (req_data->external_ui_mode) {
			GError *eui_error = NULL;
			if (!external_ui_from_child_response (info, &eui_error)) {
				applet_secrets_request_complete (req, NULL, eui_error);
			}
		} else {
			char **lines = g_strsplit (req_data->child_response->str, "\n", -1);
			int i;

			for (i = 0; lines[i] && *(lines[i]); i += 2) {
				if (lines[i + 1] == NULL)
					break;
				g_variant_builder_add (&req_data->secrets_builder, "{ss}", lines[i], lines[i + 1]);
			}
			g_strfreev (lines);
			complete_request (info);
		}
		applet_secrets_request_free (req);
	} else {
		GError *exit_error = g_error_new (NM_SECRET_AGENT_ERROR,
		                                  NM_SECRET_AGENT_ERROR_USER_CANCELED,
		                                  "Child process exited with status %d", WEXITSTATUS(exit_status));
		applet_secrets_request_complete (req, NULL, exit_error);
		applet_secrets_request_free (req);
	}

	return TRUE;
}
