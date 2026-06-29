/* nma-vpn-password-dialog.c - A password prompting dialog widget.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License as
 * published by the ree Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * Copyright (C) 1999, 2000 Eazel, Inc.
 * Copyright (C) 2011 - 2021 Red Hat, Inc.
 *
 * Authors: Ramiro Estrugo <ramiro@eazel.com>
 *          Dan Williams <dcbw@redhat.com>
 *          Lubomir Rintel <lkundrak@v3.sk>
 */

//#include "nm-default.h"
#include <gtk/gtk.h>
#include <NetworkManager.h>
#include "nma-private.h"
#include "nma-vpn-password-dialog.h"

typedef struct {
	GtkWidget *message_label;
	GtkWidget *password_label;
	GtkWidget *password_label_secondary;
	GtkWidget *password_label_tertiary;
	GtkWidget *password_entry;
	GtkWidget *password_entry_secondary;
	GtkWidget *password_entry_tertiary;
	GtkWidget *show_passwords_checkbox;

	GMainLoop *loop;
	gint response_id;
} NMAVpnPasswordDialogPrivate;

G_DEFINE_TYPE_WITH_CODE (NMAVpnPasswordDialog, nma_vpn_password_dialog, GTK_TYPE_DIALOG,
                         G_ADD_PRIVATE (NMAVpnPasswordDialog))


#define NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), \
                                                NMA_VPN_TYPE_PASSWORD_DIALOG, \
                                                NMAVpnPasswordDialogPrivate))

/* NMAVpnPasswordDialogClass methods */
static void nma_vpn_password_dialog_class_init (NMAVpnPasswordDialogClass *password_dialog_class);
static void nma_vpn_password_dialog_init       (NMAVpnPasswordDialog      *password_dialog);

/* GtkDialog callbacks */
static void dialog_show_callback (GtkWidget *widget, gpointer callback_data);
static void dialog_close_callback (GtkWidget *widget, gpointer callback_data);
static void dialog_response_callback (GtkWidget *widget, gint response_id, gpointer callback_data);

#if 0
int
nma_gtk_dialog_run (GtkDialog *dialog)
{
    NmaDialogData data;

    data.loop = g_main_loop_new (NULL, FALSE);
    g_signal_connect (dialog, "response", G_CALLBACK (nma_dialog_response), &data);

    gtk_window_set_hide_on_close (GTK_WINDOW (dialog), TRUE);
    gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);
    gtk_window_present (GTK_WINDOW (dialog));

    g_main_loop_run (data.loop);
    g_main_loop_unref (data.loop);

    gtk_widget_hide (GTK_WIDGET (dialog));

    return data.response_id;
}
#endif
/*****************************************************************************/

static void
show_passwords_toggled_cb (GtkWidget *widget, gpointer user_data)
{
	NMAVpnPasswordDialog *dialog = NMA_VPN_PASSWORD_DIALOG (user_data);
	NMAVpnPasswordDialogPrivate *priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);
	gboolean visible;

	visible = gtk_check_button_get_active (GTK_CHECK_BUTTON (widget));

	gtk_entry_set_visibility (GTK_ENTRY (priv->password_entry), visible);
	gtk_entry_set_visibility (GTK_ENTRY (priv->password_entry_secondary), visible);
	gtk_entry_set_visibility (GTK_ENTRY (priv->password_entry_tertiary), visible);
}

static void
entry_activate_cb (GtkButton *button, gpointer user_data)
{
	GtkDialog *dialog = GTK_DIALOG (user_data);

	gtk_dialog_response (dialog, GTK_RESPONSE_OK);
}

static void
nma_vpn_password_dialog_class_init (NMAVpnPasswordDialogClass *klass)
{
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	g_type_ensure (NM_TYPE_DEVICE);
	gtk_widget_class_set_template_from_resource (widget_class,
	                                             "/org/gnome/libnma/nma-vpn-password-dialog.ui");

	gtk_widget_class_bind_template_child_private (widget_class, NMAVpnPasswordDialog, message_label);
	gtk_widget_class_bind_template_child_private (widget_class, NMAVpnPasswordDialog, password_label);
	gtk_widget_class_bind_template_child_private (widget_class, NMAVpnPasswordDialog, password_label_secondary);
	gtk_widget_class_bind_template_child_private (widget_class, NMAVpnPasswordDialog, password_label_tertiary);
	gtk_widget_class_bind_template_child_private (widget_class, NMAVpnPasswordDialog, password_entry);
	gtk_widget_class_bind_template_child_private (widget_class, NMAVpnPasswordDialog, password_entry_secondary);
	gtk_widget_class_bind_template_child_private (widget_class, NMAVpnPasswordDialog, password_entry_tertiary);
	gtk_widget_class_bind_template_child_private (widget_class, NMAVpnPasswordDialog, show_passwords_checkbox);

	gtk_widget_class_bind_template_callback (widget_class, dialog_close_callback);
	gtk_widget_class_bind_template_callback (widget_class, dialog_show_callback);
	gtk_widget_class_bind_template_callback (widget_class, dialog_response_callback);
	gtk_widget_class_bind_template_callback (widget_class, entry_activate_cb);
	gtk_widget_class_bind_template_callback (widget_class, show_passwords_toggled_cb);
}

