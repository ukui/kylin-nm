// SPDX-License-Identifier: LGPL-2.1+
/* NetworkManager Applet -- allow user control over networking
 *
 * Copyright (C) 2011-2022 Red Hat, Inc.
 */

#ifndef NMA_VERSION_H
#define NMA_VERSION_H

#include <glib.h>

/**
 * NMA_MAJOR_VERSION:
 *
 * Evaluates to the major version number of NetworkManager which this source
 * is compiled against.
 */
#define NMA_MAJOR_VERSION (1)

/**
 * NMA_MINOR_VERSION:
 *
 * Evaluates to the minor version number of NetworkManager which this source
 * is compiled against.
 */
#define NMA_MINOR_VERSION (10)

/**
 * NMA_MICRO_VERSION:
 *
 * Evaluates to the micro version number of NetworkManager which this source
 * compiled against.
 */
#define NMA_MICRO_VERSION (6)

/**
 * NMA_CHECK_VERSION:
 * @major: major version (e.g. 1 for version 1.2.5)
 * @minor: minor version (e.g. 2 for version 1.2.5)
 * @micro: micro version (e.g. 5 for version 1.2.5)
 *
 * Returns: %TRUE if the version of the NetworkManager header files
 * is the same as or newer than the passed-in version.
 */
#define NMA_CHECK_VERSION(major,minor,micro)                         \
    (NMA_MAJOR_VERSION > (major) ||                                  \
     (NMA_MAJOR_VERSION == (major) && NMA_MINOR_VERSION > (minor)) || \
     (NMA_MAJOR_VERSION == (major) && NMA_MINOR_VERSION == (minor) && NMA_MICRO_VERSION >= (micro)))

#define NMA_ENCODE_VERSION(major,minor,micro) ((major) << 16 | (minor) << 8 | (micro))

#define NMA_VERSION_1_2    (NMA_ENCODE_VERSION (1, 2, 0))
#define NMA_VERSION_1_4    (NMA_ENCODE_VERSION (1, 4, 0))
#define NMA_VERSION_1_8    (NMA_ENCODE_VERSION (1, 8, 0))
#define NMA_VERSION_1_8_2  (NMA_ENCODE_VERSION (1, 8, 2))
#define NMA_VERSION_1_8_4  (NMA_ENCODE_VERSION (1, 8, 4))
#define NMA_VERSION_1_8_6  (NMA_ENCODE_VERSION (1, 8, 6))
#define NMA_VERSION_1_8_8  (NMA_ENCODE_VERSION (1, 8, 8))
#define NMA_VERSION_1_8_10 (NMA_ENCODE_VERSION (1, 8, 10))
#define NMA_VERSION_1_8_12 (NMA_ENCODE_VERSION (1, 8, 12))
#define NMA_VERSION_1_8_22 (NMA_ENCODE_VERSION (1, 8, 22))
#define NMA_VERSION_1_8_26 (NMA_ENCODE_VERSION (1, 8, 26))
#define NMA_VERSION_1_8_28 (NMA_ENCODE_VERSION (1, 8, 28))
#define NMA_VERSION_1_8_34 (NMA_ENCODE_VERSION (1, 8, 34))
#define NMA_VERSION_1_8_36 (NMA_ENCODE_VERSION (1, 8, 36))
#define NMA_VERSION_1_10_6 (NMA_ENCODE_VERSION (1, 10, 6))

/* For releases, NMA_API_VERSION is equal to NMA_VERSION.
 *
 * For development builds, NMA_API_VERSION is the next
 * stable API after NMA_VERSION. When you run a development
 * version, you are already using the future API, even if
 * it is not yet release. Hence, the currently used API
 * version is the future one.  */
#define NMA_API_VERSION \
    (((NMA_MINOR_VERSION % 2) == 1) \
        ? NMA_ENCODE_VERSION (NMA_MAJOR_VERSION, NMA_MINOR_VERSION + 1, 0                               ) \
        : NMA_ENCODE_VERSION (NMA_MAJOR_VERSION, NMA_MINOR_VERSION    , ((NMA_MICRO_VERSION + 1) / 2) * 2))

