// SPDX-License-Identifier: GPL-2.0+
/* NetworkManager Applet -- allow user control over networking
 *
 * Lubomir Rintel <lkundrak@v3.sk>
 *
 * Copyright (C) 2019 - 2021 Red Hat, Inc.
 */


#ifndef __RESOURCE_nma_H__
#define __RESOURCE_nma_H__

#include <gio/gio.h>

G_GNUC_INTERNAL GResource *nma_get_resource (void);
#endif