static void
nma_vpn_password_dialog_init (NMAVpnPasswordDialog *dialog)
{
    gtk_widget_init_template (GTK_WIDGET (dialog));
}

/* GtkDialog callbacks */
static void
dialog_show_callback (GtkWidget *widget, gpointer callback_data)
{
	NMAVpnPasswordDialog *dialog = NMA_VPN_PASSWORD_DIALOG (callback_data);
	NMAVpnPasswordDialogPrivate *priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);
	GtkWidget *to_focus = NULL;


	if (   gtk_widget_get_visible (priv->password_entry)
	    && gtk_entry_get_text_length (GTK_ENTRY (priv->password_entry)) == 0)
		to_focus = priv->password_entry;
	else if (   gtk_widget_get_visible (priv->password_entry_secondary)
	         && gtk_entry_get_text_length (GTK_ENTRY (priv->password_entry_secondary)) == 0)
		to_focus = priv->password_entry_secondary;
	else if (   gtk_widget_get_visible (priv->password_entry_tertiary)
	         && gtk_entry_get_text_length (GTK_ENTRY (priv->password_entry_tertiary)) == 0)
		to_focus = priv->password_entry_tertiary;

	gtk_widget_grab_focus (to_focus ?: priv->password_entry);
}

static void
dialog_close_callback (GtkWidget *widget, gpointer callback_data)
{
	gtk_widget_hide (widget);
}

static void
dialog_response_callback (GtkWidget *widget, gint response_id, gpointer callback_data)
{
	NMAVpnPasswordDialog *dialog = NMA_VPN_PASSWORD_DIALOG (callback_data);
	NMAVpnPasswordDialogPrivate *priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);

	priv->response_id = response_id;
	if (priv->loop == NULL)
		return;
	if (g_main_loop_is_running (priv->loop))
		g_main_loop_quit (priv->loop);
}

/* Public NMAVpnPasswordDialog methods */
GtkWidget *
nma_vpn_password_dialog_new (const char *title,
                             const char *message,
                             const char *password)
{
	GtkWidget *dialog;
	NMAVpnPasswordDialogPrivate *priv;

	dialog = g_object_new (NMA_VPN_TYPE_PASSWORD_DIALOG, "title", title, NULL);
	if (!dialog)
		return NULL;
	priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);

	if (message) {
		gtk_label_set_text (GTK_LABEL (priv->message_label), message);
		gtk_widget_show (priv->message_label);
	}

	nma_vpn_password_dialog_set_password (NMA_VPN_PASSWORD_DIALOG (dialog), password);
	
	return GTK_WIDGET (dialog);
}

#if 0
gboolean
nma_vpn_password_dialog_run_and_block (NMAVpnPasswordDialog *dialog)
{
	gint button_clicked;

	g_return_val_if_fail (dialog != NULL, FALSE);
	g_return_val_if_fail (NMA_VPN_IS_PASSWORD_DIALOG (dialog), FALSE);

	button_clicked = nma_gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_hide (GTK_WIDGET (dialog));

	return button_clicked == GTK_RESPONSE_OK;
}
#endif

void
nma_vpn_password_dialog_set_password (NMAVpnPasswordDialog	*dialog,
                                      const char *password)
{
	NMAVpnPasswordDialogPrivate *priv;

	g_return_if_fail (NMA_VPN_IS_PASSWORD_DIALOG (dialog));

	priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);
	gtk_editable_set_text (GTK_EDITABLE (priv->password_entry), password ? password : "");
}

void
nma_vpn_password_dialog_set_password_secondary (NMAVpnPasswordDialog *dialog,
                                                const char *password_secondary)
{
	NMAVpnPasswordDialogPrivate *priv;

	g_return_if_fail (NMA_VPN_IS_PASSWORD_DIALOG (dialog));

	priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);
	gtk_editable_set_text (GTK_EDITABLE (priv->password_entry_secondary),
	                    password_secondary ? password_secondary : "");
}

