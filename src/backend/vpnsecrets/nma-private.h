// SPDX-License-Identifier: GPL-2.0+
/* NetworkManager Applet -- allow user control over networking
 *
 * Lubomir Rintel <lkundrak@v3.sk>
 *
 * Copyright (C) 2019 - 2021 Red Hat, Inc.
 */

#ifndef __NMA_PRIVATE_H__
#define __NMA_PRIVATE_H__


#if !GTK_CHECK_VERSION(4,0,0)
#define gtk_editable_set_text(editable,text)		gtk_entry_set_text(GTK_ENTRY(editable), (text))
#define gtk_editable_get_text(editable)			gtk_entry_get_text(GTK_ENTRY(editable))
#define gtk_editable_set_width_chars(editable, n_chars)	gtk_entry_set_width_chars(GTK_ENTRY(editable), (n_chars))
#define gtk_box_append(box, widget)			gtk_container_add(GTK_CONTAINER(box), (widget))
#define gtk_box_remove(box, widget)			gtk_container_remove(GTK_CONTAINER(box), (widget))
#define gtk_window_destroy(window)			gtk_widget_destroy(GTK_WIDGET (window))
#define gtk_check_button_get_active(button)		gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(button))
#define gtk_check_button_set_active(button, active)	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(button), active)
#define gtk_window_set_child(window, child)		gtk_container_add(GTK_CONTAINER (window), child)
#define gtk_image_new_from_icon_name(name)		gtk_image_new_from_icon_name((name), GTK_ICON_SIZE_BUTTON)
#define gtk_button_set_child(button, widget)		gtk_container_add(GTK_CONTAINER(button), (widget))
#define gtk_init()					gtk_init(&argc, &argv)
#define gtk_window_new()				gtk_window_new(GTK_WINDOW_TOPLEVEL)
#define gtk_widget_get_root(widget)			gtk_widget_get_toplevel(widget)
#define gtk_window_set_hide_on_close(window, hide)	g_signal_connect(dialog, "delete-event", G_CALLBACK (gtk_widget_hide_on_delete), NULL)

typedef void GtkRoot;
#endif

int nma_gtk_dialog_run (GtkDialog *dialog);//本来定义在nma-ui-utils.c中现定义在nma-vpn-password-dialog

#endif /* __NMA_PRIVATE_H__ */
