/*
 * Copyright (C) 1999  Internet Software Consortium.
 * 
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

#ifndef NS_TYPES_H
#define NS_TYPES_H 1

#include <isc/list.h>

#include <dns/types.h>

typedef struct ns_client		ns_client_t;
typedef struct ns_clientmgr		ns_clientmgr_t;
typedef struct ns_query			ns_query_t;

/*
 * XXXRTH  This stuff is temporary.  As soon as we have zone and config
 *         support, it will go away.
 */

typedef struct ns_dbinfo {
	char *				path;
	char *				origin;
	isc_boolean_t			iscache;
	dns_db_t *			db;
	ISC_LINK(struct ns_dbinfo)	link;
} ns_dbinfo_t;

typedef ISC_LIST(ns_dbinfo_t)		ns_dbinfolist_t;

/*
 * XXXRTH  End of temporary stuff.
 */

#endif /* NS_TYPES_H */
