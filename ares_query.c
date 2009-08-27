/* Copyright 1998 by the Massachusetts Institute of Technology.
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in
 * advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 */

static const char rcsid[] = "$Id";

#include <sys/types.h>
#include <arpa/nameser.h>
#include <stdlib.h>
#include "ares.h"
#include "ares_private.h"

struct qquery {
  ares_callback callback;
  void *arg;
};

static void qcallback(void *arg, int status, unsigned char *abuf, int alen);

void ares_query(ares_channel channel, const char *name, int class,
		int type, ares_callback callback, void *arg)
{
  struct qquery *qquery;
  char *qbuf;
  int qlen, rd, status;

  rd = !(channel->flags & ARES_FLAG_NORECURSE);
  status = ares_mkquery(name, class, type, channel->next_id, rd, &qbuf, &qlen);
  channel->next_id++;
  if (status != ARES_SUCCESS)
    {
      callback(arg, status, NULL, 0);
      return;
    }

  qquery = malloc(sizeof(struct qquery));
  if (!qquery)
    {
      free(qbuf);
      callback(arg, ARES_ENOMEM, NULL, 0);
      return;
    }

  qquery->callback = callback;
  qquery->arg = arg;

  ares_send(channel, qbuf, qlen, qcallback, qquery);
  free(qbuf);
}

static void qcallback(void *arg, int status, unsigned char *abuf, int alen)
{
  struct qquery *qquery = (struct qquery *) arg;
  int rcode, ancount;

  if (status != ARES_SUCCESS)
    qquery->callback(qquery->arg, status, abuf, alen);
  else
    {
      /* Pull the response code and answer count from the packet. */
      rcode = abuf[3] & 0xf;
      ancount = abuf[6] << 8 | abuf[7];

      /* Convert errors. */
      switch (rcode)
	{
	case NOERROR:
	  status = (ancount > 0) ? ARES_SUCCESS : ARES_ENODATA;
	  break;
	case FORMERR:
	  status = ARES_EFORMERR;
	  break;
	case SERVFAIL:
	  status = ARES_ESERVFAIL;
	  break;
	case NXDOMAIN:
	  status = ARES_ENOTFOUND;
	  break;
	case NOTIMP:
	  status = ARES_ENOTIMP;
	  break;
	case REFUSED:
	  status = ARES_EREFUSED;
	  break;
	}
      qquery->callback(qquery->arg, status, abuf, alen);
    }
  free(qquery);
}