void
nma_vpn_password_dialog_set_password_ternary (NMAVpnPasswordDialog *dialog,
                                              const char *password_tertiary)
{
	NMAVpnPasswordDialogPrivate *priv;

	g_return_if_fail (NMA_VPN_IS_PASSWORD_DIALOG (dialog));

	priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);
	gtk_editable_set_text (GTK_EDITABLE (priv->password_entry_tertiary),
	                    password_tertiary ? password_tertiary : "");
}

void
nma_vpn_password_dialog_set_show_password (NMAVpnPasswordDialog *dialog, gboolean show)
{
	NMAVpnPasswordDialogPrivate *priv;

	g_return_if_fail (dialog != NULL);
	g_return_if_fail (NMA_VPN_IS_PASSWORD_DIALOG (dialog));

	priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);
	gtk_widget_set_visible (priv->password_label, show);
	gtk_widget_set_visible (priv->password_entry, show);
}

void
nma_vpn_password_dialog_set_show_password_secondary (NMAVpnPasswordDialog *dialog,
                                                     gboolean show)
{
	NMAVpnPasswordDialogPrivate *priv;

	g_return_if_fail (dialog != NULL);
	g_return_if_fail (NMA_VPN_IS_PASSWORD_DIALOG (dialog));

	priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);
	gtk_widget_set_visible (priv->password_label_secondary, show);
	gtk_widget_set_visible (priv->password_entry_secondary, show);
}

void
nma_vpn_password_dialog_set_show_password_ternary (NMAVpnPasswordDialog *dialog,
                                                   gboolean show)
{
	NMAVpnPasswordDialogPrivate *priv;

	g_return_if_fail (dialog != NULL);
	g_return_if_fail (NMA_VPN_IS_PASSWORD_DIALOG (dialog));

	priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);
	gtk_widget_set_visible (priv->password_label_tertiary, show);
	gtk_widget_set_visible (priv->password_entry_tertiary, show);
}


const char *
nma_vpn_password_dialog_get_password (NMAVpnPasswordDialog *dialog)
{
	NMAVpnPasswordDialogPrivate *priv;

	g_return_val_if_fail (NMA_VPN_IS_PASSWORD_DIALOG (dialog), NULL);

	priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);
	return gtk_editable_get_text (GTK_EDITABLE (priv->password_entry));
}

const char *
nma_vpn_password_dialog_get_password_secondary (NMAVpnPasswordDialog *dialog)
{
	NMAVpnPasswordDialogPrivate *priv;

	g_return_val_if_fail (NMA_VPN_IS_PASSWORD_DIALOG (dialog), NULL);

	priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);
	return gtk_editable_get_text (GTK_EDITABLE (priv->password_entry_secondary));
}

const char *
nma_vpn_password_dialog_get_password_ternary (NMAVpnPasswordDialog *dialog)
{
	NMAVpnPasswordDialogPrivate *priv;

	g_return_val_if_fail (NMA_VPN_IS_PASSWORD_DIALOG (dialog), NULL);

	priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);
	return gtk_editable_get_text (GTK_EDITABLE (priv->password_entry_tertiary));
}

void nma_vpn_password_dialog_set_password_label (NMAVpnPasswordDialog *dialog,
                                                 const char *label)
{
	NMAVpnPasswordDialogPrivate *priv;

	g_return_if_fail (dialog != NULL);
	g_return_if_fail (NMA_VPN_IS_PASSWORD_DIALOG (dialog));

	priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);

	gtk_label_set_text_with_mnemonic (GTK_LABEL (priv->password_label), label);
}

void nma_vpn_password_dialog_set_password_secondary_label (NMAVpnPasswordDialog *dialog,
                                                           const char *label)
{
	NMAVpnPasswordDialogPrivate *priv;

	g_return_if_fail (dialog != NULL);
	g_return_if_fail (NMA_VPN_IS_PASSWORD_DIALOG (dialog));

	priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);

	gtk_label_set_text_with_mnemonic (GTK_LABEL (priv->password_label_secondary), label);
}

void
nma_vpn_password_dialog_set_password_ternary_label (NMAVpnPasswordDialog *dialog,
                                                    const char *label)
{
	NMAVpnPasswordDialogPrivate *priv;

	g_return_if_fail (dialog != NULL);
	g_return_if_fail (NMA_VPN_IS_PASSWORD_DIALOG (dialog));

	priv = NMA_VPN_PASSWORD_DIALOG_GET_PRIVATE (dialog);

	gtk_label_set_text_with_mnemonic (GTK_LABEL (priv->password_label_tertiary), label);
}