/* deprecated. */
#define NMA_VERSION_CUR_STABLE  NMA_API_VERSION

/* deprecated. */
#define NMA_VERSION_NEXT_STABLE NMA_API_VERSION

#define NMA_VERSION NMA_ENCODE_VERSION (NMA_MAJOR_VERSION, NMA_MINOR_VERSION, NMA_MICRO_VERSION)

/*****************************************************************************/

#if !defined (NMA_VERSION_MIN_REQUIRED) || (NMA_VERSION_MIN_REQUIRED == 0)
# undef NMA_VERSION_MIN_REQUIRED
# define NMA_VERSION_MIN_REQUIRED (NMA_API_VERSION)
#endif

#if !defined (NMA_VERSION_MAX_ALLOWED) || (NMA_VERSION_MAX_ALLOWED == 0)
# undef NMA_VERSION_MAX_ALLOWED
# define NMA_VERSION_MAX_ALLOWED (NMA_API_VERSION)
#endif

/* sanity checks */
#if NMA_VERSION_MIN_REQUIRED > NMA_API_VERSION
#error "NMA_VERSION_MIN_REQUIRED must be <= NMA_API_VERSION"
#endif
#if NMA_VERSION_MAX_ALLOWED < NMA_VERSION_MIN_REQUIRED
#error "NMA_VERSION_MAX_ALLOWED must be >= NMA_VERSION_MIN_REQUIRED"
#endif
#if NMA_VERSION_MIN_REQUIRED < NMA_VERSION_1_2
#error "NMA_VERSION_MIN_REQUIRED must be >= NMA_VERSION_1_2"
#endif

/*****************************************************************************/

/* Deprecation / Availability macros */

#if NMA_VERSION_MIN_REQUIRED >= NMA_VERSION_1_2
# define NMA_DEPRECATED_IN_1_2           G_DEPRECATED
# define NMA_DEPRECATED_IN_1_2_FOR(f)    G_DEPRECATED_FOR(f)
#else
# define NMA_DEPRECATED_IN_1_2
# define NMA_DEPRECATED_IN_1_2_FOR(f)
#endif

#if NMA_VERSION_MIN_REQUIRED >= NMA_VERSION_1_8
# define NMA_DEPRECATED_IN_1_8           G_DEPRECATED
# define NMA_DEPRECATED_IN_1_8_FOR(f)    G_DEPRECATED_FOR(f)
#else
# define NMA_DEPRECATED_IN_1_8
# define NMA_DEPRECATED_IN_1_8_FOR(f)
#endif

#if NMA_VERSION_MAX_ALLOWED < NMA_VERSION_1_8
# define NMA_AVAILABLE_IN_1_8            G_UNAVAILABLE(1,8)
#else
# define NMA_AVAILABLE_IN_1_8
#endif

#if NMA_VERSION_MAX_ALLOWED < NMA_VERSION_1_8_22
# define NMA_AVAILABLE_IN_1_8_22            G_UNAVAILABLE(1.8,22)
#else
# define NMA_AVAILABLE_IN_1_8_22
#endif

#if NMA_VERSION_MAX_ALLOWED < NMA_VERSION_1_8_28
# define NMA_AVAILABLE_IN_1_8_28            G_UNAVAILABLE(1.8,28)
#else
# define NMA_AVAILABLE_IN_1_8_28
#endif

#if NMA_VERSION_MAX_ALLOWED < NMA_VERSION_1_8_34
# define NMA_AVAILABLE_IN_1_8_34            G_UNAVAILABLE(1.8,34)
#else
# define NMA_AVAILABLE_IN_1_8_34
#endif

#if NMA_VERSION_MAX_ALLOWED < NMA_VERSION_1_8_36
# define NMA_AVAILABLE_IN_1_8_36            G_UNAVAILABLE(1.8,36)
#else
# define NMA_AVAILABLE_IN_1_8_36
#endif

#endif  /* NMA_VERSION_H */
